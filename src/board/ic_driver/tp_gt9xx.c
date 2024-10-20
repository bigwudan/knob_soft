/**
 *******************************************************************************************************************************************
 * @file        tp_gt9xx.c
 * @brief       电容触摸驱动: GT9xx系列(汇顶科技)
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

/** 如果需要打印触摸调试信息请定义宏 GT9XX_DEBUG */
#define GT9XX_DEBUG
#ifdef GT9XX_DEBUG
#include <stdio.h>
# define GT9XX_LOG(...)        printf(__VA_ARGS__)
#else
# define GT9XX_LOG(...)
#endif //GT9XX_DEBUG

/* GT9xx Register and Parameter */
/* Note: I2C 地址宏此处以 8bit + 1bit(rw) 方式定义 */
#define GT911_I2C_ADDR       (0x5D << 1) /**< gt911设备地址(0xBA) */

#define GT9XX_REG_CTRL        (0x8040) /**< gt9xx控制寄存器 */
#define GT9XX_REG_CFGS        (0x8047) /**< gt9xx配置寄存器 */
#define GT9XX_REG_CHECK       (0x80FF) /**< gt9xx校验和寄存器 */
#define GT9XX_REG_PID         (0x8140) /**< gt9xx产品ID寄存器 */
#define GT9XX_REG_GSTID       (0x814E) /**< 当前检测到的触摸情况 */
#define GT9XX_REG_TP1         (0x8150) /**< 第一个触摸点数据地址 */
#define GT9XX_REG_TP2         (0x8158) /**< 第二个触摸点数据地址 */
#define GT9XX_REG_TP3         (0x8160) /**< 第三个触摸点数据地址 */
#define GT9XX_REG_TP4         (0x8168) /**< 第四个触摸点数据地址 */
#define GT9XX_REG_TP5         (0x8170) /**< 第五个触摸点数据地址 */

/** 如果需要更新触摸配置请定义宏 GT9XX_UPDATE_CFG,
 * warning: 如无特殊需求, 请不要打开该宏!!!打开该宏前联系你的触摸模组供应商以确保你的行为是否正确.
 */
// #define GT9XX_UPDATE_CFG

static const uint16_t GT9XX_TP_Reg[] = {GT9XX_REG_TP1, GT9XX_REG_TP2, GT9XX_REG_TP3, GT9XX_REG_TP4, GT9XX_REG_TP5};
static const uint16_t GT9XX_Max_Points = sizeof(GT9XX_TP_Reg) / sizeof(GT9XX_TP_Reg[0]);
static uint16_t GT9XX_I2C_Addr = GT911_I2C_ADDR; /* default */

#include "dev_tp_private.c"

#define __GT9XX_TP_WR_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_WR_REG_16(reg, buf, len, tp_desc, i2c_addr, timeout)
#define __GT9XX_TP_RD_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_RD_REG_16(reg, buf, len, tp_desc, i2c_addr, timeout)

#define _GT9XX_TP_WR_REG(reg, buf, len, tp_desc)      __GT9XX_TP_WR_REG(reg, buf, len, tp_desc, GT9XX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)
#define _GT9XX_TP_RD_REG(reg, buf, len, tp_desc)      __GT9XX_TP_RD_REG(reg, buf, len, tp_desc, GT9XX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)

/* const tp_desc_t *self is caller function parameters */
#define GT9XX_TP_WR_REG(reg, buf, len)      _GT9XX_TP_WR_REG(reg, buf, len, self)
#define GT9XX_TP_RD_REG(reg, buf, len)      _GT9XX_TP_RD_REG(reg, buf, len, self)

uint8_t gt9xx_init(tp_desc_t *self)
{
    if (!(self && self->board.hw_rst_gpio_set && self->board.hw_int_gpio_set_input && self->board.mdelay)) {
        GT9XX_LOG("[%s] parm error: [hw_rst_gpio_set / hw_int_gpio_set_input / mdelay] is invaild!\r\n", __FUNCTION__);
        return 1;
    }
    /* 检查本驱动适配列表 */
    if (0 == strcmp("GT911", self->cfg.name) || 0 == strcmp("GT9147", self->cfg.name)) {
        GT9XX_I2C_Addr = GT911_I2C_ADDR;
    } else {
        if (!strstr(self->cfg.name, "GT9")) GT9XX_LOG("[%s] Not belonging to [%s] driver!\r\n", self->cfg.name, "GT9xx");
        GT9XX_LOG("[%s] unadapted the driver!\r\n", self->cfg.name);
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
    
    uint8_t temp[5] = {0};
    /* check ID */
    res = GT9XX_TP_RD_REG(GT9XX_REG_PID, temp, 4);
    if (0 != res) {
        return res;
    }
    GT9XX_LOG("[%s] product id: [%s]\r\n", self->cfg.name, temp);
    
    //清除 Ready 标志
    temp[0] = 0;
    if (0 != GT9XX_TP_WR_REG(GT9XX_REG_GSTID, temp, 1)) {
        GT9XX_LOG("[%s] [GT9XX_REG_GSTID] clear fail\r\n", self->cfg.name);
    }
#ifdef GT9XX_UPDATE_CFG
    static uint8_t gt9xx_update_fw_cfg(const tp_desc_t *self);
    gt9xx_update_fw_cfg(self); /* warning!!!更新触摸芯片固件配置可能导致TP变砖 */
#endif
    /* 选配 INT 中断触发 */
    if (TP_MODE_INT == self->cfg.work_mode) {
        if (self->board.hw_int_gpio_interrupt_init) {
            self->board.hw_int_gpio_interrupt_init(0); //下降沿触发
        } else {
            GT9XX_LOG("error: [board.hw_int_gpio_interrupt_init] is invaild!\r\n");
        }
    }
    return 0;
}

uint8_t gt9xx_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points)
{
    if (points > GT9XX_Max_Points) {
        points = GT9XX_Max_Points; /* 以本驱动支持的最大点数为上限 */
    }

    uint8_t touch_sta = 0, touch_nums = 0, i = 0;
    uint8_t buf[5] = {0};
    if (0 != GT9XX_TP_RD_REG(GT9XX_REG_GSTID, &touch_sta, 1)) {
        goto read_points_fail;
    }
    touch_nums = touch_sta & 0x0F;
    if (0 != (touch_sta & 0x80)) {
        uint8_t temp = 0;
        //清除 Ready 标志
        if (0 != GT9XX_TP_WR_REG(GT9XX_REG_GSTID, &temp, 1)) {
            goto read_points_fail;
        }
    }
    if (touch_nums > points) {
        touch_nums = points;
    }
    if (0 == touch_nums) {
        goto read_points_fail;
    }
    for (i = 0; i < touch_nums; ++i) {
        if (0 != GT9XX_TP_RD_REG(GT9XX_TP_Reg[i], buf, 4)) {
            continue;
        }
        tp_data[i].x = (((uint16_t)buf[1] << 8) + buf[0]);
        tp_data[i].y = (((uint16_t)buf[3] << 8) + buf[2]);
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
        GT9XX_LOG("tp_pos: [%d, %d] \n", tp_data[i].x, tp_data[i].y);
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
//    GT9XX_LOG("[%s]: fail!\r\n", __FUNCTION__);
    return 0;
}

#ifdef GT9XX_UPDATE_CFG
/** 配置文件的版本号(新下发的配置版本号大于原版本, 或等于原版本号但配置内容有变化时保存,
 * 版本号版本正常范围：'A'~'Z', 发送 0x00 则将版本号初始化为'A') 
 */
static uint8_t GT9XX_FW_Config_Table[] = {
    0x00,       /**< config_version */
    0xE0, 0x01, /**< x output max */
    0x10, 0x01, /**< y output max */
    0x05,       /**< touch number */
    0x3D, 0x00, /**< module switch */
    0x02, 0x08, 0x1E, 0x08, 0x50, 0x3C, 0x0F, 0x05,
    0x00, 0x00, 0xFF, 0x67, 0x50, 0x00, 0x00, 0x18,
    0x1A, 0x1E, 0x14, 0x89, 0x28, 0x0A, 0x30, 0x2E,
    0xBB, 0x0A, 0x03, 0x00, 0x00, 0x02, 0x33, 0x1D,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32,
    0x00, 0x00, 0x2A, 0x1C, 0x5A, 0x94, 0xC5, 0x02,
    0x07, 0x00, 0x00, 0x00, 0xB5, 0x1F, 0x00, 0x90,
    0x28, 0x00, 0x77, 0x32, 0x00, 0x62, 0x3F, 0x00,
    0x52, 0x50, 0x00, 0x52, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0F, 0x0F, 0x03, 0x06, 0x10,
    0x42, 0xF8, 0x0F, 0x14, 0x00, 0x00, 0x00, 0x00,
    0x1A, 0x18, 0x16, 0x14, 0x12, 0x10, 0x0E, 0x0C,
    0x0A, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x28,
    0x24, 0x22, 0x20, 0x1F, 0x1E, 0x1D, 0x0E, 0x0C,
    0x0A, 0x08, 0x06, 0x05, 0x04, 0x02, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/**
 * @brief  更新触摸配置固件
 * @retval 0     : success
 * @retval other : error code
 */
static uint8_t gt9xx_update_fw_cfg(const tp_desc_t *self)
{
    uint8_t buf[2] = {0};
    /* set x range */
    GT9XX_FW_Config_Table[2] = (uint8_t)(HOR_RES >> 8);
    GT9XX_FW_Config_Table[1] = (uint8_t)(HOR_RES & 0xff);

    /* set y range */
    GT9XX_FW_Config_Table[4] = (uint8_t)(VER_RES >> 8);
    GT9XX_FW_Config_Table[3] = (uint8_t)(VER_RES & 0xff);

    /* change x y */
    // GT9XX_FW_Config_Table[6] ^= (1 << 3);

    /* change int trig type */
    /* FLAG_INT_FALL_RX */
    GT9XX_FW_Config_Table[6] &= 0xFC;
    GT9XX_FW_Config_Table[6] |= 0x01;
    /* FLAG_RDONLY */
    // GT9XX_FW_Config_Table[6] &= 0xFC;
    // GT9XX_FW_Config_Table[6] |= 0x03;

    /* send config */
    if (0 != GT9XX_TP_WR_REG(GT9XX_REG_CFGS, GT9XX_FW_Config_Table, sizeof(GT9XX_FW_Config_Table))) {
        GT9XX_LOG("send config failed\n");
        return 1;
    }
    /* check config */
    buf[0] = 0;
    for (uint32_t i = 0; i < sizeof(GT9XX_FW_Config_Table); ++i) {
        buf[0] += GT9XX_FW_Config_Table[i];
    }
    buf[0] = (~buf[0]) + 1;
    buf[1] = 1;
    if (0 != GT9XX_TP_WR_REG(GT9XX_REG_CHECK, buf, 2)) {
        GT9XX_LOG("send config check failed\n");
        return 2;
    }
    for (uint32_t i = 0; i < 1 * 1000 * 1000; ++i) __NOP();
    return 0;
}
#endif
