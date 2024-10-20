/**
 *******************************************************************************************************************************************
 * @file        sw_spi_desc.h
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
#ifndef __DEV_SW_SPI_H__
#define __DEV_SW_SPI_H__

#include <stdint.h>
#include "SWM221.h"

/* SPI 时钟频率(Hz)换算成对应内部实现的默认延时函数的延时数值(default: 机器周期__NOP()个数) */
#define SPI_CLK_TO_TICK_DEFAULT(__spi_clk)              (SystemCoreClock / (__spi_clk))

/* SPI Mode = [CPOL, CPHA]
 * 0 [00]
 * 1 [01]
 * 2 [10]
 * 3 [11]
 * SPI Flash 常用模式 0 or 3
 */

typedef struct spi_gpio_desc_t
{
    GPIO_TypeDef *gpio;
    uint8_t pin;
} spi_gpio_desc_t;

typedef struct sw_spi_desc_t sw_spi_desc_t;
struct sw_spi_desc_t
{
    spi_gpio_desc_t cs, sclk, mosi, miso;
    spi_gpio_desc_t data2, data3; /* Use it for QSPI */

    uint32_t delay_tick; /* SPI 时钟频率对应的延时数值 */

    enum /* 读写标志(Only Read / Only Write / Read & Write) */
    {
        SW_SPI_FLAG_OR = (1 << 0),
        SW_SPI_FLAG_OW = (1 << 1),
        SW_SPI_FLAG_RW = SW_SPI_FLAG_OR | SW_SPI_FLAG_OW,
        __SW_SPI_FLAG_MAX__ = 0xFF,
    } rw_flag;

    /* SPI 传输数据位宽 / bit (不超过 32 bit, 使能 QSPI 则要被 4 整除) */
    uint8_t data_width;
    /* SPI 时钟极性 :
     * 0 = SCLK State Idle-Low, Active-High
     * 1 = SCLK State Idle-High, Active-Low
     */
    uint8_t cpol : 1;
    /* SPI 时钟相位 : 数据位采样相对于时钟线的时序
     * 0 = 在时钟信号 SCLK 的第 1 个跳变沿采样
     * 1 = 在时钟信号 SCLK 的第 2 个跳变沿采样
     */
    uint8_t cpha : 1;
    uint8_t qspi_en : 1; /* QSPI 四线使能 */
    uint8_t endian : 1;  /* 字节序: 0 => MSB(default)    1 => LSB(reserve) */

    void (*delay)(uint32_t tick); /* SPI 延时回调(user config optional) */
};

typedef struct sw_spi_cfg_t
{
    sw_spi_desc_t obj;
} sw_spi_cfg_t;

/* sw_spi_ret */
enum
{
    SW_SPI_RET_OK = 0,
    SW_SPI_RET_ERROR_RW_FLAG,
    SW_SPI_RET_NO_ENABLE_QSPI,
    SW_SPI_RET_NO_ALIGN,
};

/**
 * @brief  构造实例
 * @param  dev : this
 * @param  cfg : see sw_spi_cfg_t
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_init(sw_spi_desc_t *dev, sw_spi_cfg_t *cfg);

/**
 * @brief  析构实例
 * @param  dev : this
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_depose(sw_spi_desc_t *dev);

/**
 * @brief  主机获取片选信号
 * @param  dev : this
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_cs_take(sw_spi_desc_t *dev);

/**
 * @brief  主机释放片选信号
 * @param  dev : this
 * @retval see enum sw_spi_ret
 */
uint8_t sw_spi_cs_release(sw_spi_desc_t *dev);

/**
 * @brief  主机使用标准 SPI 读写从机
 * @param  dev        : this
 * @param  write_data : 写入的数据
 * @retval 读取的数据
 */
uint32_t sw_spi_readwrite(sw_spi_desc_t *dev, uint32_t write_data);

/**
 * @brief  主机使用 QSPI 写从机
 * @param  dev  : this
 * @param  data : 写入的数据
 * @retval see enum sw_spi_ret
 */
uint8_t sw_qspi_write(sw_spi_desc_t *dev, uint32_t data);

/**
 * @brief  主机使用 QSPI 读从机
 * @param  dev  : this
 * @param  data : 读取的数据
 * @retval see enum sw_spi_ret
 */
uint8_t sw_qspi_read(sw_spi_desc_t *dev, void *data);

#endif // __DEV_SW_SPI_H__
