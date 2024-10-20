/**
 *******************************************************************************************************************************************
 * @file        lcd_ili9488.c
 * @brief       LCD COG [ILI9488] driver
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * 2024-08-18   lzh          refactor [XXX_WR_DATA]
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

/* 枚举本驱动已适配 Driver_IC(COG) 为 ILI9488 的 LCM 模组型号:
 * 需要注意的是：即便使用了相同的 Driver_IC(COG), 但由于 LCM 模组供应商实现方式有差异(比如内部打线),
 * 不同型号的屏在初始化时序中设置参数部分可能会有个别不同, 需要自行向屏幕供应商索取相应参数.
 * MCU 端仅能提供驱动接口支持, 涉及到屏幕端的技术参数请找屏幕供应商, 以屏幕供应商提供的为准!
 */
#define JLT35002A_V2   "JLT35002A_V2" /**< 3.5 inch 320*480 晶力泰 */

#if 1 /* 调试打印 */
#include <stdio.h>
#define ILI9488_LOG(...)        printf(__VA_ARGS__)
#else
#define ILI9488_LOG(...)
#endif

/* self is caller function parameters(lcd_mpu_driver_t *) */
#define ILI9488_DELAY_MS(ms)             lcd_mpu_driver_mdelay(self, ms)
#define ILI9488_WR_CMD(cmd)              lcd_mpu_driver_wr_cmd(self, cmd, NULL)
#define ILI9488_WR_DATA(data, bytes)     lcd_mpu_driver_wr_data(self, data, bytes)
#define ILI9488_RD_DATA(data, bytes)     lcd_mpu_driver_rd_data(self, data, bytes)

#define ILI9488_WR_DATA_VA_ARGS(...)    do {                                     \
                                            uint8_t temp[] = {__VA_ARGS__};      \
                                            ILI9488_WR_DATA(temp, sizeof(temp)); \
                                        } while (0)

/**
 * @brief 上电初始化时序以配置屏幕参数
 */
void ili9488_timseq_init(lcd_mpu_driver_t *self, lcd_mpu_cfg_t *cfg)
{
    if (!(cfg && self && self->hw_rst_gpio_set && self->wr_cmd && self->wr_data && self->mdelay)) {
        ILI9488_LOG("[%s] parm error: [hw_rst_gpio_set / write_cmd / write_data / mdelay] is invaild!\r\n", __FUNCTION__);
        //for (;;) __NOP();
    }
    //ILI9488_DELAY_MS(120); //Wait for the power supply to stabilize

    /* RST */
    self->hw_rst_gpio_set(1); //H
    ILI9488_DELAY_MS(1);
    self->hw_rst_gpio_set(0); //L
    ILI9488_DELAY_MS(20);
    self->hw_rst_gpio_set(1); //H
    ILI9488_DELAY_MS(120);

#if 1 /************* Start Initial Sequence **********/
    ILI9488_WR_CMD(0x11);
    ILI9488_DELAY_MS(150);
    /* ILI9488 */

	/* PGAMCTRL (Positive Gamma Control) (E0h) */
	ILI9488_WR_CMD(0xE0);
	ILI9488_WR_DATA_VA_ARGS(0x00);
	ILI9488_WR_DATA_VA_ARGS(0x07);
	ILI9488_WR_DATA_VA_ARGS(0x10);
	ILI9488_WR_DATA_VA_ARGS(0x09);
	ILI9488_WR_DATA_VA_ARGS(0x17);
	ILI9488_WR_DATA_VA_ARGS(0x0B);
	ILI9488_WR_DATA_VA_ARGS(0x41);
	ILI9488_WR_DATA_VA_ARGS(0x89);
	ILI9488_WR_DATA_VA_ARGS(0x4B);
	ILI9488_WR_DATA_VA_ARGS(0x0A);
	ILI9488_WR_DATA_VA_ARGS(0x0C);
	ILI9488_WR_DATA_VA_ARGS(0x0E);
	ILI9488_WR_DATA_VA_ARGS(0x18);
	ILI9488_WR_DATA_VA_ARGS(0x1B);
	ILI9488_WR_DATA_VA_ARGS(0x0F);

	/* NGAMCTRL (Negative Gamma Control) (E1h)  */
	ILI9488_WR_CMD(0XE1);
	ILI9488_WR_DATA_VA_ARGS(0x00);
	ILI9488_WR_DATA_VA_ARGS(0x17);
	ILI9488_WR_DATA_VA_ARGS(0x1A);
	ILI9488_WR_DATA_VA_ARGS(0x04);
	ILI9488_WR_DATA_VA_ARGS(0x0E);
	ILI9488_WR_DATA_VA_ARGS(0x06);
	ILI9488_WR_DATA_VA_ARGS(0x2F);
	ILI9488_WR_DATA_VA_ARGS(0x45);
	ILI9488_WR_DATA_VA_ARGS(0x43);
	ILI9488_WR_DATA_VA_ARGS(0x02);
	ILI9488_WR_DATA_VA_ARGS(0x0A);
	ILI9488_WR_DATA_VA_ARGS(0x09);
	ILI9488_WR_DATA_VA_ARGS(0x32);
	ILI9488_WR_DATA_VA_ARGS(0x36);
	ILI9488_WR_DATA_VA_ARGS(0x0F);

	/* Adjust Control 3 (F7h)  */
	ILI9488_WR_CMD(0XF7);
	ILI9488_WR_DATA_VA_ARGS(0xA9);
	ILI9488_WR_DATA_VA_ARGS(0x51);
	ILI9488_WR_DATA_VA_ARGS(0x2C);
	ILI9488_WR_DATA_VA_ARGS(0x82); /* DSI write DCS command, use loose packet RGB 666 */

	/* Power Control 1 (C0h)  */
	ILI9488_WR_CMD(0xC0);
	ILI9488_WR_DATA_VA_ARGS(0x11);
	ILI9488_WR_DATA_VA_ARGS(0x09);

	/* Power Control 2 (C1h) */
	ILI9488_WR_CMD(0xC1);
	ILI9488_WR_DATA_VA_ARGS(0x41);

	/* VCOM Control (C5h)  */
	ILI9488_WR_CMD(0XC5);
	ILI9488_WR_DATA_VA_ARGS(0x00);
	ILI9488_WR_DATA_VA_ARGS(0x0A);
	ILI9488_WR_DATA_VA_ARGS(0x80);

	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	ILI9488_WR_CMD(0xB1);
	ILI9488_WR_DATA_VA_ARGS(0xB0);
	ILI9488_WR_DATA_VA_ARGS(0x11);

	/* Display Inversion Control (B4h) */
	ILI9488_WR_CMD(0xB4);
	ILI9488_WR_DATA_VA_ARGS(0x02);

	/* Display Function Control (B6h)  */
	ILI9488_WR_CMD(0xB6);
	ILI9488_WR_DATA_VA_ARGS(0x02);
	ILI9488_WR_DATA_VA_ARGS(0x22);

	/* Entry Mode Set (B7h)  */
	ILI9488_WR_CMD(0xB7);
	ILI9488_WR_DATA_VA_ARGS(0xc6);

	/* HS Lanes Control (BEh) */
	ILI9488_WR_CMD(0xBE);
	ILI9488_WR_DATA_VA_ARGS(0x00);
	ILI9488_WR_DATA_VA_ARGS(0x04);

	/* Set Image Function (E9h)  */
	ILI9488_WR_CMD(0xE9);
	ILI9488_WR_DATA_VA_ARGS(0x00);

    /* Set Refresh direction (36h)  */
    ILI9488_WR_CMD(0x36);
    ILI9488_WR_DATA_VA_ARGS(0x08); /* 有些屏内部接线 R 与 B 是对调的, 有些则不是, 详询屏幕供应商 */
    
    /* bit 7 : 0 (reserve)
     *
     * bit 6 ~ 4 : RGB self color format
     * ‘101’ = 65K of RGB self
     * ‘110’ = 262K of RGB self
     *
     * bit 3 : 0 (reserve)
     *
     * bit 2 ~ 0 : Control self color format
     * ‘011’ = 12bit/pixel
     * ‘101’ = 16bit/pixel
     * ‘110’ = 18bit/pixel
     * ‘111’ = 16M truncated
     */
    ILI9488_WR_CMD(0x3A);
    ILI9488_WR_DATA_VA_ARGS(0x55);

    ILI9488_WR_CMD(0x29);
    ILI9488_DELAY_MS(120); /* Sometimes it can also be ignored */
#endif
}

/**
 * @brief  设置显示窗口
 * @param  xs\xe\ys\ye : 窗口区域坐标(以 0 为起点)
 * @retval \
 */
void ili9488_set_disp_area(lcd_mpu_driver_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    ILI9488_WR_CMD(0x2A); /* X */
    ILI9488_WR_DATA_VA_ARGS((xs >> 8) & 0xFF, xs & 0xFF, 
                            (xe >> 8) & 0xFF, xe & 0xFF);

    ILI9488_WR_CMD(0x2B); /* Y */
    ILI9488_WR_DATA_VA_ARGS((ys >> 8) & 0xFF, ys & 0xFF, 
                            (ye >> 8) & 0xFF, ye & 0xFF);

    ILI9488_WR_CMD(0x2C); /* display on */
}

/**
 * @brief  设置屏幕旋转
 * @param  direction : 旋转方向(自定义实现)
 * @retval \
 */
void ili9488_set_rotate(lcd_mpu_driver_t *self, uint8_t direction)
{
    ILI9488_WR_CMD(0x36);
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
    ILI9488_WR_DATA_VA_ARGS((h4 << 4) | l4);
}

/**
 * @brief  设置屏幕背光
 * @param  val : 期望设置亮度
 * @retval 实际设置亮度
 */
uint32_t ili9488_set_backlight(lcd_mpu_driver_t *self, uint32_t val)
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
void ili9488_set_power(lcd_mpu_driver_t *self, uint8_t mode)
{
    /* 一些 COG 也支持以发送指令的方式来进入低功耗模式,
     * 亦可直接使用 GPIO 控制 TFT-LCD 的 Power In
     */
}
