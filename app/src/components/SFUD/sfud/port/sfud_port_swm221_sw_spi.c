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
#include <stdarg.h>

#include <sfud.h>

#include "SWM221.h"
#include "dev_sw_spi.h"

#define SPI_GPIO_CS          GPIOC
#define SPI_PIN_CS           PIN2

#define SPI_GPIO_SCLK        GPIOC
#define SPI_PIN_SCLK         PIN3

#define SPI_GPIO_MOSI        GPIOA
#define SPI_PIN_MOSI         PIN15

#define SPI_GPIO_MISO        GPIOB
#define SPI_PIN_MISO         PIN0

#define SPI_GPIO_DATA2       GPIOB
#define SPI_PIN_DATA2        PIN1

#define SPI_GPIO_DATA3       GPIOB
#define SPI_PIN_DATA3        PIN2

typedef struct {
    sw_spi_desc_t * sw_spi_desc;
} spi_user_data, *spi_user_data_t;

void sfud_log_debug(const char *file, const long line, const char *format, ...);
void sfud_log_info(const char *format, ...);

/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    uint32_t send_data = 0, read_data = 0;

    spi_user_data_t spi_dev = (spi_user_data_t)spi->user_data;

    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }

    sw_spi_cs_take(spi_dev->sw_spi_desc);
    /* 开始读写数据 */
    for (size_t i = 0; i < write_size + read_size; i++) {
        /* 先写缓冲区中的数据到 SPI 总线，数据写完后，再写 dummy(0xFF) 到 SPI 总线 */
        if (i < write_size) {
            send_data = *write_buf++;
        } else {
            send_data = SFUD_DUMMY_DATA;
        }
        /* 发送数据 && 接收数据 */
        read_data = sw_spi_readwrite(spi_dev->sw_spi_desc, send_data);
        /* 写缓冲区中的数据发完后，再读取 SPI 总线中的数据到读缓冲区 */
        if (i >= write_size) {
            *read_buf++ = read_data;
        }
    }
    
exit:
    sw_spi_cs_release(spi_dev->sw_spi_desc);

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

#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr, sfud_qspi_read_cmd_format *qspi_read_cmd_format,
        uint8_t *read_buf, size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
            
    /* 强制转成 line: 1-cmd, 1-addr, 4-data */
    if ( !(qspi_read_cmd_format->instruction_lines == 1 && qspi_read_cmd_format->address_lines == 1 
        && qspi_read_cmd_format->data_lines == 4) ) {
        SFUD_INFO("no support [instruction_lines](%d), [address_lines](%d), [data_lines](%d)\r\n", 
             qspi_read_cmd_format->instruction_lines, qspi_read_cmd_format->address_lines, qspi_read_cmd_format->data_lines);
        
        qspi_read_cmd_format->instruction_lines = 1;
        qspi_read_cmd_format->address_lines = 1;
        qspi_read_cmd_format->data_lines = 4;
        
        SFUD_INFO("change new [instruction_lines](%d), [address_lines](%d), [data_lines](%d)\r\n", 
             qspi_read_cmd_format->instruction_lines, qspi_read_cmd_format->address_lines, qspi_read_cmd_format->data_lines);
        
        if ( !(qspi_read_cmd_format->instruction == SFUD_CMD_QUAD_OUTPUT_READ_DATA
            && qspi_read_cmd_format->instruction == SFUD_CMD_QUAD_OUTPUT_READ_DATA + 1) ) {
            SFUD_INFO("no support [instruction](0x%x)\r\n", qspi_read_cmd_format->instruction);
            
            /* if medium size greater than 16Mb, use 4-Byte address, instruction should be added one */
            if (qspi_read_cmd_format->address_size == 24) {
                qspi_read_cmd_format->instruction = SFUD_CMD_QUAD_OUTPUT_READ_DATA;
            } else { //qspi_read_cmd_format->address_size == 32
                qspi_read_cmd_format->instruction = SFUD_CMD_QUAD_OUTPUT_READ_DATA + 1;
            }
            qspi_read_cmd_format->dummy_cycles = 8;
            
            SFUD_INFO("change new [instruction](0x%x)\r\n", qspi_read_cmd_format->instruction);
        }
    }
    spi_user_data_t spi_dev = (spi_user_data_t)spi->user_data;
    
    //CS select
    sw_spi_cs_take(spi_dev->sw_spi_desc);
    
    SFUD_INFO("[instruction_lines](%d), [address_lines](%d), [data_lines](%d)\r\n", 
            qspi_read_cmd_format->instruction_lines, qspi_read_cmd_format->address_lines, qspi_read_cmd_format->data_lines);
    SFUD_INFO("[instruction](0x%x), [addr](0x%x), [address_size](%d), [dummy_cycles](%d)\r\n", 
            qspi_read_cmd_format->instruction, addr, qspi_read_cmd_format->address_size, qspi_read_cmd_format->dummy_cycles);
    //cmd
    if (qspi_read_cmd_format->instruction_lines == 4) {
        sw_qspi_write(spi_dev->sw_spi_desc, qspi_read_cmd_format->instruction);
    }
    else if (qspi_read_cmd_format->instruction_lines == 1) {
        sw_spi_readwrite(spi_dev->sw_spi_desc, qspi_read_cmd_format->instruction);
    }
    //addr
    if (qspi_read_cmd_format->address_lines == 4) {
        for (uint32_t i = 0; i < qspi_read_cmd_format->address_size; i += 8) {
            sw_qspi_write(spi_dev->sw_spi_desc, addr >> (qspi_read_cmd_format->address_size - 8 - i));
        }
        for (uint32_t i = 0; i < qspi_read_cmd_format->dummy_cycles; i += 8) {
            sw_qspi_write(spi_dev->sw_spi_desc, 0xFF); //dummy_cycles == 6?   error: 6 % 4 = 2
        }
    }
    else if (qspi_read_cmd_format->address_lines == 1) {
        for (uint32_t i = 0; i < qspi_read_cmd_format->address_size; i += 8) {
            sw_spi_readwrite(spi_dev->sw_spi_desc, addr >> (qspi_read_cmd_format->address_size - 8 - i));
        }
        for (uint32_t i = 0; i < qspi_read_cmd_format->dummy_cycles; i += 8) {
            sw_spi_readwrite(spi_dev->sw_spi_desc, 0xFF);
        }
    }
    //data
    if (qspi_read_cmd_format->data_lines == 4) {
        for (uint32_t i = 0; i < read_size; ++i) {
            sw_qspi_read(spi_dev->sw_spi_desc, &read_buf[i]);
        }
    }
    else if (qspi_read_cmd_format->data_lines == 1) {
        for (uint32_t i = 0; i < read_size; ++i) {
            read_buf[i] = sw_spi_readwrite(spi_dev->sw_spi_desc, 0xFF);
        }
    }
exit: //CS release
    sw_spi_cs_release(spi_dev->sw_spi_desc);
    return result;
}
#endif /* SFUD_USING_QSPI */

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;
    
    sw_spi_cfg_t spi_cfg = {
        .obj = {
            .cs = {SPI_GPIO_CS, SPI_PIN_CS},
            .sclk = {SPI_GPIO_SCLK, SPI_PIN_SCLK},
            .mosi = {SPI_GPIO_MOSI, SPI_PIN_MOSI},
            .miso = {SPI_GPIO_MISO, SPI_PIN_MISO},
            .data2 = {SPI_GPIO_DATA2, SPI_PIN_DATA2},
            .data3 = {SPI_GPIO_DATA3, SPI_PIN_DATA3},
            
            .delay_tick = SPI_CLK_TO_TICK_DEFAULT(SystemCoreClock),
            .delay = NULL, //sw_spi_delay_default,
            
            .rw_flag = SW_SPI_FLAG_RW,
            .data_width = 8,
            .cpol = 0,
            .cpha = 0,
            .qspi_en = 0,
        },
    };
    static sw_spi_desc_t sw_spi_dev;
    /* software SPI init */
    sw_spi_init(&sw_spi_dev, &spi_cfg);
    
    static const spi_user_data spi_x = { .sw_spi_desc = &sw_spi_dev };

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
    flash->spi.qspi_read = qspi_read;
    #endif

    return result;
}

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
