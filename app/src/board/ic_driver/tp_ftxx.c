/**
 *******************************************************************************************************************************************
 * @file        tp_ftxxx.c
 * @brief       电容触摸驱动: FTxx(敦泰电子)
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
#include "dev_tp.h"

/** 如果需要打印触摸调试信息请定义宏 FTXX_DEBUG */
#define FTXX_DEBUG
#ifdef FTXX_DEBUG
#include <stdio.h>
# define FTXX_LOG(...)      printf(__VA_ARGS__)
#else
# define FTXX_LOG(...)      
#endif //FTXX_DEBUG

/* 按 8 位地址的方式定义(7 bit 器件地址 + 1 bit 读写控制位) */
#define FT6336_I2C_ADDR        ( 0x90 ) /**< ft6336 设备地址 */
#define FT5426_I2C_ADDR        ( 0x38 ) /**< ft5426 / ft5206 设备地址 */

/* FTXX 部分寄存器定义 */
#define FTXX_REG_STAT                 (0x02)
#define FTXX_MSK_MODE_BUFFER_INVALID  (0x08)
#define FTXX_MSK_STAT_NUMBER_TOUCH    (0x07)

#define FTXX_REG_TP1          (0x03)
#define FTXX_REG_TP2          (0x09)
#define FTXX_REG_TP3          (0x0F)
#define FTXX_REG_TP4          (0x15)

static const uint8_t FTXX_TP_Reg[] = {FTXX_REG_TP1, FTXX_REG_TP2, FTXX_REG_TP3, FTXX_REG_TP4};
static const uint16_t FTXX_Max_Points = sizeof(FTXX_TP_Reg) / sizeof(FTXX_TP_Reg[0]);
static uint16_t FTXX_I2C_Addr = FT6336_I2C_ADDR; /* default */

#include "dev_tp_private.c"

#define __FTXX_TP_WR_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_WR_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)
#define __FTXX_TP_RD_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_RD_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)

#define _FTXX_TP_WR_REG(reg, buf, len, tp_desc)      __FTXX_TP_WR_REG(reg, buf, len, tp_desc, FTXX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)
#define _FTXX_TP_RD_REG(reg, buf, len, tp_desc)      __FTXX_TP_RD_REG(reg, buf, len, tp_desc, FTXX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)

/* const tp_desc_t *self is caller function parameters */
#define FTXX_TP_WR_REG(reg, buf, len)      _FTXX_TP_WR_REG(reg, buf, len, self)
#define FTXX_TP_RD_REG(reg, buf, len)      _FTXX_TP_RD_REG(reg, buf, len, self)

uint8_t ftxx_init(tp_desc_t *self)
{
    if (!(self && self->board.hw_rst_gpio_set && self->board.hw_int_gpio_set_input && self->board.mdelay)) {
        FTXX_LOG("[%s] parm error: [hw_rst_gpio_set / hw_int_gpio_set_input / mdelay] is invaild!\r\n", __FUNCTION__);
        return 1;
    }
    /* 检查本驱动适配列表 */
    if (0 == strcmp("FT6336", self->cfg.name)) {
        FTXX_I2C_Addr = FT6336_I2C_ADDR;
    } else if (0 == strcmp("FT5426", self->cfg.name) || 0 == strcmp("FT5206", self->cfg.name)) {
        FTXX_I2C_Addr = FT5426_I2C_ADDR;
    } else {
        if (!strstr(self->cfg.name, "FT")) FTXX_LOG("[%s] Not belonging to [%s] driver!\r\n", self->cfg.name, "FTxx");
        FTXX_LOG("[%s] unadapted the driver!\r\n", self->cfg.name);
    }
    uint8_t res = 0;
    res = self->board.i2c_ops.init(400 * 1000);
    if (0 != res) {
        return res;
    }
    /* INT 设为输入并开启下拉. 复位时 INT 为低, 选择 I2C 器件从地址 */
    self->board.hw_int_gpio_set_input(0);
    /* HW-RST */
    self->board.hw_rst_gpio_set(0); //L
    self->board.mdelay(10);
    self->board.hw_rst_gpio_set(1); //H
    self->board.mdelay(120);

    /* check ID */
    uint8_t temp = 0;
    for (uint8_t i = 0; i < 20; ++i) {
        res = FTXX_TP_RD_REG(0xA3, &temp, 1);
        if (0 != res) {
            return res;
        }
        if (temp == 0x06 || temp == 0x36 || temp == 0x64) {
            break;
        }
    }
    FTXX_LOG("[%s] product id: 0x[%x]\r\n", self->cfg.name, temp);
    /* 选配 INT 中断触发 */
    if (TP_MODE_INT == self->cfg.work_mode) {
        if (self->board.hw_int_gpio_interrupt_init) {
            self->board.hw_int_gpio_interrupt_init(0); //下降沿触发
        } else {
            FTXX_LOG("error: [board.hw_int_gpio_interrupt_init] is invaild!\r\n");
        }
    }
    return 0;
}

uint8_t ftxx_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points)
{
    if (points > FTXX_Max_Points) {
        points = FTXX_Max_Points; /* 以本驱动支持的最大点数为上限 */
    }

    uint8_t touch_sta = 0, touch_nums = 0, i = 0;
    uint8_t buf[4] = {0};
    do { /* make sure data in buffer is valid */
        if (0 != FTXX_TP_RD_REG(FTXX_REG_STAT, &touch_sta, 1)) {
            goto read_points_fail;
        }
        if (i++ == 0x30) {
            FTXX_LOG("[%s]: timeout[%d]\r\n", __FUNCTION__, i);
            goto read_points_fail;
        }
    } while (touch_sta & FTXX_MSK_MODE_BUFFER_INVALID);
    
    touch_nums = touch_sta & FTXX_MSK_STAT_NUMBER_TOUCH;
    FTXX_LOG("touch_sta: 0x[%x], touch_nums: 0x[%x] \r\n", touch_sta, touch_nums);
    if (touch_nums > points) {
        touch_nums = points;
    }
    if (0 == touch_nums) {
        goto read_points_fail;
    }
    for (i = 0; i < touch_nums; ++i) {
        if (0 != FTXX_TP_RD_REG(FTXX_TP_Reg[i], buf, sizeof(buf) / sizeof(buf[0]))) {
            continue;
        }
        tp_data[i].x = (((uint16_t)(buf[0] & 0x0F)) << 8) | buf[1];
        tp_data[i].y = (((uint16_t)(buf[2] & 0x0F)) << 8) | buf[3];
        /* 坐标对调 & 偏移 & 镜像 */
        if (self->cfg.pos.swap_xy) {
            tp_data[i].x = tp_data[i].x ^ tp_data[i].y;
            tp_data[i].y = tp_data[i].x ^ tp_data[i].y;
            tp_data[i].x = tp_data[i].x ^ tp_data[i].y;
        }
        tp_data[i].x += self->cfg.pos.offset_x;
        tp_data[i].y += self->cfg.pos.offset_y;
        if (self->cfg.pos.mirror_x) {
            tp_data[i].x = self->cfg.pos.max_x - tp_data[i].x;
        }
        if (self->cfg.pos.mirror_y) {
            tp_data[i].y = self->cfg.pos.max_y - tp_data[i].y;
        }
        FTXX_LOG("tp_pos: [%d, %d] \n", tp_data[i].x, tp_data[i].y);
        /* 有效性判断 */
        if (tp_data[i].x <= self->cfg.pos.max_x && tp_data[i].y <= self->cfg.pos.max_y) {
            tp_data[i].sta = TP_STA_PRESSED;
        } else {
            tp_data[i].x = 0xFFFF;
            tp_data[i].y = 0xFFFF;
            tp_data[i].sta = TP_STA_RELEASED;
        }
        /* 仅解析了第一个点的状态 */
        if (FTXX_TP_Reg[i] == FTXX_REG_TP1) {
            if (0x01 == ((buf[0] >> 6) & 0x03)) {
                tp_data[i].sta = TP_STA_RELEASED;
            }
        }
    }
    return touch_nums;

read_points_fail:
    for (i = 0; i < points; ++i) {
        tp_data[i].x = 0xFFFF;
        tp_data[i].y = 0xFFFF;
        tp_data[i].sta = TP_STA_RELEASED;
    }
//    FTXX_LOG("[%s]: fail!\r\n", __FUNCTION__);
    return 0;
}
