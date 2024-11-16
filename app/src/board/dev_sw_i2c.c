/**
 *******************************************************************************************************************************************
 * @file        dev_sw_i2c.c
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
#include <string.h>
#include "dev_sw_i2c.h"

#if 0 /* debug */
#include <stdio.h>
# define I2C_LOG(...)      printf(__VA_ARGS__)
#else 
# define I2C_LOG(...)      
#endif
#if 1 /* assert */
//#include <assert.h>
# define I2C_ASSERT(expr)      if (expr) for(;;) __NOP() /*I2C_LOG("[%s]\r\n", __FUNCTION__)*/
#else 
# define I2C_ASSERT(expr)      
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

#define I2C_GPIO_INIT(gpio_desc, sta)      GPIO_INIT((gpio_desc).gpio, (gpio_desc).pin, sta)
#define I2C_GPIO_CTRL_OUT(gpio_desc)       \
    if (0 == ((((gpio_desc).gpio)->DIR) & (0x01 << ((gpio_desc).pin)))) (((gpio_desc).gpio)->DIR) |= (0x01 << ((gpio_desc).pin))
#define I2C_GPIO_CTRL_IN(gpio_desc)        \
    if (0 != ((((gpio_desc).gpio)->DIR) & (0x01 << ((gpio_desc).pin)))) (((gpio_desc).gpio)->DIR) &= ~(0x01 << ((gpio_desc).pin))
//#define I2C_GPIO_H(gpio_desc)               GPIO_AtomicSetBit((gpio_desc).gpio, (gpio_desc).pin)
#define I2C_GPIO_L(gpio_desc)               GPIO_AtomicClrBit((gpio_desc).gpio, (gpio_desc).pin)
#define I2C_GPIO_GET(gpio_desc)             GPIO_GetBit((gpio_desc).gpio, (gpio_desc).pin)

enum /* 兼容固件库的 ACK/NAK 传参 */
{
    SW_I2C_NAK = 0,
    SW_I2C_ACK = 1,
};

/* i2c_desc port */
#define SDA_INIT(i2c_desc)           I2C_GPIO_INIT((i2c_desc)->sda, GPIO_STA_INPUT | GPIO_STA_PU | GPIO_STA_OD)
#define SDA_H(i2c_desc)              I2C_GPIO_CTRL_IN((i2c_desc)->sda)
#define SDA_L(i2c_desc)              I2C_GPIO_CTRL_OUT((i2c_desc)->sda); I2C_GPIO_L((i2c_desc)->sda)
#define SDA_GET(i2c_desc)            I2C_GPIO_GET((i2c_desc)->sda)

#define SCL_INIT(i2c_desc)           I2C_GPIO_INIT((i2c_desc)->scl, GPIO_STA_INPUT | GPIO_STA_PU | GPIO_STA_OD)
#define SCL_H(i2c_desc)              I2C_GPIO_CTRL_IN((i2c_desc)->scl)
#define SCL_L(i2c_desc)              I2C_GPIO_CTRL_OUT((i2c_desc)->scl); I2C_GPIO_L((i2c_desc)->scl)
#define SCL_GET(i2c_desc)            I2C_GPIO_GET((i2c_desc)->scl)

#define I2C_CLK_DELAY(i2c_desc)      (i2c_desc)->delay(((i2c_desc)->delay_tick + 1) >> 1)

/*******************************************************************************************************************************************
 * static prototypes
 *******************************************************************************************************************************************/
static uint8_t sw_i2c_bus_unlock(sw_i2c_desc_t *dev);
static uint8_t sw_i2c_start_signal(sw_i2c_desc_t *dev);
static uint8_t sw_i2c_stop_signal(sw_i2c_desc_t *dev);
static uint8_t sw_i2c_wait_ack_signal(sw_i2c_desc_t *dev, uint8_t *ack);
static uint8_t sw_i2c_send_byte(sw_i2c_desc_t *dev, uint8_t data);
static uint8_t sw_i2c_reply_ack_signal(sw_i2c_desc_t *dev, uint8_t ack);
static uint8_t sw_i2c_recv_byte(sw_i2c_desc_t *dev, uint8_t *data);
static uint8_t sw_i2c_send_addr(sw_i2c_desc_t *dev, uint16_t addr, uint8_t restart);

/**
 * @brief 软件模拟 I2C 通讯延时保持的默认实现
 */
static void sw_i2c_delay_default(uint32_t tick)
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
 * @param  cfg : see sw_i2c_cfg_t
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_init(sw_i2c_desc_t *dev, sw_i2c_cfg_t *cfg)
{
    I2C_ASSERT(!dev);
    if (cfg) {
        memcpy(dev, &cfg->obj, sizeof(sw_i2c_desc_t));
        if (!dev->delay) {
            dev->delay = sw_i2c_delay_default; /* default */
        }
    }
    SDA_INIT(dev);
    SCL_INIT(dev);
    return SW_I2C_RET_OK;
}

/**
 * @brief  析构实例
 * @param  dev : this
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_depose(sw_i2c_desc_t *dev)
{
    I2C_ASSERT(!dev);
    memset(dev, 0, sizeof(sw_i2c_desc_t));
    //free(dev); //unused(dev)
    return SW_I2C_RET_OK;
}

/**
 * @brief  I2C 主机产生起始信号并发送从机地址
 * @param  dev     : this
 * @param  addr    : 从机地址
 * @param  restart : 是否为 restart
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_start(sw_i2c_desc_t *dev, uint16_t addr, uint8_t restart)
{
    I2C_ASSERT(!dev);

    uint8_t res = SW_I2C_RET_OK;
    res = sw_i2c_start_signal(dev);
    if (SW_I2C_RET_OK != res) {
        res = sw_i2c_bus_unlock(dev);
        if (SW_I2C_RET_OK != res) {
            return res;
        }
        res = sw_i2c_start_signal(dev);
        if (SW_I2C_RET_OK != res) {
            return res;
        }
    }
    return sw_i2c_send_addr(dev, addr, restart);
}

/**
 * @brief  I2C 主机读取 1 字节数据
 * @param  dev  : this
 * @param  data : 数据
 * @param  ack  : 应答信号(0-ACK / 1-NACK)
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_read(sw_i2c_desc_t *dev, uint8_t *data, uint8_t ack)
{
    I2C_ASSERT(!(dev && data));

    uint8_t res = SW_I2C_RET_OK;
    res = sw_i2c_recv_byte(dev, data);
    if (SW_I2C_RET_OK != res) {
        return res;
    }
    res = sw_i2c_reply_ack_signal(dev, ack);
    return res;
}

/**
 * @brief  I2C 主机传输 1 字节数据
 * @param  dev  : this
 * @param  data : 数据
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_write(sw_i2c_desc_t *dev, uint8_t data)
{
    I2C_ASSERT(!dev);

    uint8_t ack = SW_I2C_ACK;
    uint8_t res = SW_I2C_RET_OK;
    res = sw_i2c_send_byte(dev, data);
    if (SW_I2C_RET_OK != res) {
        return res;
    }
    res = sw_i2c_wait_ack_signal(dev, &ack);
    if (SW_I2C_RET_OK != res) {
        return res;
    }
    return (SW_I2C_ACK == ack) ? SW_I2C_RET_OK : SW_I2C_RET_NO_ACK;
}

/**
 * @brief  I2C 主机产生停止信号
 * @param  dev : this
 * @retval see enum sw_i2c_ret
 */
uint8_t sw_i2c_stop(sw_i2c_desc_t *dev)
{
    I2C_ASSERT(!dev);
    uint8_t res = SW_I2C_RET_OK;
    res = sw_i2c_stop_signal(dev);
    I2C_CLK_DELAY(dev); //start half time cycle
    I2C_CLK_DELAY(dev); //end half time cycle
    return res;
}

/*******************************************************************************************************************************************
 * static functions
 *******************************************************************************************************************************************/
/**
 * @brief  I2C "开漏线与" 特性, 时钟同步（高电平延展）
 * @param  dev : this
 * @retval SW_I2C_RET_OK            : 总线空闲, 接下来可以拉低占用总线获取控制权
 * @retval SW_I2C_RET_ERROR_BUS_SCL : 总线忙碌, 无法获取总线控制权
 */
static uint8_t sw_i2c_clk_sync(sw_i2c_desc_t *dev)
{
    for (uint8_t retry = 0; retry <= dev->retry_times; ++retry) {
        if (SCL_GET(dev)) {
            return SW_I2C_RET_OK;
        }
        I2C_CLK_DELAY(dev); //延展
    }
    return SW_I2C_RET_ERROR_BUS_SCL;
}

/**
 * @brief  I2C "开漏线与" 特性, 仲裁 SDA 总线上的数据是否与本主机发送的一致(软件读仅能仲裁高电平)
 * @param  dev : this
 * @retval SW_I2C_RET_OK            : 主机未输掉仲裁
 * @retval SW_I2C_RET_ERROR_BUS_SDA : 主机输掉了仲裁, 需要停止传输数据
 */
static uint8_t sw_i2c_sda_arbitration(sw_i2c_desc_t *dev)
{
    if (SDA_GET(dev)) {
        return SW_I2C_RET_OK;
    }
    return SW_I2C_RET_ERROR_BUS_SDA;
}

/**
 * if i2c is locked, this function will unlock it
 *
 * @param i2c desc class.
 *
 * @return SW_I2C_RET_OK indicates successful unlock.
 */
static uint8_t sw_i2c_bus_unlock(sw_i2c_desc_t *dev)
{
    SDA_H(dev);

    if (0 == SDA_GET(dev))
    {
        for (uint8_t i = 0; i < 9; ++i) // 8bit + ack/nack
        {
            SCL_H(dev);
            I2C_CLK_DELAY(dev);
            SCL_L(dev);
            I2C_CLK_DELAY(dev);
        }
    }
    if (0 == SDA_GET(dev))
    {
        return SW_I2C_RET_ERROR_BUS_SDA;
    }
    return SW_I2C_RET_OK;
}

/**
 * @brief  I2C 主机发送起始信号(start / restart)
 * @param  dev : this
 * @retval SW_I2C_RET_OK : success
 * @retval other         : error code
 */
static uint8_t sw_i2c_start_signal(sw_i2c_desc_t *dev)
{
    uint8_t res = SW_I2C_RET_OK;

    //start: scl/sda H => scl H + sda L 
    //restart: scl L + sda H/L => scl/sda H => scl H + sda L 

    SDA_H(dev);
    res = sw_i2c_sda_arbitration(dev);
    if (SW_I2C_RET_OK != res) {
        return res;
    }

    SCL_H(dev);
    I2C_CLK_DELAY(dev); //end half time cycle

    res = sw_i2c_clk_sync(dev);
    if (SW_I2C_RET_OK != res) {
        return res;
    }

    /* 发送起始时序 */
    I2C_CLK_DELAY(dev); //start half time cycle
    SDA_L(dev);
    I2C_CLK_DELAY(dev); //end half time cycle

    SCL_L(dev);
    return res;
}

/**
 * @brief  I2C 主机发送停止信号
 * @param  dev : this
 * @retval SW_I2C_RET_OK : success
 * @retval other         : error code
 */
static uint8_t sw_i2c_stop_signal(sw_i2c_desc_t *dev)
{
    uint8_t res = SW_I2C_RET_OK;

    //stop: sda H/L + scl L => sda L + scl L => sda L + scl H => sda H + scl H
    SDA_L(dev);
    I2C_CLK_DELAY(dev); //end half time cycle

    SCL_H(dev);
    I2C_CLK_DELAY(dev); //start half time cycle

    res = sw_i2c_clk_sync(dev);
    if (SW_I2C_RET_OK != res) {
        return res;
    }

    I2C_CLK_DELAY(dev); //end half time cycle
    SDA_H(dev);
    I2C_CLK_DELAY(dev); //start half time cycle
    I2C_CLK_DELAY(dev); //end half time cycle
    res = sw_i2c_sda_arbitration(dev);
    return res;
}

/**
 * @brief  I2C 主机接收从机应答信号
 * @param  dev : this
 * @param  ack : 应答信号(0-ACK / 1-NACK)
 * @retval SW_I2C_RET_OK : success
 * @retval other         : error code
 */
static uint8_t sw_i2c_wait_ack_signal(sw_i2c_desc_t *dev, uint8_t *ack)
{
    uint8_t res = SW_I2C_RET_OK;

    SDA_H(dev);
    I2C_CLK_DELAY(dev); //end half time cycle

    SCL_H(dev);
    I2C_CLK_DELAY(dev); //start half time cycle

    res = sw_i2c_clk_sync(dev);
    if (SW_I2C_RET_OK != res) {
        return res;
    }
    
    *ack = (SDA_GET(dev)) ? SW_I2C_NAK : SW_I2C_ACK;
    I2C_LOG("[%s]: ack [%s]\r\n", __FUNCTION__, (SW_I2C_ACK == *ack) ? "ACK" : "NACK");
    
    SCL_L(dev);
    return res;
}

/**
 * @brief  I2C 主机传输 1 Byte 数据
 * @note   本函数未回复 ack, 需紧接着调用 sw_i2c_wait_ack_signal(dev, &ack);
 * @param  dev  : this
 * @param  data : 数据
 * @retval SW_I2C_RET_OK : success
 * @retval other         : error code
 */
static uint8_t sw_i2c_send_byte(sw_i2c_desc_t *dev, uint8_t data)
{
    uint8_t res = SW_I2C_RET_OK;

    for (uint8_t i = 0; i < 8; ++i)
    {
        SCL_L(dev);

        if ( data & ( 1 << ( (8 - 1) - i ) ) ) { /* MSB */
            SDA_H(dev);
            I2C_CLK_DELAY(dev); //start half time cycle

            res = sw_i2c_sda_arbitration(dev);
            if (SW_I2C_RET_OK != res) {
                return res;
            }
        } else {
            SDA_L(dev);
            I2C_CLK_DELAY(dev); //start half time cycle
        }

        SCL_H(dev);
        I2C_CLK_DELAY(dev); //end half time cycle

        res = sw_i2c_clk_sync(dev);
        if (SW_I2C_RET_OK != res) {
            return res;
        }
    }
    I2C_LOG("[%s]: data 0x[%x]\r\n", __FUNCTION__, data);
    SCL_L(dev);
    I2C_CLK_DELAY(dev); //start half time cycle
    return res;
}


/**
 * @brief  I2C 主机回复从机应答信号
 * @param  dev : this
 * @param  ack : 应答信号(0-ACK / 1-NACK)
 * @retval SW_I2C_RET_OK : success
 * @retval other         : error code
 */
static uint8_t sw_i2c_reply_ack_signal(sw_i2c_desc_t *dev, uint8_t ack)
{
    uint8_t res = SW_I2C_RET_OK;
    
    I2C_LOG("[%s]: ack [%s]\r\n", __FUNCTION__, (SW_I2C_ACK == ack) ? "ACK" : "NACK");
    if (SW_I2C_ACK == ack)
    {
        SDA_L(dev);
    }
    I2C_CLK_DELAY(dev); //end half time cycle

    SCL_H(dev);
    I2C_CLK_DELAY(dev); //start half time cycle

    res = sw_i2c_clk_sync(dev);
    if (SW_I2C_RET_OK != res) {
        return res;
    }

    SCL_L(dev);
    return res;
}

/**
 * @brief  I2C 主机接收 1 Byte 数据
 * @note   本函数未回复 ack, 需紧接着调用 sw_i2c_reply_ack_signal(dev, ack);
 * @param  dev  : this
 * @param  data : 数据
 * @retval SW_I2C_RET_OK : success
 * @retval other         : error code
 */
static uint8_t sw_i2c_recv_byte(sw_i2c_desc_t *dev, uint8_t *data)
{
    uint8_t res = SW_I2C_RET_OK;

    SDA_H(dev);
    I2C_CLK_DELAY(dev); //start half time cycle
    for (uint8_t i = 0; i < 8; ++i)
    {
        SCL_H(dev);
        I2C_CLK_DELAY(dev); //(first: end) start half time cycle

        res = sw_i2c_clk_sync(dev);
        if (SW_I2C_RET_OK != res) {
            return res;
        }

        *data |= ( SDA_GET(dev) << ( (8 - 1) - i ) ); /* MSB */

        SCL_L(dev);
        I2C_CLK_DELAY(dev); //(first: start + end) end + start half = total time cycle
        I2C_CLK_DELAY(dev); 
    }
    I2C_LOG("[%s]: data 0x[%x]\r\n", __FUNCTION__, *data);
    return res;
}

/**
 * @brief  I2C 主机发送从机地址(8bit)
 * @param  dev  : this
 * @param  addr : 从机地址(带读写控制位)
 * @retval SW_I2C_RET_OK : success
 * @retval other         : error code
 */
static uint8_t sw_i2c_send_addr_bytes(sw_i2c_desc_t *dev, uint8_t addr)
{
    uint8_t res = SW_I2C_RET_OK;
    uint8_t ack = SW_I2C_NAK;
    for (uint8_t retry = 0; retry <= dev->retry_times; ++retry) {
        res = sw_i2c_send_byte(dev, addr);
        if (SW_I2C_RET_OK != res) {
            return res;
        }
        res = sw_i2c_wait_ack_signal(dev, &ack);
        if (SW_I2C_RET_OK != res) {
            return res;
        }
        I2C_LOG("[%s]: sw_i2c_wait_ack_signal is [%d], retry[%d]\r\n", __FUNCTION__, ack, retry);
        if (SW_I2C_ACK == ack) {
            break;
        }
        /* SW_I2C_NCK continue */
        res = sw_i2c_stop_signal(dev);
        if (SW_I2C_RET_OK != res) {
            return res;
        }
        I2C_CLK_DELAY(dev); //start half time cycle
        I2C_CLK_DELAY(dev); //end half time cycle
        res = sw_i2c_start_signal(dev);
        if (SW_I2C_RET_OK != res) {
            return res;
        }
    }
    return (SW_I2C_ACK == ack) ? SW_I2C_RET_OK : SW_I2C_RET_NO_ACK;
}

static uint8_t sw_i2c_send_addr(sw_i2c_desc_t *dev, uint16_t addr, uint8_t restart)
{
    /* addr 10bit = 0x1111 00(高两位) + 读写控制位 & 低 8 bit */
    #define I2C_ADDR_10_MSK(addr)      ( ( ( ( (0b111100) << 8 ) | addr ) >> 8 ) & 0xFF )

    if (dev->addr_10b) {
        uint8_t res = SW_I2C_RET_OK;
        res = sw_i2c_send_addr_bytes(dev, I2C_ADDR_10_MSK(addr));
        /* 10bit 地址第二次读不需要发低 8bit */
        if (0 != restart) {
            if (SW_I2C_RET_OK != res) {
                return res;
            }
            res = sw_i2c_send_addr_bytes(dev, addr & 0xFF);
        }
        return res;
    }
    /* 7bit 地址 */
    return sw_i2c_send_addr_bytes(dev, addr & 0xFF);
}