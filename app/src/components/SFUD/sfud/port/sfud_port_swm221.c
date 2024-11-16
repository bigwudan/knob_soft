/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>

#include "SWM221.h"

#define SPI_GPIO_CS          GPIOB
#define SPI_PIN_CS           PIN15

#define SPI_X                SPI0

#define SPI_PORT_SCLK        PORTB
#define SPI_PIN_SCLK         PIN11
#define SPI_FUNMUX_SCLK      PORTB_PIN11_SPI0_SCLK

#define SPI_PORT_MOSI        PORTB
#define SPI_PIN_MOSI         PIN12
#define SPI_FUNMUX_MOSI      PORTB_PIN12_SPI0_MOSI

#define SPI_PORT_MISO        PORTB
#define SPI_PIN_MISO         PIN14
#define SPI_FUNMUX_MISO      PORTB_PIN14_SPI0_MISO


typedef struct {
    SPI_TypeDef *spix;
    GPIO_TypeDef *cs_gpiox;
    uint32_t cs_gpio_pin;
} spi_user_data, *spi_user_data_t;

void sfud_log_debug(const char *file, const long line, const char *format, ...);
void sfud_log_info(const char *format, ...);

sfud_err sfud_software_port_init(const sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;
    SFUD_ASSERT(flash);
  
#ifdef SFUD_USING_QSPI
    /* 设置此 QE 位的过程特定于制造商，并且在同一制造商的不同内存型号之间也可能发生变化。
     * 每个厂商几乎同时都有 QE bit 默认固化是开或者关的 Flash 型号，这在 Flash 规格书中会有体现.
     * 对于几乎所有内存制造商来说，QE 位是非易失性的，必须在执行任何需要 4 条 I/O 线的 SPI 命令之前设置。
     * 
     * SFUD-v1.1.0 采用的是 JESD216 (V1.0) 标准: 
     * 最初的版本 JESD216 里没有对于 QE bit 的定义（SFDP 里 Basic Flash Parameter Table 仅包含 9 个 DWORD），
     * 从 JESD216A 开始 QE bit 信息也被记录在了 SFDP 表中（Basic Flash Parameter Table 扩展到 16 个 DWORD）
     * 故 JESD216A 及以上版本的 Flash，直接读出 SFDP 表即可知道 QE bit 信息(但目前的 SFDP 未实现此部分 QE bit 的处理)。
     * 而对于不支持 SFDP 或者仅是 JESD216 版本 SFDP 的 Flash，需要查其数据手册找到 QE bit 信息手动处理。
     */
    #define __SFUD_CMD_READ_STATUS_REGISTER_2       ( 0x35 ) //Read_CMD register-2 ( QE bit )
    #define __SFUD_CMD_WRITE_STATUS_REGISTER_2      ( 0x31 ) //Write_CMD register-2 ( QE bit )
    
    uint8_t status;
    uint8_t cmd = __SFUD_CMD_READ_STATUS_REGISTER_2; //Read_CMD register-2 ( QE bit )
    result = flash->spi.wr(&flash->spi, &cmd, 1, &status, 1);
    if (SFUD_SUCCESS != result)
    {
        SFUD_INFO("Error: Read_status register-2 failed.");
        return result;
    }
    if (0 == (status & (1 << 1))) //Read QE bit
    {
        uint8_t register_status;
        cmd = SFUD_CMD_WRITE_ENABLE; //SFUD_CMD_WRITE_DISABLE
        result = flash->spi.wr(&flash->spi, &cmd, 1, NULL, 0);
        if (result == SFUD_SUCCESS) {
            result = sfud_read_status(flash, &register_status);
        }
        if (result == SFUD_SUCCESS) {
            if ((register_status & SFUD_STATUS_REGISTER_WEL) == 0) {
                SFUD_INFO("Error: Can't enable write status.");
                return SFUD_ERR_WRITE;
            }
        }
        
        if (result == SFUD_SUCCESS) {
            uint8_t cmd_data[2];
            cmd_data[0] = __SFUD_CMD_WRITE_STATUS_REGISTER_2;
            cmd_data[1] = status | (1 << 1); //Write QE bit
            result = flash->spi.wr(&flash->spi, cmd_data, 2, NULL, 0);
        }
        if (result != SFUD_SUCCESS) {
            SFUD_INFO("Error: Write_status register-2 failed.");
        }
    }
#endif
    return result;
}

/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    uint8_t send_data, read_data;

    spi_user_data_t spi_dev = (spi_user_data_t)spi->user_data;

    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }
    //SFUD_INFO("[%s]: [write_size](%d), [read_size](%d)\r\n", __FUNCTION__, write_size, read_size);
    
    GPIO_AtomicClrBit(spi_dev->cs_gpiox, spi_dev->cs_gpio_pin);
    /* 开始读写数据 */
    for (size_t i = 0; i < write_size + read_size; i++) {
        /* 先写缓冲区中的数据到 SPI 总线，数据写完后，再写 dummy(0xFF) 到 SPI 总线 */
        if (i < write_size) {
            send_data = *write_buf++;
        } else {
            send_data = SFUD_DUMMY_DATA;
        }
        /* 发送数据 && 接收数据 */
        read_data = SPI_ReadWrite(spi_dev->spix, send_data);
        
        /* 写缓冲区中的数据发完后，再读取 SPI 总线中的数据到读缓冲区 */
        if (i >= write_size) {
            *read_buf++ = read_data;
        }
    }
exit:
    GPIO_AtomicSetBit(spi_dev->cs_gpiox, spi_dev->cs_gpio_pin);
    return result;
}
        
/* about 100 microsecond delay */
static void retry_delay_100us(void) {
    for (uint32_t delay = CyclesPerUs * 100; delay > 0; --delay) __NOP();
}

static void spi_lock(const sfud_spi *spi) {
    __disable_irq();
}

static void spi_unlock(const sfud_spi *spi) {
    __enable_irq();
}

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;
    
    GPIO_INIT(SPI_GPIO_CS, SPI_PIN_CS, GPIO_OUTPUT);
    GPIO_AtomicSetBit(SPI_GPIO_CS, SPI_PIN_CS);
    
    PORT_Init(SPI_PORT_MOSI, SPI_PIN_MOSI, SPI_FUNMUX_MOSI, 0);
    PORT_Init(SPI_PORT_MISO, SPI_PIN_MISO, SPI_FUNMUX_MISO, 1);
    PORT_Init(SPI_PORT_SCLK, SPI_PIN_SCLK, SPI_FUNMUX_SCLK, 0);

    SPI_InitStructure SPI_initStruct;
    /* SPI_CLK 最高支持 30MHz 时钟, 时钟频率 60MHz 以下, 支持 2 分频；60MHz 以上, 则需 4 分频.
     * (如果时钟高于 24MHz, 则建议硬件上接滤波)
     */
    SPI_initStruct.clkDiv = (SystemCoreClock <= 6 * 1000 * 1000) ? SPI_CLKDIV_2 : SPI_CLKDIV_4;
    SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
    SPI_initStruct.SampleEdge = SPI_FIRST_EDGE;
    SPI_initStruct.IdleLevel = SPI_LOW_LEVEL;
    SPI_initStruct.WordSize = 8;
    SPI_initStruct.Master = 1;
    SPI_initStruct.RXThreshold = 0;
    SPI_initStruct.RXThresholdIEn = 0;
    SPI_initStruct.TXThreshold = 0;
    SPI_initStruct.TXThresholdIEn = 0;
    SPI_initStruct.TXCompleteIEn = 0;
    SPI_Init(SPI_X, &SPI_initStruct);
    SPI_Open(SPI_X);
    
    static const spi_user_data spi_x = { .spix = SPI_X, .cs_gpiox = SPI_GPIO_CS, .cs_gpio_pin = SPI_PIN_CS };
    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */
    flash->spi.wr = spi_write_read;
    flash->spi.lock = NULL;//spi_lock;
    flash->spi.unlock = NULL;//spi_unlock;
    flash->spi.user_data = (void *)&spi_x;
    flash->retry.delay = retry_delay_100us;
    flash->retry.times = 10000;
    #ifdef SFUD_USING_QSPI
    flash->spi.qspi_read = NULL;
    #endif

    return result;
}

#include <stdarg.h>

static char log_buf[256];

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}
