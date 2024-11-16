/**
 *******************************************************************************************************************************************
 * @file        lcd_st7789.c
 * @brief       LCD COG [ST7789] driver
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   tw           the first version
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

/* 枚举本驱动已适配 Driver_IC(COG) 为 ST7789 的 LCM 模组型号:
 * 需要注意的是：即便使用了相同的 Driver_IC(COG), 但由于 LCM 模组供应商实现方式有差异(比如内部打线),
 * 不同型号的屏在初始化时序中设置参数部分可能会有个别不同, 需要自行向屏幕供应商索取相应参数.
 * MCU 端仅能提供驱动接口支持, 涉及到屏幕端的技术参数请找屏幕供应商, 以屏幕供应商提供的为准!
 */
#define SIMLEWAY24     "SIMLEWAY24"  /**< 2.4 inch 240*320 思迈微 */
#define ZJY240IT008    "ZJY240IT008" /**< 2.4 inch 240*320 中景园 */

#if 1 /* 调试打印 */
#include <stdio.h>
#define ST7789_LOG(...)        printf(__VA_ARGS__)
#else
#define ST7789_LOG(...)
#endif

/* self is caller function parameters(lcd_mpu_driver_t *) */
#define ST7789_DELAY_MS(ms)             lcd_mpu_driver_mdelay(self, ms)
#define ST7789_WR_CMD(cmd)              lcd_mpu_driver_wr_cmd(self, cmd, NULL)
#define ST7789_WR_DATA(data, bytes)     lcd_mpu_driver_wr_data(self, data, bytes)
#define ST7789_RD_DATA(data, bytes)     lcd_mpu_driver_rd_data(self, data, bytes)

#define ST7789_WR_DATA_VA_ARGS(...)     do {                                    \
                                            uint8_t temp[] = {__VA_ARGS__};     \
                                            ST7789_WR_DATA(temp, sizeof(temp)); \
                                        } while (0)

/**
 * @brief 上电初始化时序以配置屏幕参数
 */
void st7789_timseq_init(lcd_mpu_driver_t *self, lcd_mpu_cfg_t *cfg)
{
    if (!(cfg && self && self->hw_rst_gpio_set && self->wr_cmd && self->wr_data && self->mdelay)) {
        ST7789_LOG("[%s] parm error: [hw_rst_gpio_set / write_cmd / write_data / mdelay] is invaild!\r\n", __FUNCTION__);
        //for (;;) __NOP();
    }
    //ST7789_DELAY_MS(120); //Wait for the power supply to stabilize

    /* RST */
    self->hw_rst_gpio_set(1); //H
    ST7789_DELAY_MS(1);
    self->hw_rst_gpio_set(0); //L
    ST7789_DELAY_MS(20);
    self->hw_rst_gpio_set(1); //H
    ST7789_DELAY_MS(120);

#if 1 /************* Start Initial Sequence **********/
    ST7789_WR_CMD(0x11);
    ST7789_DELAY_MS(150);
    /* ST7789 */
	ST7789_WR_CMD(0xB2);     
	ST7789_WR_DATA_VA_ARGS(0x0C);   
	ST7789_WR_DATA_VA_ARGS(0x0C);   
	ST7789_WR_DATA_VA_ARGS(0x00);   
	ST7789_WR_DATA_VA_ARGS(0x33);   
	ST7789_WR_DATA_VA_ARGS(0x33);  

	ST7789_WR_CMD(0xB7);     
	ST7789_WR_DATA_VA_ARGS(0x76);   

	ST7789_WR_CMD(0xBB);     
	ST7789_WR_DATA_VA_ARGS(0x22);   

	ST7789_WR_CMD(0xC0);     
	ST7789_WR_DATA_VA_ARGS(0x2C);   

	ST7789_WR_CMD(0xC2);     
	ST7789_WR_DATA_VA_ARGS(0x01);   

	ST7789_WR_CMD(0xC3);     
	ST7789_WR_DATA_VA_ARGS(0x13);   

	ST7789_WR_CMD(0xC6);     
	ST7789_WR_DATA_VA_ARGS(0x0F);     

	ST7789_WR_CMD(0xD0);     
	ST7789_WR_DATA_VA_ARGS(0xA4);   
	ST7789_WR_DATA_VA_ARGS(0xA1);   

	ST7789_WR_CMD(0xD6);     
	ST7789_WR_DATA_VA_ARGS(0xA1); 

	ST7789_WR_CMD(0xE0);
	ST7789_WR_DATA_VA_ARGS(0xD0);
	ST7789_WR_DATA_VA_ARGS(0x04);
	ST7789_WR_DATA_VA_ARGS(0x0E);
	ST7789_WR_DATA_VA_ARGS(0x15);
	ST7789_WR_DATA_VA_ARGS(0x16);
	ST7789_WR_DATA_VA_ARGS(0x17);
	ST7789_WR_DATA_VA_ARGS(0x3F);
	ST7789_WR_DATA_VA_ARGS(0x54);
	ST7789_WR_DATA_VA_ARGS(0x4E);
	ST7789_WR_DATA_VA_ARGS(0x09);
	ST7789_WR_DATA_VA_ARGS(0x0F);
	ST7789_WR_DATA_VA_ARGS(0x0D);
	ST7789_WR_DATA_VA_ARGS(0x1C);
	ST7789_WR_DATA_VA_ARGS(0x1E);

	ST7789_WR_CMD(0xE1);
	ST7789_WR_DATA_VA_ARGS(0xD0);
	ST7789_WR_DATA_VA_ARGS(0x02);
	ST7789_WR_DATA_VA_ARGS(0x0A);
	ST7789_WR_DATA_VA_ARGS(0x0C);
	ST7789_WR_DATA_VA_ARGS(0x0E);
	ST7789_WR_DATA_VA_ARGS(0x30);
	ST7789_WR_DATA_VA_ARGS(0x3F);
	ST7789_WR_DATA_VA_ARGS(0x54);
	ST7789_WR_DATA_VA_ARGS(0x4E);
	ST7789_WR_DATA_VA_ARGS(0x3E);
	ST7789_WR_DATA_VA_ARGS(0x1C);
	ST7789_WR_DATA_VA_ARGS(0x1A);
	ST7789_WR_DATA_VA_ARGS(0x23);
	ST7789_WR_DATA_VA_ARGS(0x26);
    
    /* bit 7 : 0 (reserve)
     *
     * bit 6 ~ 4 : RGB interface color format
     * ‘101’ = 65K of RGB interface
     * ‘110’ = 262K of RGB interface
     *
     * bit 3 : 0 (reserve)
     *
     * bit 2 ~ 0 : Control interface color format
     * ‘011’ = 12bit/pixel
     * ‘101’ = 16bit/pixel
     * ‘110’ = 18bit/pixel
     * ‘111’ = 16M truncated
     */
    ST7789_WR_CMD(0x3A);
    ST7789_WR_DATA_VA_ARGS(0x55);

    /* This command is used to recover from display inversion mode
     * 个别屏内部 Memory 写入值与实际显示是反向的, 详询屏幕供应商
     */
//    ST7789_WR_CMD(0x21);
    
    ST7789_WR_CMD(0x36);
    /* D3 : RGB/BGR Order
     * D1 ~ 0 : reserve
     */
    if (0 == strcmp(cfg->name, ZJY240IT008)) {
        ST7789_WR_DATA_VA_ARGS(0x00);
    }
    else if (0 == strcmp(cfg->name, SIMLEWAY24)) {
        ST7789_WR_DATA_VA_ARGS(0x48); /* 有些屏内部接线 R 与 B 是对调的, 有些则不是, 详询屏幕供应商 */
    }

    ST7789_WR_CMD(0x29);
    ST7789_DELAY_MS(120); /* Sometimes it can also be ignored */
#endif
}

/**
 * @brief  设置显示窗口
 * @param  xs\xe\ys\ye : 窗口区域坐标(以 0 为起点)
 * @retval \
 */
void st7789_set_disp_area(lcd_mpu_driver_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    ST7789_WR_CMD(0x2A); /* X */
    ST7789_WR_DATA_VA_ARGS((xs >> 8) & 0xFF, xs & 0xFF, 
                           (xe >> 8) & 0xFF, xe & 0xFF);

    ST7789_WR_CMD(0x2B); /* Y */
    ST7789_WR_DATA_VA_ARGS((ys >> 8) & 0xFF, ys & 0xFF, 
                           (ye >> 8) & 0xFF, ye & 0xFF);

    ST7789_WR_CMD(0x2C); /* display on */
}

/**
 * @brief  设置屏幕旋转
 * @param  direction : 旋转方向(自定义实现)
 * @retval \
 */
void st7789_set_rotate(lcd_mpu_driver_t *self, uint8_t direction)
{
    ST7789_WR_CMD(0x36);
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
    ST7789_WR_DATA_VA_ARGS((h4 << 4) | l4);
}

/**
 * @brief  设置屏幕背光
 * @param  val : 期望设置亮度
 * @retval 实际设置亮度
 */
uint32_t st7789_set_backlight(lcd_mpu_driver_t *self, uint32_t val)
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
void st7789_set_power(lcd_mpu_driver_t *self, uint8_t mode)
{
    /* 一些 COG 也支持以发送指令的方式来进入低功耗模式,
     * 亦可直接使用 GPIO 控制 TFT-LCD 的 Power In
     */
}
