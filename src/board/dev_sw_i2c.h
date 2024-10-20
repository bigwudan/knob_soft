/**
 *******************************************************************************************************************************************
 * @file        dev_sw_i2c.h
 * @brief       IO 软件模拟 I2C 通讯
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
#ifndef __DEV_SW_I2C_H__
#define __DEV_SW_I2C_H__

#include <stdint.h>
#include "SWM221.h"

/** I2C 时钟频率(Hz)换算成对应内部实现的默认延时函数的延时数值(default: 机器周期__NOP()个数) */
#define I2C_CLK_TO_TICK_DEFAULT(__i2c_clk)               (SystemCoreClock / (__i2c_clk))

typedef struct i2c_gpio_desc_t
{
    GPIO_TypeDef *gpio;
    uint8_t pin;
} i2c_gpio_desc_t;

typedef struct sw_i2c_desc_t
{
    i2c_gpio_desc_t scl, sda;
    uint32_t delay_tick;          /* I2C 时钟延时数值 */
    uint8_t retry_times;          /* I2C 错误重试次数 */
    uint8_t addr_10b;             /* I2C 地址模式: 0 => 8bit(default)    1 => 10bit */
    void (*delay)(uint32_t tick); /* I2C 延时回调(user config optional) */
} sw_i2c_desc_t;

typedef struct sw_i2c_cfg_t
{
    sw_i2c_desc_t obj;
} sw_i2c_cfg_t;

/* sw_i2c_ret */
enum
{
    SW_I2C_RET_OK = 0,        // ACK
    SW_I2C_RET_NO_ACK,        // NACK
    SW_I2C_RET_ERROR_BUS_SCL, // scl bus arbitration fail
    SW_I2C_RET_ERROR_BUS_SDA, // sda bus arbitration fail
};

/**
 * @brief  构造实例
 * @param  dev : this
 * @param  cfg : see sw_i2c_cfg_t
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_init(sw_i2c_desc_t *dev, sw_i2c_cfg_t *cfg);

/**
 * @brief  析构实例
 * @param  dev : this
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_depose(sw_i2c_desc_t *dev);

/**
 * @brief  I2C 主机产生起始信号并发送从机地址
 * @param  dev     : this
 * @param  addr    : 从机地址
 * @param  restart : 是否为 restart
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_start(sw_i2c_desc_t *dev, uint16_t addr, uint8_t restart);

/**
 * @brief  I2C 主机读取 1 字节数据
 * @param  dev  : this
 * @param  data : 数据
 * @param  ack  : 应答信号(0-ACK / 1-NACK)
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_read(sw_i2c_desc_t *dev, uint8_t *data, uint8_t ack);

/**
 * @brief  I2C 主机传输 1 字节数据
 * @param  dev  : this
 * @param  data : 数据
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_write(sw_i2c_desc_t *dev, uint8_t data);

/**
 * @brief  I2C 主机产生停止信号
 * @param  dev : this
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_stop(sw_i2c_desc_t *dev);

#endif // __DEV_SW_I2C_H__
