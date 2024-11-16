/**
 *******************************************************************************************************************************************
 * @file        dev_sw_spi.c
 * @brief       IO 软件模拟 SPI 通讯
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include <string.h>
#include "dev_sw_spi.h"

#if 0 /* Debug */
#include <stdio.h>
# define SPI_LOG(...)      printf(__VA_ARGS__)
#else 
# define SPI_LOG(...)      
#endif
#if 1 /* assert */
//#include <assert.h>
# define SPI_ASSERT(expr)      if (expr) for(;;) __NOP() /*SPI_LOG("[%s]\r\n", __FUNCTION__);*/
#else 
# define SPI_ASSERT(expr)      
#endif
/*******************************************************************************************************************************************
 * static defines
 *******************************************************************************************************************************************/
/* platform port */
enum /* sta 应兼容 SWMxxx_GPIO.h */
{
    GPIO_STA_INPUT = (0 << 0),  /* 输入方向, 不可以和输出方向进行 “|” 操作 */
    GPIO_STA_OUTPUT = (1 << 0), /* 输出方向, 不可以和输入方向进行 “|” 操作 */
    GPIO_STA_PU = (1 << 1),     /* pull_up */
    GPIO_STA_PD = (1 << 2),     /* pull_down */
    GPIO_STA_OD = (1 << 3),     /* open_drain */
};

#define SPI_GPIO_INIT(gpio_desc, sta)      GPIO_INIT((gpio_desc).gpio, (gpio_desc).pin, sta)
#define SPI_GPIO_CTRL_OUT(gpio_desc)       \
    if (0 == ((((gpio_desc).gpio)->DIR) & (0x01 << ((gpio_desc).pin)))) (((gpio_desc).gpio)->DIR) |= (0x01 << ((gpio_desc).pin))
#define SPI_GPIO_CTRL_IN(gpio_desc)        \
    if (0 != ((((gpio_desc).gpio)->DIR) & (0x01 << ((gpio_desc).pin)))) (((gpio_desc).gpio)->DIR) &= ~(0x01 << ((gpio_desc).pin))
#define SPI_GPIO_SET(gpio_desc, level)     \
    if (level) GPIO_AtomicSetBit((gpio_desc).gpio, (gpio_desc).pin); else GPIO_AtomicClrBit((gpio_desc).gpio, (gpio_desc).pin)
#define SPI_GPIO_GET(gpio_desc)             GPIO_GetBit((gpio_desc).gpio, (gpio_desc).pin)

/*******************************************************************************************************************************************
 * static prototypes
 *******************************************************************************************************************************************/
/** sw_spi_desc port */
#define SCLK_IDLE(spi_desc)          SPI_GPIO_SET((spi_desc)->sclk, ((spi_desc)->cpol) ? 1 : 0)
#define SCLK_BUSY(spi_desc)          SPI_GPIO_SET((spi_desc)->sclk, ((spi_desc)->cpol) ? 0 : 1)

#define CS_L(spi_desc)               SPI_GPIO_SET((spi_desc)->cs, 0)
#define CS_H(spi_desc)               SPI_GPIO_SET((spi_desc)->cs, 1)

#define MOSI_SET(spi_desc, level)            SPI_GPIO_SET((spi_desc)->mosi, level)
#define MISO_SET(spi_desc, level)            SPI_GPIO_SET((spi_desc)->miso, level)
#define DATA2_SET(spi_desc, level)           SPI_GPIO_SET((spi_desc)->data2, level)
#define DATA3_SET(spi_desc, level)           SPI_GPIO_SET((spi_desc)->data3, level)

#define MOSI_GET(spi_desc)                   SPI_GPIO_GET((spi_desc)->mosi)
#define MISO_GET(spi_desc)                   SPI_GPIO_GET((spi_desc)->miso)
#define DATA2_GET(spi_desc)                  SPI_GPIO_GET((spi_desc)->data2)
#define DATA3_GET(spi_desc)                  SPI_GPIO_GET((spi_desc)->data3)

#define SET_QSPI_READ(spi_desc)            \
    do {                                   \
        SPI_GPIO_CTRL_IN(spi_desc->mosi);  \
        SPI_GPIO_CTRL_IN(spi_desc->miso);  \
        SPI_GPIO_CTRL_IN(spi_desc->data2); \
        SPI_GPIO_CTRL_IN(spi_desc->data3); \
    } while(0)

#define SET_QSPI_WRITE(spi_desc)            \
    do {                                    \
        SPI_GPIO_CTRL_OUT(spi_desc->mosi);  \
        SPI_GPIO_CTRL_OUT(spi_desc->miso);  \
        SPI_GPIO_CTRL_OUT(spi_desc->data2); \
        SPI_GPIO_CTRL_OUT(spi_desc->data3); \
    } while(0)

#define RESET_STD_SPI(spi_desc)            \
    do {                                   \
        SPI_GPIO_CTRL_OUT(spi_desc->mosi); \
        SPI_GPIO_CTRL_IN(spi_desc->miso);  \
        SPI_GPIO_CTRL_IN(spi_desc->data2); \
        SPI_GPIO_CTRL_IN(spi_desc->data3); \
    } while(0)

#define UNIT_BIT(spi_desc)           ((spi_desc)->data_width)
#define SPI_CLK_DELAY(spi_desc)      (spi_desc)->delay(((spi_desc)->delay_tick + 1) >> 1)

/**
 * @brief 软件模拟 SPI 通讯延时保持的默认实现
 */
static void sw_spi_delay_default(uint32_t tick)
{
    uint32_t i = 0;
    do {
        __NOP(); /* At least 1 NOP */
    } while (++i < tick);
}

/*******************************************************************************************************************************************
 * public functions
 *******************************************************************************************************************************************/
/**
 * @brief  构造实例
 * @param  dev : this
 * @param  cfg : see sw_spi_cfg_t
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_init(sw_spi_desc_t *dev, sw_spi_cfg_t *cfg)
{
    SPI_ASSERT(!dev);
    if (cfg) {
        SPI_ASSERT(!(cfg->obj.data_width));
        memcpy(dev, &cfg->obj, sizeof(sw_spi_desc_t));
        if (!dev->delay) {
            dev->delay = sw_spi_delay_default; /* default */
        }
    }
    SPI_GPIO_INIT(dev->cs, GPIO_STA_OUTPUT | GPIO_STA_PU);
    CS_H(dev);

    SPI_GPIO_INIT(dev->sclk, GPIO_STA_OUTPUT);
    SCLK_IDLE(dev);

    if ( (SW_SPI_FLAG_OW & dev->rw_flag) || dev->qspi_en ) {
        SPI_GPIO_INIT(dev->mosi, GPIO_STA_OUTPUT);
        MOSI_SET(dev, 0);
    }
    if ( (SW_SPI_FLAG_OR & dev->rw_flag) || dev->qspi_en ) {
        SPI_GPIO_INIT(dev->miso, GPIO_STA_INPUT | GPIO_STA_PU); /* Set High Impedance Mode(IO Input & Pull_Up) */
    }
    /* WP/HOLD 端口: (Active-Low)
     * Standard SPI 操作模式下, WP/HOLD 有效, 端口按用户实际需求进行配置(可选);
     * QSPI 操作模式下, WP/HOLD 失去原有功能，变成 QSPI mode 下的双向数据传输端口;
     */
    if (dev->qspi_en) {
        SPI_GPIO_INIT(dev->data2, GPIO_STA_INPUT | GPIO_STA_PU);
        SPI_GPIO_INIT(dev->data3, GPIO_STA_INPUT | GPIO_STA_PU);
    }
    return SW_SPI_RET_OK;
}

/**
 * @brief  析构实例
 * @param  dev : this
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_depose(sw_spi_desc_t *dev)
{
    SPI_ASSERT(!dev);
    memset(dev, 0, sizeof(sw_spi_desc_t));
    //free(dev); //unused(dev)
    return SW_SPI_RET_OK;
}

/**
 * @brief  主机获取片选信号
 * @param  dev : this
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_cs_take(sw_spi_desc_t *dev)
{
    SPI_ASSERT(!dev);
    CS_L(dev);
    return SW_SPI_RET_OK;
}

/**
 * @brief  主机释放片选信号
 * @param  dev : this
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_cs_release(sw_spi_desc_t *dev)
{
    SPI_ASSERT(!dev);
    CS_H(dev);
    return SW_SPI_RET_OK;
}

/**
 * @brief  主机使用标准 SPI 读写从机
 * @param  dev        : this
 * @param  write_data : 写入的数据
 * @retval 读取的数据
 */
uint32_t sw_spi_readwrite(sw_spi_desc_t *dev, uint32_t write_data)
{
    SPI_ASSERT(!(dev && UNIT_BIT(dev)));

    if (dev->qspi_en) {
        RESET_STD_SPI(dev);
    }

    uint32_t read_data = 0; //only | set 1, no &~ clear 0, so pre clear
    uint8_t bit_msb = UNIT_BIT(dev) - 1;

    if (dev->cpha) { /* CPHA == 1 */
        for (uint8_t bit = 0; bit < UNIT_BIT(dev); ++bit) {
            SCLK_BUSY(dev);
            /* Write-MSB */
            if (SW_SPI_FLAG_OW & dev->rw_flag) {
                MOSI_SET(dev, write_data & ( 1 << (bit_msb - bit) ) );
            }
            SPI_CLK_DELAY(dev);
            SCLK_IDLE(dev);
            SPI_CLK_DELAY(dev);
            /* Read-MSB */
            if (SW_SPI_FLAG_OR & dev->rw_flag) {
                read_data |= (MISO_GET(dev) << (bit_msb - bit));
            }
        }
    } else { /* CPHA == 0 */
        for (uint8_t bit = 0; bit < UNIT_BIT(dev); ++bit) {
            /* Write-MSB */
            if (SW_SPI_FLAG_OW & dev->rw_flag) {
                MOSI_SET(dev, write_data & ( 1 << (bit_msb - bit) ) );
            }
            SCLK_BUSY(dev);
            SPI_CLK_DELAY(dev);
            /* Read-MSB */
            if (SW_SPI_FLAG_OR & dev->rw_flag) {
                read_data |= (MISO_GET(dev) << (bit_msb - bit));
            }
            SCLK_IDLE(dev);
            SPI_CLK_DELAY(dev);
        }
    }
    return read_data;
}

/**
 * @brief  主机使用 QSPI 写从机
 * @param  dev  : this
 * @param  data : 写入的数据
 * @retval see enum sw_spi_ret
 */
uint8_t sw_qspi_write(sw_spi_desc_t *dev, uint32_t data)
{
    SPI_ASSERT(!(dev && 0 == (UNIT_BIT(dev) & (4 - 1))));
    if (!(SW_SPI_FLAG_OW & dev->rw_flag)) {
        return SW_SPI_RET_ERROR_RW_FLAG;
    }
    if (0 == dev->qspi_en) {
        return SW_SPI_RET_NO_ENABLE_QSPI;
    }
    SET_QSPI_WRITE(dev);
    
/* Write-MSB: D3->D2->MISO->M0SI */
#define __QSPI_W() \
    DATA3_SET(dev, (data) & ( 1 << (bit_msb - bit++) ) ); \
    DATA2_SET(dev, (data) & ( 1 << (bit_msb - bit++) ) ); \
    MISO_SET(dev, (data) & ( 1 << (bit_msb - bit++) ) ); \
    MOSI_SET(dev, (data) & ( 1 << (bit_msb - bit++) ) )

    uint8_t bit_msb = UNIT_BIT(dev) - 1;

    if (dev->cpha) { /* CPHA == 1 */
        for (uint8_t bit = 0; bit < UNIT_BIT(dev); ) {
            SCLK_BUSY(dev);
            __QSPI_W();
            SPI_CLK_DELAY(dev);
            SCLK_IDLE(dev);
            SPI_CLK_DELAY(dev);
        }
    } else { /* CPHA == 0 */
        for (uint8_t bit = 0; bit < UNIT_BIT(dev); ) {
            __QSPI_W();
            SCLK_BUSY(dev);
            SPI_CLK_DELAY(dev);
            SCLK_IDLE(dev);
            SPI_CLK_DELAY(dev);
        }
    }
    return SW_SPI_RET_OK;
}

/**
 * @brief  主机使用 QSPI 读从机
 * @param  dev  : this
 * @param  data : 读取的数据
 * @retval see enum sw_spi_ret
 */
uint8_t sw_qspi_read(sw_spi_desc_t *dev, void *data)
{
    SPI_ASSERT(!(dev && 0 == (UNIT_BIT(dev) & (4 - 1)) && data));
    if (!(SW_SPI_FLAG_OR & dev->rw_flag)) {
        return SW_SPI_RET_ERROR_RW_FLAG;
    }
    if (0 == dev->qspi_en) {
        return SW_SPI_RET_NO_ENABLE_QSPI;
    }
    SET_QSPI_READ(dev);

/* Read-MSB: D3->D2->MISO->M0SI */
#define __QSPI_R() \
    (read_data) |= (DATA3_GET(dev)) << (bit_msb - bit++); \
    (read_data) |= (DATA2_GET(dev)) << (bit_msb - bit++); \
    (read_data) |= (MISO_GET(dev)) << (bit_msb - bit++); \
    (read_data) |= (MOSI_GET(dev)) << (bit_msb - bit++)

    uint32_t read_data = 0; //only | set 1, no &~ clear 0, so pre clear
    uint8_t bit_msb = UNIT_BIT(dev) - 1;

    if (dev->cpha) { /* CPHA == 1 */
        for (uint8_t bit = 0; bit < UNIT_BIT(dev); ) {
            SCLK_BUSY(dev);
            SPI_CLK_DELAY(dev);
            SCLK_IDLE(dev);
            SPI_CLK_DELAY(dev);
            __QSPI_R();
        }
    } else { /* CPHA == 0 */
        for (uint8_t bit = 0; bit < UNIT_BIT(dev); ) {
            SCLK_BUSY(dev);
            SPI_CLK_DELAY(dev);
            __QSPI_R();
            SCLK_IDLE(dev);
            SPI_CLK_DELAY(dev);
        }
    }
    if (UNIT_BIT(dev) > 16) {
        if ((uint32_t)data & (4 - 1)) {
            return SW_SPI_RET_NO_ALIGN;
        }
        *(uint32_t *)data = read_data;
    }
    else if (UNIT_BIT(dev) > 8 && 16 <= UNIT_BIT(dev)) {
        if ((uint32_t)data & (2 - 1)) {
            return SW_SPI_RET_NO_ALIGN;
        }
        *(uint16_t *)data = read_data & 0xFFFF;
    }
    else {
        *(uint8_t *)data = read_data & 0xFF;
    }
    return SW_SPI_RET_OK;
}
