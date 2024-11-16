/**
 *******************************************************************************************************************************************
 * @file        tp_ili2117a.c
 * @brief       电容触摸驱动: ILI2117A(奕力)
 * @since       Change Logs:
 * Date         Author       Notes
 * 2023-03-22   wt          the first version
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

/** 如果需要打印触摸调试信息请定义宏 ILIXX_DEBUG */
#define ILIXX_DEBUG
#ifdef ILIXX_DEBUG
#include <stdio.h>
# define ILIXX_LOG(...)        printf(__VA_ARGS__)
#else
# define ILIXX_LOG(...)
#endif //ILIXX_DEBUG

/* Note: I2C 地址宏此处以 8/7 bit + 1bit(rw) 方式定义 */
#define ILI2117A_I2C_ADDR 	(0x4C)

/* ILI2117A 部分寄存器定义 */
#define ILIXX_REG_TP1      (0x10) /**< 获取触摸数据 */
#define ILIXX_MSK_W	       (0x00) /**< 写 */
#define ILIXX_MSK_R 	   (0x01) /**< 读 */

static const uint8_t ILIXX_TP_Reg[] = {ILIXX_REG_TP1};
static const uint16_t ILIXX_Max_Points = sizeof(ILIXX_TP_Reg) / sizeof(ILIXX_TP_Reg[0]);
static uint16_t ILIXX_I2C_Addr = ILI2117A_I2C_ADDR; /* default */

#include "dev_tp_private.c"

#define __ILIXX_TP_WR_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_WR_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)
#define __ILIXX_TP_RD_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_RD_REG_8(reg, buf, len, tp_desc, i2c_addr, timeout)

#define _ILIXX_TP_WR_REG(reg, buf, len, tp_desc)      __ILIXX_TP_WR_REG(reg, buf, len, tp_desc, ILIXX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)
#define _ILIXX_TP_RD_REG(reg, buf, len, tp_desc)      __ILIXX_TP_RD_REG(reg, buf, len, tp_desc, ILIXX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)

/* const tp_desc_t *self is caller function parameters */
#define ILIXX_TP_WR_REG(reg, buf, len)      _ILIXX_TP_WR_REG(reg, buf, len, self)
#define ILIXX_TP_RD_REG(reg, buf, len)      _ILIXX_TP_RD_REG(reg, buf, len, self)

uint8_t ili2117a_init(tp_desc_t *self)
{
    if (!(self && self->board.hw_rst_gpio_set && self->board.hw_int_gpio_set_input && self->board.mdelay)) {
        ILIXX_LOG("[%s] parm error: [hw_rst_gpio_set / hw_int_gpio_set_input / mdelay] is invaild!\r\n", __FUNCTION__);
        return 1;
    }
    /* 检查本驱动适配列表 */
    if (0 == strcmp("ILI2117A", self->cfg.name)) {
        ILIXX_I2C_Addr = ILI2117A_I2C_ADDR;
    } else {
        if (!strstr(self->cfg.name, "ILI")) ILIXX_LOG("[%s] Not belonging to [%s] driver!\r\n", self->cfg.name, "ILIxx");
        ILIXX_LOG("[%s] unadapted the driver!\r\n", self->cfg.name);
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
            ILIXX_LOG("error: [board.hw_int_gpio_interrupt_init] is invaild!\r\n");
        }
    }
    return 0;
}

uint8_t ili2117a_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points)
{
    if (points > ILIXX_Max_Points) {
        points = ILIXX_Max_Points; /* 以本驱动支持的最大点数为上限 */
    }

    uint8_t touch_nums = 0, i = 0;
    uint8_t buf[32] = {0};
    if (0 != ILIXX_TP_RD_REG(ILIXX_TP_Reg[0], buf, 31)) {
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
	tp_data[i].x = ((((uint16_t)buf[3] & 0xF0)<<4) + buf[4]) * self->cfg.pos.max_x / 2048;
	tp_data[i].y = ((((uint16_t)buf[3] & 0x0F)<<8) + buf[5]) * self->cfg.pos.max_y / 2048;
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
    ILIXX_LOG("tp_pos: [%d, %d] \n", tp_data[i].x, tp_data[i].y);
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
//    ILIXX_LOG("[%s]: fail!\r\n", __FUNCTION__);
    return 0;
}
