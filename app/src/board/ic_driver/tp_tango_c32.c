/**
 *******************************************************************************************************************************************
 * @file        tp_tango_c32.c
 * @brief       电容触摸驱动: Tango C32(瀚瑞微 PIXCIR)
 * @since       Change Logs:
 * Date         Author       Notes
 * 2023-01-11   ysr          the first version
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

/** 如果需要打印触摸调试信息请定义宏 TANGO_DEBUG */
#define TANGO_DEBUG
#ifdef TANGO_DEBUG
#include <stdio.h>
# define TANGO_LOG(...)      printf(__VA_ARGS__)
#else
# define TANGO_LOG(...)      
#endif //TANGO_DEBUG

/* 按 8 位地址的方式定义(7 bit 器件地址 + 1 bit 读写控制位) */
#define TANGO_C32_IIC_ADDR      (0xB8) //0x5C

/* Tango C32 部分寄存器定义 */
#define TANGO_REG_TP1      (0x00) /**< 触摸数据读取地址 */
#define TANGO_REG_INT_MODE (0x34) /**< 中断模式寄存器 */
#define TANGO_REG_PID      (0x40) /**< 版本寄存器 */

static const uint8_t Tango_C32_TP_Reg[] = {TANGO_REG_TP1};
static const uint16_t Tango_C32_Max_Points = sizeof(Tango_C32_TP_Reg) / sizeof(Tango_C32_TP_Reg[0]);
static uint16_t Tango_C32_I2C_Addr = TANGO_C32_IIC_ADDR; /* default */

#include "dev_tp_private.c"

#define __TANGO_C32_TP_WR_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_WR_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)
#define __TANGO_C32_TP_RD_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_RD_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)

#define _TANGO_C32_TP_WR_REG(reg, buf, len, tp_desc)      __TANGO_C32_TP_WR_REG(reg, buf, len, tp_desc, Tango_C32_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)
#define _TANGO_C32_TP_RD_REG(reg, buf, len, tp_desc)      __TANGO_C32_TP_RD_REG(reg, buf, len, tp_desc, Tango_C32_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)

/* const tp_desc_t *self is caller function parameters */
#define TANGO_C32_TP_WR_REG(reg, buf, len)      _TANGO_C32_TP_WR_REG(reg, buf, len, self)
#define TANGO_C32_TP_RD_REG(reg, buf, len)      _TANGO_C32_TP_RD_REG(reg, buf, len, self)

uint8_t tango_c32_init(tp_desc_t *self)
{
    if (!(self && self->board.hw_rst_gpio_set && self->board.hw_int_gpio_set_input && self->board.mdelay)) {
        TANGO_LOG("[%s] parm error: [hw_rst_gpio_set / hw_int_gpio_set_input / mdelay] is invaild!\r\n", __FUNCTION__);
        return 1;
    }
    /* 检查本驱动适配列表 */
    if (0 == strcmp("Tango_C32", self->cfg.name)) {
        Tango_C32_I2C_Addr = TANGO_C32_IIC_ADDR;
    } else {
        if (!strstr(self->cfg.name, "Tango")) TANGO_LOG("[%s] Not belonging to [%s] driver!\r\n", self->cfg.name, "Tango");
        TANGO_LOG("[%s] unadapted the driver!\r\n", self->cfg.name);
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
    /* 选配 INT 中断触发 */
    if (TP_MODE_INT == self->cfg.work_mode) {
        if (self->board.hw_int_gpio_interrupt_init) {
            self->board.hw_int_gpio_interrupt_init(0); //下降沿触发
        } else {
            TANGO_LOG("error: [board.hw_int_gpio_interrupt_init] is invaild!\r\n");
        }
    }
    return 0;
}

uint8_t tango_c32_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points)
{
    if (points > Tango_C32_Max_Points) {
        points = Tango_C32_Max_Points; /* 以本驱动支持的最大点数为上限 */
    }

    uint8_t touch_sta = 0, touch_nums = 0, i = 0;
    uint8_t buf[32] = {0};
    /* 读取 X/Y 坐标值、触摸状态信息 */
    if (0 != TANGO_C32_TP_RD_REG(Tango_C32_TP_Reg[0], buf, 32)) {
        goto read_points_fail;
    }
    touch_sta = buf[0];
    //uint8_t button = buf[1];
    touch_nums = touch_sta;
    if (touch_nums > points) {
        touch_nums = points;
    }
    if (0 == touch_nums) {
        goto read_points_fail;
    }
    for (i = 0; i < touch_nums; ++i) {
        //buf[6  11  16  21  26] /* 手指 ID */
        tp_data[i].x = (((uint16_t)buf[3 + 5 * i] << 8) | buf[2 + 5 * i]);
        tp_data[i].y = (((uint16_t)buf[5 + 5 * i] << 8) | buf[4 + 5 * i]);
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
        TANGO_LOG("tp_pos: [%d, %d] \n", tp_data[i].x, tp_data[i].y);
        /* 有效性判断 */
        if (tp_data[i].x <= self->cfg.pos.max_x && tp_data[i].y <= self->cfg.pos.max_y) {
            tp_data[i].sta = TP_STA_PRESSED;
        } else {
            tp_data[i].x = 0xFFFF;
            tp_data[i].y = 0xFFFF;
            tp_data[i].sta = TP_STA_RELEASED;
        }
    }
    return touch_nums;

read_points_fail:
    for (i = 0; i < points; ++i) {
        tp_data[i].x = 0xFFFF;
        tp_data[i].y = 0xFFFF;
        tp_data[i].sta = TP_STA_RELEASED;
    }
//    TANGO_LOG("[%s]: fail!\r\n", __FUNCTION__);
    return 0;
}
