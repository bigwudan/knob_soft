/**
 *******************************************************************************************************************************************
 * @file        tp_chsc6413.c
 * @brief       电容触摸驱动: CHSC6413
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-05-09   tw           the first version
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

/** 如果需要打印触摸调试信息请定义宏 CHSCXX_DEBUG */
#define CHSCXX_DEBUG
#ifdef CHSCXX_DEBUG
#include <stdio.h>
# define CHSCXX_LOG(...)        printf(__VA_ARGS__)
#else
# define CHSCXX_LOG(...)
#endif //CHSCXX_DEBUG

/* Note: I2C 地址宏此处以 8/7 bit + 1bit(rw) 方式定义 */
#define CHSC6413_I2C_ADDR 	(0x5C)

/* CHSC6413 部分寄存器定义 */
#define CHSCXX_REG_TP1      (0xE0) /**< 获取触摸数据 */

static const uint8_t CHSCXX_TP_Reg[] = {CHSCXX_REG_TP1};
static const uint16_t CHSCXX_Max_Points = sizeof(CHSCXX_TP_Reg) / sizeof(CHSCXX_TP_Reg[0]);
static uint16_t CHSCXX_I2C_Addr = CHSC6413_I2C_ADDR; /* default */

#include "dev_tp_private.c"

#define __CHSCXX_TP_WR_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_WR_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)
#define __CHSCXX_TP_RD_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_RD_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)

#define _CHSCXX_TP_WR_REG(reg, buf, len, tp_desc)      __CHSCXX_TP_WR_REG(reg, buf, len, tp_desc, CHSCXX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)
#define _CHSCXX_TP_RD_REG(reg, buf, len, tp_desc)      __CHSCXX_TP_RD_REG(reg, buf, len, tp_desc, CHSCXX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)

/* const tp_desc_t *self is caller function parameters */
#define CHSCXX_TP_WR_REG(reg, buf, len)      _CHSCXX_TP_WR_REG(reg, buf, len, self)
#define CHSCXX_TP_RD_REG(reg, buf, len)      _CHSCXX_TP_RD_REG(reg, buf, len, self)

uint8_t chsc6413_init(tp_desc_t *self)
{
    if (!(self && self->board.hw_rst_gpio_set && self->board.hw_int_gpio_set_input && self->board.mdelay)) {
        CHSCXX_LOG("[%s] parm error: [hw_rst_gpio_set / hw_int_gpio_set_input / mdelay] is invaild!\r\n", __FUNCTION__);
        return 1;
    }
    /* 检查本驱动适配列表 */
    if (0 == strcmp("CHSC6413", self->cfg.name)) {
        CHSCXX_I2C_Addr = CHSC6413_I2C_ADDR;
    } else {
        if (!strstr(self->cfg.name, "CHSC")) CHSCXX_LOG("[%s] Not belonging to [%s] driver!\r\n", self->cfg.name, "CHSC");
        CHSCXX_LOG("[%s] unadapted the driver!\r\n", self->cfg.name);
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
            CHSCXX_LOG("error: [board.hw_int_gpio_interrupt_init] is invaild!\r\n");
        }
    }
    return 0;
}

uint8_t chsc6413_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points)
{
    if (points > CHSCXX_Max_Points) {
        points = CHSCXX_Max_Points; /* 以本驱动支持的最大点数为上限 */
    }

    uint8_t touch_nums = 0, i = 0;
    uint8_t buf[3] = {0};// ? 读取坐标的方式是，先去往寄存器地址0x5C写入E0，然后再去读取0X5D的地址，连续读取8个数据长度
    if (0 != CHSCXX_TP_RD_REG(CHSCXX_TP_Reg[0], buf, 3)) {
        goto read_points_fail;
    }
    /* 目前仅支持单点读取 */
    touch_nums = points;
    if (touch_nums > points) {
        touch_nums = points;
    }
    if (0 == touch_nums) {
        goto read_points_fail;
    }

    tp_data[i].x = buf[1];
    if (buf[0] >> 8) {
        tp_data[i].x += 0xFF;
    }
    tp_data[i].y = buf[2];
    if (buf[0] >> 7) {
        tp_data[i].x += 0xFF;
    }

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
    CHSCXX_LOG("tp_pos: [%d, %d] \n", tp_data[i].x, tp_data[i].y);
    /* 有效性判断 */
    if (tp_data[i].x <= self->cfg.pos.max_x && tp_data[i].y <= self->cfg.pos.max_y) {
        tp_data[i].sta = TP_STA_PRESSED;
    } else {
        tp_data[i].x = 0xFFFF;
        tp_data[i].y = 0xFFFF;
        tp_data[i].sta = TP_STA_RELEASED;
    }
    return touch_nums;

read_points_fail:
    for (i = 0; i < points; ++i) {
        tp_data[i].x = 0xFFFF;
        tp_data[i].y = 0xFFFF;
        tp_data[i].sta = TP_STA_RELEASED;
    }
//    CHSCXX_LOG("[%s]: fail!\r\n", __FUNCTION__);
    return 0;
}
