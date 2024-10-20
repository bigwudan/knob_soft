/**
 *******************************************************************************************************************************************
 * @file        lcd_nv3041.c
 * @brief       LCD COG [NV3041] driver
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-08-18   lzh          the first version
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

/* 枚举本驱动已适配 Driver_IC(COG) 为 NV3041 的 LCM 模组型号:
 * 需要注意的是：即便使用了相同的 Driver_IC(COG), 但由于 LCM 模组供应商实现方式有差异(比如内部打线),
 * 不同型号的屏在初始化时序中设置参数部分可能会有个别不同, 需要自行向屏幕供应商索取相应参数.
 * MCU 端仅能提供驱动接口支持, 涉及到屏幕端的技术参数请找屏幕供应商, 以屏幕供应商提供的为准!
 */
#define H043A28   "H043A28" /* 4.3 inch [480*272](NV3041), 鑫洪泰（QSPI / i8080_8bit / SPI-4/3） */

#if 1 /* 调试打印 */
#include <stdio.h>
#define NV3041_LOG(...)        printf(__VA_ARGS__)
#else
#define NV3041_LOG(...)
#endif

/* self is caller function parameters(lcd_mpu_driver_t *) */
#define NV3041_DELAY_MS(ms)                         lcd_mpu_driver_mdelay(self, ms)
#define NV3041_WR_CMD(cmd, seq)                     lcd_mpu_driver_wr_cmd(self, cmd, seq)
#define NV3041_WR_DATA(data, bytes)                 lcd_mpu_driver_wr_data(self, data, bytes)
#define NV3041_RD_DATA(data, bytes)                 lcd_mpu_driver_rd_data(self, data, bytes)

/* QSPI 读写指令及时序须查看 COG 规格书设定以下宏 */
#include "dev_lcd_mpu_qspi_private.h"
//__LCD_QSPI_SEQ_SET_WR(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount)
//__LCD_QSPI_SEQ_SET_RD(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount)

//Command write mode: 
// Write Cycle Sequence: 
//PP  : Command - 02h(1_line)    24-bit address [0x00 cmd 00](1_line)    data(1_line)
// Write GRAM(0x2C/3C): 
//PP4O: Command - 32h(1_line)    24-bit address [0x00 cmd 00](1_line)    data(4_line)
//PP4O: Command - 12h(1_line)    24-bit address [0x00 cmd 00](4_line)    data(4_line)
#define NV3041_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes)          __LCD_QSPI_SEQ_SET_WR(seq, 0x02, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 1, bytes)
#define NV3041_QSPI_SEQ_SET_WR_L14(seq, cmd, bytes)          __LCD_QSPI_SEQ_SET_WR(seq, 0x32, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 4, bytes)
#define NV3041_QSPI_SEQ_SET_WR_L44(seq, cmd, bytes)          __LCD_QSPI_SEQ_SET_WR(seq, 0x12, 1, (cmd << 8) & 0xFF00, 4, 24, 0, 0, 0, 0, 4, bytes)

//Read command mode: (IO0 输出, IO1 输入)
//READ : Command - 03h(1_line)    24-bit address [0x00 cmd 00](1_line)        data(1_line)
#define NV3041_QSPI_SEQ_SET_RD(seq, cmd, bytes)              __LCD_QSPI_SEQ_SET_RD(seq, 0x03, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 1, bytes)

//选择写入 GRAM 的时序(1 / 2 / 4 line)
#define NV3041_QSPI_SEQ_SET_WR_GRAM(seq, cmd, bytes)         NV3041_QSPI_SEQ_SET_WR_L44(seq, cmd, bytes)

/* QSPI interface LTO */
#include "app_cfg.h"
#if defined(CFG_LCD_IF_QSPI) && CFG_LCD_IF != CFG_LCD_IF_QSPI
#undef NV3041_QSPI_SEQ_SET_WR_L11
#undef NV3041_QSPI_SEQ_SET_WR_L14
#undef NV3041_QSPI_SEQ_SET_WR_L44
#undef NV3041_QSPI_SEQ_SET_RD

#define NV3041_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes)
#define NV3041_QSPI_SEQ_SET_WR_L14(seq, cmd, bytes)
#define NV3041_QSPI_SEQ_SET_WR_L44(seq, cmd, bytes)
#define NV3041_QSPI_SEQ_SET_RD(seq, cmd, bytes)
#endif

/* QSPI extra expand */
#define NV3041_WR_CMD_SEQ(cmd, seq, bytes)          do {                                             \
                                                        NV3041_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes); \
                                                        NV3041_WR_CMD(cmd, seq);                     \
                                                    } while (0)
#define NV3041_WR_DATA_VA(...)                      do {                                    \
                                                        uint8_t temp[] = {__VA_ARGS__};     \
                                                        NV3041_WR_DATA(temp, sizeof(temp)); \
                                                    } while (0)
//eg: NV3041_WR_CMD_DATA_VA(&seq, cmd, data_1, data_2, data_3, ...)
#define NV3041_WR_CMD_DATA_VA(seq, cmd, ...)        do {                                                    \
                                                        uint8_t data[] = {__VA_ARGS__};                     \
                                                        NV3041_QSPI_SEQ_SET_WR_L11(seq, cmd, sizeof(data)); \
                                                        NV3041_WR_CMD(cmd, seq);                            \
                                                        NV3041_WR_DATA(data, sizeof(data));                 \
                                                    } while (0)

/**
 * @brief 上电初始化时序以配置屏幕参数
 */
void nv3041_timseq_init(lcd_mpu_driver_t *self, lcd_mpu_cfg_t *cfg)
{
    if (!(cfg && self && self->hw_rst_gpio_set && self->wr_cmd && self->wr_data && self->mdelay)) {
        NV3041_LOG("[%s] parm error: [hw_rst_gpio_set / write_cmd / write_data / mdelay] is invaild!\r\n", __FUNCTION__);
        //for (;;) __NOP();
    }
    //NV3041_DELAY_MS(120); //Wait for the power supply to stabilize

    /* RST */
    self->hw_rst_gpio_set(1); //H
    NV3041_DELAY_MS(1);
    self->hw_rst_gpio_set(0); //L
    NV3041_DELAY_MS(20);
    self->hw_rst_gpio_set(1); //H
    NV3041_DELAY_MS(120);

    /************* Start Initial Sequence **********/
#if 1
    lcd_qspi_seq_t seq;
    #define WR_CMD_0(cmd)                   NV3041_WR_CMD_SEQ(cmd, &seq, 0) //无参数的指令
    #define WR_CMD_1(cmd)                   NV3041_WR_CMD_SEQ(cmd, &seq, 1) //仅单字节参数的指令
    #define WR_DATA_VA(...)                 NV3041_WR_DATA_VA(__VA_ARGS__)
    #define WR_CMD_DATA_VA(cmd, ...)        NV3041_WR_CMD_DATA_VA(&seq, cmd, __VA_ARGS__)
    
#if 1 //SPI，MCU 接口初始化代码共用
    WR_CMD_1(0xff);
    WR_DATA_VA(0xa5);
    WR_CMD_1(0xE7);//TE_output_en
    WR_DATA_VA(0x10);
    WR_CMD_1(0x35);//TE_interface_en
    WR_DATA_VA(0x01);
    WR_CMD_1(0x3A);
    WR_DATA_VA(0x01);//00---666//01--565
    WR_CMD_1(0x40);
    WR_DATA_VA(0x01);
    WR_CMD_1(0x41);
    WR_DATA_VA(0x01);//01--8bit//03--16bit
    WR_CMD_1(0x55);
    WR_DATA_VA(0x01);
    WR_CMD_1(0x44);
    WR_DATA_VA(0x15);
    WR_CMD_1(0x45);
    WR_DATA_VA(0x15);
    WR_CMD_1(0x7d);
    WR_DATA_VA(0x03);
    WR_CMD_1(0xc1);
    WR_DATA_VA(0xab);
    WR_CMD_1(0xc2);
    WR_DATA_VA(0x17);
    WR_CMD_1(0xc3);
    WR_DATA_VA(0x10);

    WR_CMD_1(0xc6);
    WR_DATA_VA(0x3a);
    WR_CMD_1(0xc7);
    WR_DATA_VA(0x25);
    WR_CMD_1(0xc8);
    WR_DATA_VA(0x11);
    WR_CMD_1(0x7a);
    WR_DATA_VA(0x49);
    WR_CMD_1(0x6f);
    WR_DATA_VA(0x2f);
    WR_CMD_1(0x78);
    WR_DATA_VA(0x4b);
    WR_CMD_1(0x73);
    WR_DATA_VA(0x08);
    WR_CMD_1(0x74);
    WR_DATA_VA(0x12);
    WR_CMD_1(0xc9);
    WR_DATA_VA(0x00);
    WR_CMD_1(0x67);
    WR_DATA_VA(0x11);
    WR_CMD_1(0x51);
    WR_DATA_VA(0x20);
    WR_CMD_1(0x52);
    WR_DATA_VA(0x7c);
    WR_CMD_1(0x53);
    WR_DATA_VA(0x1c);
    WR_CMD_1(0x54);
    WR_DATA_VA(0x77);
    WR_CMD_1(0x46);
    WR_DATA_VA(0x0a);
    WR_CMD_1(0x47);
    WR_DATA_VA(0x2a);
    WR_CMD_1(0x48);
    WR_DATA_VA(0x0a);
    WR_CMD_1(0x49);
    WR_DATA_VA(0x1a);
    WR_CMD_1(0x56);

    WR_DATA_VA(0x43);
    WR_CMD_1(0x57);
    WR_DATA_VA(0x42);
    WR_CMD_1(0x58);
    WR_DATA_VA(0x3c);
    WR_CMD_1(0x59);
    WR_DATA_VA(0x64);
    WR_CMD_1(0x5a);
    WR_DATA_VA(0x41);
    WR_CMD_1(0x5b);
    WR_DATA_VA(0x3c);
    WR_CMD_1(0x5c);
    WR_DATA_VA(0x02);
    WR_CMD_1(0x5d);
    WR_DATA_VA(0x3c);
    WR_CMD_1(0x5e);
    WR_DATA_VA(0x1f);
    WR_CMD_1(0x60);
    WR_DATA_VA(0x80);
    WR_CMD_1(0x61);
    WR_DATA_VA(0x3f);
    WR_CMD_1(0x62);
    WR_DATA_VA(0x21);
    WR_CMD_1(0x63);
    WR_DATA_VA(0x07);
    WR_CMD_1(0x64);
    WR_DATA_VA(0xe0);
    WR_CMD_1(0x65);
    WR_DATA_VA(0x01);
    WR_CMD_1(0xca);
    WR_DATA_VA(0x20);
    WR_CMD_1(0xcb);
    WR_DATA_VA(0x52);
    WR_CMD_1(0xcc);
    WR_DATA_VA(0x10);
    WR_CMD_1(0xcD);
    WR_DATA_VA(0x42);
    WR_CMD_1(0xD0);
    WR_DATA_VA(0x20);

    WR_CMD_1(0xD1);
    WR_DATA_VA(0x52);
    WR_CMD_1(0xD2);
    WR_DATA_VA(0x10);
    WR_CMD_1(0xD3);
    WR_DATA_VA(0x42);
    WR_CMD_1(0xD4);
    WR_DATA_VA(0x0a);
    WR_CMD_1(0xD5);
    WR_DATA_VA(0x32);
    WR_CMD_1(0xe5);
    WR_DATA_VA(0x06);
    WR_CMD_1(0xe6);
    WR_DATA_VA(0x00);
    WR_CMD_1(0x6e);
    WR_DATA_VA(0x14);
    //gammma 01
    WR_CMD_1(0x80);
    WR_DATA_VA(0x04);
    WR_CMD_1(0xA0);
    WR_DATA_VA(0x00);
    WR_CMD_1(0x81);
    WR_DATA_VA(0x07);
    WR_CMD_1(0xA1);
    WR_DATA_VA(0x05);
    WR_CMD_1(0x82);
    WR_DATA_VA(0x06);
    WR_CMD_1(0xA2);
    WR_DATA_VA(0x04);
    WR_CMD_1(0x83);
    WR_DATA_VA(0x39);
    WR_CMD_1(0xA3);
    WR_DATA_VA(0x39);
    WR_CMD_1(0x84);
    WR_DATA_VA(0x3a);
    WR_CMD_1(0xA4);
    WR_DATA_VA(0x3a);
    WR_CMD_1(0x85);
    WR_DATA_VA(0x3f);
    WR_CMD_1(0xA5);

    WR_DATA_VA(0x3f);
    WR_CMD_1(0x86);
    WR_DATA_VA(0x2c);
    WR_CMD_1(0xA6);
    WR_DATA_VA(0x2a);
    WR_CMD_1(0x87);
    WR_DATA_VA(0x43);
    WR_CMD_1(0xA7);
    WR_DATA_VA(0x47);
    WR_CMD_1(0x88);
    WR_DATA_VA(0x08);
    WR_CMD_1(0xA8);
    WR_DATA_VA(0x08);
    WR_CMD_1(0x89);
    WR_DATA_VA(0x0f);
    WR_CMD_1(0xA9);
    WR_DATA_VA(0x0f);
    WR_CMD_1(0x8a);
    WR_DATA_VA(0x17);
    WR_CMD_1(0xAa);
    WR_DATA_VA(0x17);
    WR_CMD_1(0x8b);
    WR_DATA_VA(0x10);
    WR_CMD_1(0xAb);
    WR_DATA_VA(0x10);
    WR_CMD_1(0x8c);
    WR_DATA_VA(0x16);
    WR_CMD_1(0xAc);
    WR_DATA_VA(0x16);
    WR_CMD_1(0x8d);
    WR_DATA_VA(0x14);
    WR_CMD_1(0xAd);
    WR_DATA_VA(0x14);
    WR_CMD_1(0x8e);
    WR_DATA_VA(0x11);
    WR_CMD_1(0xAe);
    WR_DATA_VA(0x11);
    WR_CMD_1(0x8f);
    WR_DATA_VA(0x14);

    WR_CMD_1(0xAf);
    WR_DATA_VA(0x14);
    WR_CMD_1(0x90);
    WR_DATA_VA(0x06);
    WR_CMD_1(0xB0);
    WR_DATA_VA(0x06);
    WR_CMD_1(0x91);
    WR_DATA_VA(0x0f);
    WR_CMD_1(0xB1);
    WR_DATA_VA(0x0f);
    WR_CMD_1(0x92);
    WR_DATA_VA(0x16);
    WR_CMD_1(0xB2);
    WR_DATA_VA(0x16);
    WR_CMD_1(0xff);
    WR_DATA_VA(0x00);
#endif

    WR_CMD_0(0x11);
    NV3041_DELAY_MS(120);//200
    WR_CMD_0(0x29);
    NV3041_DELAY_MS(120);
#endif
}

/**
 * @brief  设置显示窗口
 * @param  xs\xe\ys\ye : 窗口区域坐标(以 0 为起点)
 * @retval \
 */
void nv3041_set_disp_area(lcd_mpu_driver_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    lcd_qspi_seq_t seq;
    NV3041_WR_CMD_DATA_VA(&seq, 0x2A, (xs >> 8) & 0xFF, xs & 0xFF, (xe >> 8) & 0xFF, xe & 0xFF );
    NV3041_WR_CMD_DATA_VA(&seq, 0x2B, (ys >> 8) & 0xFF, ys & 0xFF, (ye >> 8) & 0xFF, ye & 0xFF );
    
    uint32_t pixels = (xe - xs + 1) * (ye - ys + 1);
    NV3041_QSPI_SEQ_SET_WR_GRAM(&seq, 0x2C, pixels << 1); //RGB565: 1 pixel = 2 Bytes  
    NV3041_WR_CMD(0x2C, &seq);
}

/**
 * @brief  设置屏幕旋转
 * @param  direction : 旋转方向(自定义实现)
 * @retval \
 */
void nv3041_set_rotate(lcd_mpu_driver_t *self, uint8_t direction)
{
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
    lcd_qspi_seq_t seq;
    NV3041_WR_CMD_DATA_VA(&seq, 0x36, (h4 << 4) | l4);
}

/**
 * @brief  设置屏幕背光
 * @param  val : 期望设置亮度
 * @retval 实际设置亮度
 */
uint32_t nv3041_set_backlight(lcd_mpu_driver_t *self, uint32_t val)
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
void nv3041_set_power(lcd_mpu_driver_t *self, uint8_t mode)
{
    /* 一些 COG 也支持以发送指令的方式来进入低功耗模式,
     * 亦可直接使用 GPIO 控制 TFT-LCD 的 Power In
     */
    /* Panel_SleepIn_Mode
    {
        WR_CMD_0(0x28);
        Delayms (120);
        WR_CMD_0(0x10);
        Delayms (120);
    }*/
    /* Panel_SleepOut_Mode
    {
        WR_CMD_0(0x11);
        Delayms (120);
        WR_CMD_0(0x29);
        Delayms (120);
    }*/
}
