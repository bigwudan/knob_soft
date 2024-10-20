/**
 *******************************************************************************************************************************************
 * @file        lcd_nv3023.c
 * @brief       LCD COG [NV3023] driver
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-09-18   lzh          the first version
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
#include "dev_lcd_mpu.h"

/* 枚举本驱动已适配 Driver_IC(COG) 为 NV3023 的 LCM 模组型号:
 * 需要注意的是：即便使用了相同的 Driver_IC(COG), 但由于 LCM 模组供应商实现方式有差异(比如内部打线),
 * 不同型号的屏在初始化时序中设置参数部分可能会有个别不同, 需要自行向屏幕供应商索取相应参数.
 * MCU 端仅能提供驱动接口支持, 涉及到屏幕端的技术参数请找屏幕供应商, 以屏幕供应商提供的为准!
 */
#define YH_173BX6025N2   "YH_173BX6025N2" /**< 1.73 inch 128*160 亿华 */

#if 1 /* 调试打印 */
#include <stdio.h>
#define NV3023_LOG(...)        printf(__VA_ARGS__)
#else
#define NV3023_LOG(...)
#endif

/* self is caller function parameters(lcd_mpu_driver_t *) */
#define NV3023_DELAY_MS(ms)             lcd_mpu_driver_mdelay(self, ms)
#define NV3023_WR_CMD(cmd)              lcd_mpu_driver_wr_cmd(self, cmd, NULL)
#define NV3023_WR_DATA(data, bytes)     lcd_mpu_driver_wr_data(self, data, bytes)
#define NV3023_RD_DATA(data, bytes)     lcd_mpu_driver_rd_data(self, data, bytes)

#define NV3023_WR_DATA_VA_ARGS(...)     do {                                    \
                                            uint8_t temp[] = {__VA_ARGS__};     \
                                            NV3023_WR_DATA(temp, sizeof(temp)); \
                                        } while (0)

/**
 * @brief 上电初始化时序以配置屏幕参数
 */
void nv3023_timseq_init(lcd_mpu_driver_t *self, lcd_mpu_cfg_t *cfg)
{
    if (!(cfg && self && self->hw_rst_gpio_set && self->wr_cmd && self->wr_data && self->mdelay)) {
        NV3023_LOG("[%s] parm error: [hw_rst_gpio_set / write_cmd / write_data / mdelay] is invaild!\r\n", __FUNCTION__);
        //for (;;) __NOP();
    }
    //NV3023_DELAY_MS(120); //Wait for the power supply to stabilize

    /* RST */
    self->hw_rst_gpio_set(1); //H
    NV3023_DELAY_MS(1);
    self->hw_rst_gpio_set(0); //L
    NV3023_DELAY_MS(20);
    self->hw_rst_gpio_set(1); //H
    NV3023_DELAY_MS(120);

#if 1 /************* Start Initial Sequence **********/
    NV3023_WR_CMD(0x11);
    NV3023_DELAY_MS(150);
    /* NV3023 */
    NV3023_WR_CMD(0xff);
    NV3023_WR_DATA_VA_ARGS(0xa5); //
    NV3023_WR_CMD(0x3E);
    NV3023_WR_DATA_VA_ARGS(0x09);
    NV3023_WR_CMD(0x3A);
    NV3023_WR_DATA_VA_ARGS(0x65);
    NV3023_WR_CMD(0x82);
    NV3023_WR_DATA_VA_ARGS(0x00);
    NV3023_WR_CMD(0x98);
    NV3023_WR_DATA_VA_ARGS(0x00);
    NV3023_WR_CMD(0x63);
    NV3023_WR_DATA_VA_ARGS(0x0f);
    NV3023_WR_CMD(0x64);
    NV3023_WR_DATA_VA_ARGS(0x0f);
    NV3023_WR_CMD(0xB4);
    NV3023_WR_DATA_VA_ARGS(0x33);
    NV3023_WR_CMD(0xB5);
    NV3023_WR_DATA_VA_ARGS(0x30);
    NV3023_WR_CMD(0x83);
    NV3023_WR_DATA_VA_ARGS(0x03);
    NV3023_WR_CMD(0x86);//frc
    NV3023_WR_DATA_VA_ARGS(0x07);
    NV3023_WR_CMD(0x87);
    NV3023_WR_DATA_VA_ARGS(0x16);//16
    NV3023_WR_CMD(0x88);//VCOM
    NV3023_WR_DATA_VA_ARGS(0x28);
    NV3023_WR_CMD(0x89);//
    NV3023_WR_DATA_VA_ARGS(0x2d);//2F
    NV3023_WR_CMD(0x93); //
    NV3023_WR_DATA_VA_ARGS(0x63);
    NV3023_WR_CMD(0x96);
    NV3023_WR_DATA_VA_ARGS(0x81);
    NV3023_WR_CMD(0xC3);
    NV3023_WR_DATA_VA_ARGS(0x10);
    NV3023_WR_CMD(0xE6);
    NV3023_WR_DATA_VA_ARGS(0x00);
    NV3023_WR_CMD(0x99);
    NV3023_WR_DATA_VA_ARGS(0x01);
    ////////////////////////gamma_set_ips//////////////////////////////////////
    NV3023_WR_CMD(0x70);NV3023_WR_DATA_VA_ARGS(0x0c);//VRP 31
    NV3023_WR_CMD(0x71);NV3023_WR_DATA_VA_ARGS(0x24);//VRP 30
    NV3023_WR_CMD(0x72);NV3023_WR_DATA_VA_ARGS(0x15);//VRP 29
    NV3023_WR_CMD(0x73);NV3023_WR_DATA_VA_ARGS(0x0b);//VRP 28
    NV3023_WR_CMD(0x74);NV3023_WR_DATA_VA_ARGS(0x17);//VRP 25
    NV3023_WR_CMD(0x75);NV3023_WR_DATA_VA_ARGS(0x1b);//VRP 23
    NV3023_WR_CMD(0x76);NV3023_WR_DATA_VA_ARGS(0x3a);//VRP 21
    NV3023_WR_CMD(0x77);NV3023_WR_DATA_VA_ARGS(0x0c);//VRP 17
    NV3023_WR_CMD(0x78);NV3023_WR_DATA_VA_ARGS(0x08);//VRP 14
    NV3023_WR_CMD(0x79);NV3023_WR_DATA_VA_ARGS(0x44);//VRP 10
    NV3023_WR_CMD(0x7a);NV3023_WR_DATA_VA_ARGS(0x06);//VRP 8
    NV3023_WR_CMD(0x7b);NV3023_WR_DATA_VA_ARGS(0x0c);//VRP 6
    NV3023_WR_CMD(0x7c);NV3023_WR_DATA_VA_ARGS(0x0f);//VRP 3
    NV3023_WR_CMD(0x7d);NV3023_WR_DATA_VA_ARGS(0x1c);//VRP 2
    NV3023_WR_CMD(0x7e);NV3023_WR_DATA_VA_ARGS(0x25);//VRP 1
    NV3023_WR_CMD(0x7f);NV3023_WR_DATA_VA_ARGS(0x0a);//VRP 0
    NV3023_WR_CMD(0xa0);NV3023_WR_DATA_VA_ARGS(0x0b);//VRN 31
    NV3023_WR_CMD(0xa1);NV3023_WR_DATA_VA_ARGS(0x33);//VRN 30
    NV3023_WR_CMD(0xa2);NV3023_WR_DATA_VA_ARGS(0x0c);//VRN 29
    NV3023_WR_CMD(0xa3);NV3023_WR_DATA_VA_ARGS(0x12);//VRN 28
    NV3023_WR_CMD(0xa4);NV3023_WR_DATA_VA_ARGS(0x0f);//VRN 25
    NV3023_WR_CMD(0xa5);NV3023_WR_DATA_VA_ARGS(0x26);//VRN 23
    NV3023_WR_CMD(0xa6);NV3023_WR_DATA_VA_ARGS(0x46);//VRN 21
    NV3023_WR_CMD(0xa7);NV3023_WR_DATA_VA_ARGS(0x07);//VRN 17
    NV3023_WR_CMD(0xa8);NV3023_WR_DATA_VA_ARGS(0x09);//VRN 14
    NV3023_WR_CMD(0xa9);NV3023_WR_DATA_VA_ARGS(0x40);//VRN 10
    NV3023_WR_CMD(0xaa);NV3023_WR_DATA_VA_ARGS(0x0c);//VRN 8
    NV3023_WR_CMD(0xab);NV3023_WR_DATA_VA_ARGS(0x15);//VRN 6
    NV3023_WR_CMD(0xac);NV3023_WR_DATA_VA_ARGS(0x0e);//VRN 3
    NV3023_WR_CMD(0xad);NV3023_WR_DATA_VA_ARGS(0x05);//VRN 2
    NV3023_WR_CMD(0xae);NV3023_WR_DATA_VA_ARGS(0x2e);//VRN 1
    NV3023_WR_CMD(0xaf);NV3023_WR_DATA_VA_ARGS(0x07);//VRN 0

    
    NV3023_WR_CMD(0xff);
    NV3023_WR_DATA_VA_ARGS(0x00);

//    NV3023_WR_CMD(0x11);
//   NV3023_DELAY_MS(120);

    NV3023_WR_CMD(0x36);
    NV3023_WR_DATA_VA_ARGS(0x88);

    NV3023_WR_CMD(0x29);
    NV3023_DELAY_MS(120); /* Sometimes it can also be ignored */
#endif
}

/**
 * @brief  设置显示窗口
 * @param  xs\xe\ys\ye : 窗口区域坐标(以 0 为起点)
 * @retval \
 */
void nv3023_set_disp_area(lcd_mpu_driver_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    NV3023_WR_CMD(0x2A); /* X */
    NV3023_WR_DATA_VA_ARGS((xs >> 8) & 0xFF, xs & 0xFF, 
                           (xe >> 8) & 0xFF, xe & 0xFF);

    NV3023_WR_CMD(0x2B); /* Y */
    NV3023_WR_DATA_VA_ARGS((ys >> 8) & 0xFF, ys & 0xFF, 
                           (ye >> 8) & 0xFF, ye & 0xFF);

    NV3023_WR_CMD(0x2C); /* display on */
}

/**
 * @brief  设置屏幕旋转
 * @param  direction : 旋转方向(自定义实现)
 * @retval \
 */
void nv3023_set_rotate(lcd_mpu_driver_t *self, uint8_t direction)
{
    NV3023_WR_CMD(0x36);
    /* Memory Access Control (36h)
     * This command defines read/write scanning direction of the frame memory.
     *
     * These 3 bits control the direction from the MPU to memory write/read.
     *
     * Bit  Symbol  Name  Description
     * D7   MY  Row Address Order     -- 以X轴镜像
     * D6   MX  Column Address Order  -- 以Y轴镜像
     * D5   MV  Row/Column Exchange   -- X轴与Y轴交换
     * D4   ML  Vertical Refresh Order  LCD vertical refresh direction control.
     *
     * D3   BGR RGB-BGR Order   Color selector switch control
     *      (0 = RGB color filter panel, 1 = BGR color filter panel )
     * D2   MH  Horizontal Refresh ORDER  LCD horizontal refreshing direction control.
     * D1   X   Reserved  Reserved
     * D0   X   Reserved  Reserved
     */
    uint8_t h4 = 0, l4 = 0x08; /* 有些屏内部接线 R 与 B 是对调的, 有些则不是, 详询屏幕供应商 */
    switch (direction)
    {
    default : //break;
    case 0:
        h4 = 0;
        break;
    case 1:
        h4 = 1 << 1;
        break;
    case 2:
        h4 = 1 << 2;
        break;
    case 3:
        h4 = 1 << 3;
        break;
    
    case 4:
        h4 = (1 << 1) | (1 << 2);
        break;
    case 5:
        h4 = (1 << 1) | (1 << 3);
        break;
    case 6:
        h4 = (1 << 2) | (1 << 3);
        break;
    case 7:
        h4 = (1 << 1) | (1 << 2) | (1 << 3);
        break;
    }
    NV3023_WR_DATA_VA_ARGS((h4 << 4) | l4);
}

/**
 * @brief  设置屏幕背光
 * @param  val : 期望设置亮度
 * @retval 实际设置亮度
 */
uint32_t nv3023_set_backlight(lcd_mpu_driver_t *self, uint32_t val)
{
    /* 一些 COG 也支持以发送指令的方式来调节亮度,
    * 亦可直接使用 GPIO 控制 TFT-LCD 的 LED-K 或 使用硬件 PWM 模块实现高频 PWM 调光
    */
    return 0;
}

/**
 * @brief  设置屏幕电源
 * @param  mode : 0-低功耗  other-正常
 * @retval \
 */
void nv3023_set_power(lcd_mpu_driver_t *self, uint8_t mode)
{
    /* 一些 COG 也支持以发送指令的方式来进入低功耗模式,
     * 亦可直接使用 GPIO 控制 TFT-LCD 的 Power In
     */
}
