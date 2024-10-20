/**
 *******************************************************************************************************************************************
 * @file        tp_cst3xx.c
 * @brief       电容触摸驱动: CST3xx系列(海栎创)
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

/** 如果需要打印触摸调试信息请定义宏 CST3XX_DEBUG */
#define CST3XX_DEBUG
#ifdef CST3XX_DEBUG
#include <stdio.h>
# define CST3XX_LOG(...)    printf(__VA_ARGS__)
#else
# define CST3XX_LOG(...)    
#endif // CST3XX_DEBUG

/* 按 8 位地址的方式定义(7 bit 器件地址 + 1 bit 读写控制位) */
#define CST328_I2C_ADDR         ( 0x1A << 1 ) /* default */
#define CST3240_I2C_ADDR        ( 0x5A << 1 )

/* CST3xx 触摸点信息寄存器 */
#define CST3XX_REG_TP1                 (0xD000)

#if 1 /* register address */
/********selfcap register address start *****************/
#define HYN_REG_CAP_POWER_MODE                  0xA5
#define HYN_REG_CAP_POWER_MODE_SLEEP_VALUE      0x03
#define HYN_REG_CAP_FW_VER                      0xA6
#define HYN_REG_CAP_VENDOR_ID                   0xA8
#define HYN_REG_CAP_PROJECT_ID                  0xA9
#define HYN_REG_CAP_CHIP_ID                     0xAA
#define HYN_REG_CAP_CHIP_CHECKSUM               0xAC

#define HYN_REG_CAP_GESTURE_EN                  0xD0
#define HYN_REG_CAP_GESTURE_OUTPUT_ADDRESS      0xD3

#define HYN_REG_CAP_PROXIMITY_EN                0xB0
#define HYN_REG_CAP_PROXIMITY_OUTPUT_ADDRESS    0x01

#define HYN_REG_CAP_ESD_SATURATE                0xE0
/********selfcap register address end *****************/

/********mutcap register address start *****************/
//Myabe change
#define HYN_REG_MUT_ESD_VALUE                   0xD040
#define HYN_REG_MUT_ESD_CHECKSUM                0xD046
#define HYN_REG_MUT_PROXIMITY_EN                0xD04B
#define HYN_REG_MUT_PROXIMITY_OUTPUT_ADDRESS    0xD04B
#define HYN_REG_MUT_GESTURE_EN                  0xD04C
#define HYN_REG_MUT_GESTURE_OUTPUT_ADDRESS      0xD04C

//workmode
#define HYN_REG_MUT_DEBUG_INFO_MODE             0xD101
#define HYN_REG_MUT_RESET_MODE                  0xD102
#define HYN_REG_MUT_DEBUG_RECALIBRATION_MODE    0xD104
#define HYN_REG_MUT_DEEP_SLEEP_MODE             0xD105
#define HYN_REG_MUT_DEBUG_POINT_MODE            0xD108
#define HYN_REG_MUT_NORMAL_MODE                 0xD109

#define HYN_REG_MUT_DEBUG_RAWDATA_MODE          0xD10A
#define HYN_REG_MUT_DEBUG_DIFF_MODE             0xD10D
#define HYN_REG_MUT_DEBUG_FACTORY_MODE          0xD119
#define HYN_REG_MUT_DEBUG_FACTORY_MODE_2        0xD120

//debug info
/****************HYN_REG_MUT_DEBUG_INFO_MODE address start***********/
#define HYN_REG_MUT_DEBUG_INFO_IC_CHECKSUM             0xD208
#define HYN_REG_MUT_DEBUG_INFO_FW_VERSION              0xD204
#define HYN_REG_MUT_DEBUG_INFO_IC_TYPE                 0xD202
#define HYN_REG_MUT_DEBUG_INFO_PROJECT_ID              0xD200
#define HYN_REG_MUT_DEBUG_INFO_BOOT_TIME               0xD1FC
#define HYN_REG_MUT_DEBUG_INFO_RES_Y                   0xD1FA
#define HYN_REG_MUT_DEBUG_INFO_RES_X                   0xD1F8
#define HYN_REG_MUT_DEBUG_INFO_KEY_NUM                 0xD1F7
#define HYN_REG_MUT_DEBUG_INFO_TP_NRX                  0xD1F6
#define HYN_REG_MUT_DEBUG_INFO_TP_NTX                  0xD1F4

/****************HYN_REG_MUT_DEBUG_INFO_MODE address end***********/
#define HYN_WORK_MODE_NORMAL     0
#define HYN_WORK_MODE_FACTORY    1
#define HYN_WORK_MODE_RAWDATA    2
#define HYN_WORK_MODE_DIFF       3
#define HYN_WORK_MODE_UPDATE     4
/********mutcap register address end *****************/
#endif

static const uint16_t CST3XX_TP_Reg[] = {CST3XX_REG_TP1};
static const uint16_t CST3XX_Max_Points = sizeof(CST3XX_TP_Reg) / sizeof(CST3XX_TP_Reg[0]);
static uint16_t CST3XX_I2C_Addr = CST328_I2C_ADDR; /* default */

#include "dev_tp_private.c"

#define __CST3XX_TP_WR_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_WR_REG_16(reg, buf, len, tp_desc, i2c_addr, timeout)
#define __CST3XX_TP_RD_REG(reg, buf, len, tp_desc, i2c_addr, timeout)      TP_PRIVATE_RD_REG_16(reg, buf, len, tp_desc, i2c_addr, timeout)

#define _CST3XX_TP_WR_REG(reg, buf, len, tp_desc)      __CST3XX_TP_WR_REG(reg, buf, len, tp_desc, CST3XX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)
#define _CST3XX_TP_RD_REG(reg, buf, len, tp_desc)      __CST3XX_TP_RD_REG(reg, buf, len, tp_desc, CST3XX_I2C_Addr, TP_I2C_TIMEOUT_DEFAULT)

/* const tp_desc_t *self is caller function parameters */
#define CST3XX_TP_WR_REG(reg, buf, len)      _CST3XX_TP_WR_REG(reg, buf, len, self)
#define CST3XX_TP_RD_REG(reg, buf, len)      _CST3XX_TP_RD_REG(reg, buf, len, self)

uint8_t cst3xx_init(tp_desc_t *self)
{
    if (!(self && self->board.hw_rst_gpio_set && self->board.hw_int_gpio_set_input && self->board.mdelay)) {
        CST3XX_LOG("[%s] parm error: [hw_rst_gpio_set / hw_int_gpio_set_input / mdelay] is invaild!\r\n", __FUNCTION__);
        return 1;
    }
    /* 检查本驱动适配列表 */
    if (0 == strcmp("CST328", self->cfg.name)) {
        CST3XX_I2C_Addr = CST328_I2C_ADDR;
    } else if (0 == strcmp("CST3240", self->cfg.name)) {
        CST3XX_I2C_Addr = CST3240_I2C_ADDR;
    } else {
        if (!strstr(self->cfg.name, "CST3")) CST3XX_LOG("[%s] Not belonging to [%s] driver!\r\n", self->cfg.name, "CST3xx");
        CST3XX_LOG("[%s] unadapted the driver!\r\n", self->cfg.name);
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

    /* 进入 debug 模式 */
    uint8_t temp[4] = {0};
    res = CST3XX_TP_WR_REG(HYN_REG_MUT_DEBUG_INFO_MODE, temp, 0);
    if (0 != res) {
        return res;
    }
    /* 读取固件信息 */
    res = CST3XX_TP_RD_REG(HYN_REG_MUT_DEBUG_INFO_BOOT_TIME, temp, 4);
    if (0 != res) {
        return res;
    }
    CST3XX_LOG("[%s] product id: 0x[%x][%x][%x][%x]\r\n", self->cfg.name, temp[3], temp[2], temp[1], temp[0]);
    /* 高位为 固件校验码 == 0xCACA, 低位为 Bootloader 时间 */
    if (temp[3] == 0xCA && temp[2] == 0xCA) {
        /* IC_Type / Project_ID */
        res = CST3XX_TP_RD_REG(HYN_REG_MUT_DEBUG_INFO_FW_VERSION, temp, 4);
        if (0 != res) {
            return res;
        }
        CST3XX_LOG("CST3x [FW_info] is true!, type / id = 0x[%x][%x] / 0x[%x][%x]\r\n", 
                    temp[0], temp[1], temp[2], temp[3]);
    }
    /* 退出 debug 模式, 进入 normal 模式 */
    res = CST3XX_TP_WR_REG(HYN_REG_MUT_NORMAL_MODE, temp, 0);
    if (0 != res) {
        return res;
    }
    /* 选配 INT 中断触发 */
    if (TP_MODE_INT == self->cfg.work_mode) {
        if (self->board.hw_int_gpio_interrupt_init) {
            self->board.hw_int_gpio_interrupt_init(0); //下降沿触发
        } else {
            CST3XX_LOG("error: [board.hw_int_gpio_interrupt_init] is invaild!\r\n");
        }
    }
    return 0;
}

uint8_t cst3xx_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points)
{
    if (points > CST3XX_Max_Points) {
        points = CST3XX_Max_Points; /* 以本驱动支持的最大点数为上限 */
    }

    uint8_t touch_nums = 0, i = 0;
    uint8_t buf[7] = {0};
    if (0 != CST3XX_TP_RD_REG(CST3XX_TP_Reg[0], buf, 7)) {
        CST3XX_LOG("touch_read_regs IIC addr invalid!\r\n");
        goto read_points_fail;
    }
    /*
    CST3XX_LOG("tp_buf = \r\n");
    for (uint32_t i = 0; i < sizeof(buf) / sizeof(buf[0]); ++i) {
        CST3XX_LOG("[%d][0x%x], ", i, buf[i]);
    }
    CST3XX_LOG("\r\n");
    */
    uint8_t data_invalid = 0;
    if (buf[6] != 0xAB || buf[0] == 0xAB) {
        CST3XX_LOG("TP data invalid!\r\n");
        data_invalid = 1; /* 数据无效 */
    }
    /* 发送同步命令 0xD000AB, 结束读取触摸 */
    uint8_t sync_reg = 0xAB;
    if (0 != CST3XX_TP_WR_REG(0xD000, &sync_reg, 1) || data_invalid > 0)
    {
        CST3XX_LOG("touch_write_regs IIC addr invalid or data invalid[%d]!\r\n", data_invalid);
        goto read_points_fail;
    }
    /* 手指触摸个数 */
    touch_nums = buf[5] & 0x0F;
    if (touch_nums > points) {
        touch_nums = points;
    }
    if (0 == touch_nums) {
        goto read_points_fail;
    }
    /* 目前仅实现对第 1 个手指的读取识别 */
    for (i = 0; i < touch_nums; ++i) {
        /* 手指状态:按下(0x06) */
        tp_data[i].sta = ((buf[0] & 0x0F) == 0x06) ? TP_STA_PRESSED : TP_STA_RELEASED;
        tp_data[i].x = ( ( (uint16_t)(buf[1] & 0xFF) ) << 4) | ( (buf[3] >> 4) & 0x0F );
        tp_data[i].y = ( ( (uint16_t)(buf[2] & 0xFF) ) << 4) | ( buf[3] & 0x0F );
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
        CST3XX_LOG("tp_pos: [%d, %d] \n", tp_data[i].x, tp_data[i].y);
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
//    CST3XX_LOG("[%s]: fail!\r\n", __FUNCTION__);
    return 0;
}
