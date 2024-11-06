/**
 *******************************************************************************************************************************************
 * @file        lcd_st77916.c
 * @brief       LCD COG [ST77916] driver
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

/* 枚举本驱动已适配 Driver_IC(COG) 为 ST77916 的 LCM 模组型号:
 * 需要注意的是：即便使用了相同的 Driver_IC(COG), 但由于 LCM 模组供应商实现方式有差异(比如内部打线),
 * 不同型号的屏在初始化时序中设置参数部分可能会有个别不同, 需要自行向屏幕供应商索取相应参数.
 * MCU 端仅能提供驱动接口支持, 涉及到屏幕端的技术参数请找屏幕供应商, 以屏幕供应商提供的为准!
 */
#define ZZW180WBS   "ZZW180WBS" /* 1.8 inch [360*360](ST77916), 中正威（QSPI） */

#if 1 /* 调试打印 */
#include <stdio.h>
#define ST77916_LOG(...)        printf(__VA_ARGS__)
#else
#define ST77916_LOG(...)
#endif

/* self is caller function parameters(lcd_mpu_driver_t *) */
#define ST77916_DELAY_MS(ms)                         lcd_mpu_driver_mdelay(self, ms)
#define ST77916_WR_CMD(cmd, seq)                     lcd_mpu_driver_wr_cmd(self, cmd, seq)
#define ST77916_WR_DATA(data, bytes)                 lcd_mpu_driver_wr_data(self, data, bytes)
#define ST77916_RD_DATA(data, bytes)                 lcd_mpu_driver_rd_data(self, data, bytes)

/* QSPI 读写指令及时序须查看 COG 规格书设定以下宏 */
#include "dev_lcd_mpu_qspi.h"
//LCD_QSPI_SEQ_SET_WR(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount)
//LCD_QSPI_SEQ_SET_RD(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount)

//Command write mode: 
// Write Cycle Sequence: 
//PP  : Command - 02h(1_line)    24-bit address [0x00 cmd 00](1_line)    data(1_line)
// Write GRAM(0x2C/3C): 
//PP2O: Command - A2h(1_line)    24-bit address [0x00 cmd 00](1_line)    data(2_line)
//PP4O: Command - 32h(1_line)    24-bit address [0x00 cmd 00](1_line)    data(4_line)
//PP4O: Command - 38h(1_line)    24-bit address [0x00 cmd 00](4_line)    data(4_line)
#define ST77916_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes)          LCD_QSPI_SEQ_SET_WR(seq, 0x02, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 1, bytes)
#define ST77916_QSPI_SEQ_SET_WR_L12(seq, cmd, bytes)          LCD_QSPI_SEQ_SET_WR(seq, 0xA2, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 2, bytes)
#define ST77916_QSPI_SEQ_SET_WR_L14(seq, cmd, bytes)          LCD_QSPI_SEQ_SET_WR(seq, 0x32, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 4, bytes)
#define ST77916_QSPI_SEQ_SET_WR_L44(seq, cmd, bytes)          LCD_QSPI_SEQ_SET_WR(seq, 0x38, 1, (cmd << 8) & 0xFF00, 4, 24, 0, 0, 0, 0, 4, bytes)

//Read command mode: (IO0 输出, IO1 输入)
//FASTREAD : Command - 0Bh(1_line)    24-bit address [0x00 cmd 00](1_line)    8 Dummy Bits(1_line)    data(1_line)
#define ST77916_QSPI_SEQ_SET_RD(seq, cmd, bytes)              LCD_QSPI_SEQ_SET_RD(seq, 0x0B, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 8, 1, bytes)

//选择写入 GRAM 的时序(1 / 2 / 4 line)
#define ST77916_QSPI_SEQ_SET_WR_GRAM(seq, cmd, bytes)         ST77916_QSPI_SEQ_SET_WR_L44(seq, cmd, bytes)

/* QSPI interface LTO */
#include "app_cfg.h"
#if defined(CFG_LCD_IF_QSPI) && CFG_LCD_IF != CFG_LCD_IF_QSPI
#undef ST77916_QSPI_SEQ_SET_WR_L11
#undef ST77916_QSPI_SEQ_SET_WR_L12
#undef ST77916_QSPI_SEQ_SET_WR_L14
#undef ST77916_QSPI_SEQ_SET_WR_L44
#undef ST77916_QSPI_SEQ_SET_RD

#define ST77916_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes)
#define ST77916_QSPI_SEQ_SET_WR_L12(seq, cmd, bytes)
#define ST77916_QSPI_SEQ_SET_WR_L14(seq, cmd, bytes)
#define ST77916_QSPI_SEQ_SET_WR_L44(seq, cmd, bytes)
#define ST77916_QSPI_SEQ_SET_RD(seq, cmd, bytes)
#endif

/* QSPI extra expand */
#define ST77916_WR_CMD_SEQ(cmd, seq, bytes)         do {                                              \
                                                        ST77916_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes); \
                                                        ST77916_WR_CMD(cmd, seq);                     \
                                                    } while (0)
#define ST77916_WR_DATA_VA(...)                     do {                                     \
                                                        uint8_t temp[] = {__VA_ARGS__};      \
                                                        ST77916_WR_DATA(temp, sizeof(temp)); \
                                                    } while (0)
//eg: ST77916_WR_CMD_DATA_VA(&seq, cmd, data_1, data_2, data_3, ...)
#define ST77916_WR_CMD_DATA_VA(seq, cmd, ...)       do {                                                     \
                                                        uint8_t data[] = {__VA_ARGS__};                      \
                                                        ST77916_QSPI_SEQ_SET_WR_L11(seq, cmd, sizeof(data)); \
                                                        ST77916_WR_CMD(cmd, seq);                            \
                                                        ST77916_WR_DATA(data, sizeof(data));                 \
                                                    } while (0)

/**
 * @brief 上电初始化时序以配置屏幕参数
 */
void st77916_timseq_init(lcd_mpu_driver_t *self, lcd_mpu_cfg_t *cfg)
{
    if (!(cfg && self && self->hw_rst_gpio_set && self->wr_cmd && self->wr_data && self->mdelay)) {
        ST77916_LOG("[%s] parm error: [hw_rst_gpio_set / write_cmd / write_data / mdelay] is invaild!\r\n", __FUNCTION__);
        //for (;;) __NOP();
    }
    //ST77916_DELAY_MS(120); //Wait for the power supply to stabilize

    /* RST */
    self->hw_rst_gpio_set(1); //H
    ST77916_DELAY_MS(1);
    self->hw_rst_gpio_set(0); //L
    ST77916_DELAY_MS(20);
    self->hw_rst_gpio_set(1); //H
    ST77916_DELAY_MS(120);

    /************* Start Initial Sequence **********/
#if 1
    lcd_qspi_seq_t seq;
    #define WR_CMD_DATA_VA(cmd, ...)     ST77916_WR_CMD_DATA_VA(&seq, cmd, __VA_ARGS__)
    
#if 1
    WR_CMD_DATA_VA(0xF0, 0x08);
    WR_CMD_DATA_VA(0xF2, 0x08);
    WR_CMD_DATA_VA(0x9B, 0x51);
    WR_CMD_DATA_VA(0x86, 0x53);
    WR_CMD_DATA_VA(0xF2, 0x80);
    WR_CMD_DATA_VA(0xF0, 0x00);
    WR_CMD_DATA_VA(0xF0, 0x01);
    WR_CMD_DATA_VA(0xF1, 0x01);
    WR_CMD_DATA_VA(0xB0, 0x54);

    WR_CMD_DATA_VA(0xB1, 0x3F);
    WR_CMD_DATA_VA(0xB2, 0x2A);
    WR_CMD_DATA_VA(0xB4, 0x46);
    WR_CMD_DATA_VA(0xB5, 0x34);
    WR_CMD_DATA_VA(0xB6, 0xD5);
    WR_CMD_DATA_VA(0xB7, 0x30);
    WR_CMD_DATA_VA(0xB8, 0x04);
    WR_CMD_DATA_VA(0xBA, 0x00);

    WR_CMD_DATA_VA(0xBB, 0x08);
    WR_CMD_DATA_VA(0xBC, 0x08);
    WR_CMD_DATA_VA(0xBD, 0x00);
    WR_CMD_DATA_VA(0xC0, 0x80);
    WR_CMD_DATA_VA(0xC1, 0x10);
    WR_CMD_DATA_VA(0xC2, 0x37);
    WR_CMD_DATA_VA(0xC3, 0x80);
    WR_CMD_DATA_VA(0xC4, 0x10);
    WR_CMD_DATA_VA(0xC5, 0x37);
    
    WR_CMD_DATA_VA(0xC6, 0xA9);
    WR_CMD_DATA_VA(0xC7, 0x41);
    WR_CMD_DATA_VA(0xC8, 0x51);
    WR_CMD_DATA_VA(0xC9, 0xA9);
    WR_CMD_DATA_VA(0xCA, 0x41);
    WR_CMD_DATA_VA(0xCB, 0x51);
    WR_CMD_DATA_VA(0xD0, 0x91);
    WR_CMD_DATA_VA(0xD1, 0x68);

    WR_CMD_DATA_VA(0xD2, 0x69);
    WR_CMD_DATA_VA(0xF5, 0x00, 0xA5);
    WR_CMD_DATA_VA(0xDD, 0x35);
    WR_CMD_DATA_VA(0xDE, 0x35);
    WR_CMD_DATA_VA(0xF1, 0x10);
    WR_CMD_DATA_VA(0xF0, 0x00);
    WR_CMD_DATA_VA(0xF0, 0x02);

    WR_CMD_DATA_VA(0xE0, 
    0x70, 0x09, 0x12, 0x0C, 
    0x0B, 0x27, 0x38, 0x54,
    0x4E, 0x19, 0x15, 0x15,
    0x2C, 0x2F);
    WR_CMD_DATA_VA(0xE1,
    0x70, 0x08, 0x11, 0x0C,
    0x0B, 0x27, 0x38, 0x43,
    0x4C, 0x18, 0x14, 0x14,
    0x2B, 0x2D);
    WR_CMD_DATA_VA(0xF0, 0x10);
    WR_CMD_DATA_VA(0xF3, 0x10);
    WR_CMD_DATA_VA(0xE0, 0x0A);
    WR_CMD_DATA_VA(0xE1, 0x00);
    WR_CMD_DATA_VA(0xE2, 0x0B);
    WR_CMD_DATA_VA(0xE3, 0x00);
    WR_CMD_DATA_VA(0xE4, 0xE0);
    WR_CMD_DATA_VA(0xE5, 0x06);

    WR_CMD_DATA_VA(0xE6, 0x21);
    WR_CMD_DATA_VA(0xE7, 0x00);
    WR_CMD_DATA_VA(0xE8, 0x05);
    WR_CMD_DATA_VA(0xE9, 0x82);
    WR_CMD_DATA_VA(0xEA, 0xDF);
    WR_CMD_DATA_VA(0xEB, 0x89);
    WR_CMD_DATA_VA(0xEC, 0x20);
    WR_CMD_DATA_VA(0xED, 0x14);
    WR_CMD_DATA_VA(0xEE, 0xFF);
    WR_CMD_DATA_VA(0xEF, 0x00);
    WR_CMD_DATA_VA(0xF8, 0xFF);
    WR_CMD_DATA_VA(0xF9, 0x00);
    WR_CMD_DATA_VA(0xFA, 0x00);
    WR_CMD_DATA_VA(0xFB, 0x30);
    WR_CMD_DATA_VA(0xFC, 0x00);
    WR_CMD_DATA_VA(0xFD, 0x00);
    WR_CMD_DATA_VA(0xFE, 0x00);
    WR_CMD_DATA_VA(0xFF, 0x00);
    WR_CMD_DATA_VA(0x60, 0x42);
    WR_CMD_DATA_VA(0x61, 0xE0);
    WR_CMD_DATA_VA(0x62, 0x40);
    WR_CMD_DATA_VA(0x63, 0x40);
    WR_CMD_DATA_VA(0x64, 0x02);
    WR_CMD_DATA_VA(0x65, 0x00);
    WR_CMD_DATA_VA(0x66, 0x40);
    WR_CMD_DATA_VA(0x67, 0x03);
    WR_CMD_DATA_VA(0x68, 0x00);
    WR_CMD_DATA_VA(0x69, 0x00);
    WR_CMD_DATA_VA(0x6A, 0x00);
    WR_CMD_DATA_VA(0x6B, 0x00);

    WR_CMD_DATA_VA(0x70, 0x42);
    WR_CMD_DATA_VA(0x71, 0xE0);
    WR_CMD_DATA_VA(0x72, 0x40);
    WR_CMD_DATA_VA(0x73, 0x40);
    WR_CMD_DATA_VA(0x74, 0x02);
    WR_CMD_DATA_VA(0x75, 0x00);
    WR_CMD_DATA_VA(0x76, 0x40);
    WR_CMD_DATA_VA(0x77, 0x03);
    WR_CMD_DATA_VA(0x78, 0x00);
    WR_CMD_DATA_VA(0x79, 0x00);
    WR_CMD_DATA_VA(0x7A, 0x00);
    WR_CMD_DATA_VA(0x7B, 0x00);
    WR_CMD_DATA_VA(0x80, 0x38);
    WR_CMD_DATA_VA(0x81, 0x00);
    WR_CMD_DATA_VA(0x82, 0x04);
    WR_CMD_DATA_VA(0x83, 0x02);
    WR_CMD_DATA_VA(0x84, 0xDC);
    WR_CMD_DATA_VA(0x85, 0x00);
    WR_CMD_DATA_VA(0x86, 0x00);
    WR_CMD_DATA_VA(0x87, 0x00);
    WR_CMD_DATA_VA(0x88, 0x38);
    WR_CMD_DATA_VA(0x89, 0x00);
    WR_CMD_DATA_VA(0x8A, 0x06);
    WR_CMD_DATA_VA(0x8B, 0x02);
    WR_CMD_DATA_VA(0x8C, 0xDE);
    WR_CMD_DATA_VA(0x8D, 0x00);
    WR_CMD_DATA_VA(0x8E, 0x00);
    WR_CMD_DATA_VA(0x8F, 0x00);
    WR_CMD_DATA_VA(0x90, 0x38);
    WR_CMD_DATA_VA(0x91, 0x00);

    WR_CMD_DATA_VA(0x92, 0x08);
    WR_CMD_DATA_VA(0x93, 0x02);
    WR_CMD_DATA_VA(0x94, 0xE0);
    WR_CMD_DATA_VA(0x95, 0x00);
    WR_CMD_DATA_VA(0x96, 0x00);
    WR_CMD_DATA_VA(0x97, 0x00);
    WR_CMD_DATA_VA(0x98, 0x38);
    WR_CMD_DATA_VA(0x99, 0x00);
    WR_CMD_DATA_VA(0x9A, 0x0A);
    WR_CMD_DATA_VA(0x9B, 0x02);
    WR_CMD_DATA_VA(0x9C, 0xE2);
    WR_CMD_DATA_VA(0x9D, 0x00);
    WR_CMD_DATA_VA(0x9E, 0x00);
    WR_CMD_DATA_VA(0x9F, 0x00);
    WR_CMD_DATA_VA(0xA0, 0x38);
    WR_CMD_DATA_VA(0xA1, 0x00);
    WR_CMD_DATA_VA(0xA2, 0x03);
    WR_CMD_DATA_VA(0xA3, 0x02);
    WR_CMD_DATA_VA(0xA4, 0xDB);
    WR_CMD_DATA_VA(0xA5, 0x00);
    WR_CMD_DATA_VA(0xA6, 0x00);
    WR_CMD_DATA_VA(0xA7, 0x00);
    WR_CMD_DATA_VA(0xA8, 0x38);
    WR_CMD_DATA_VA(0xA9, 0x00);
    WR_CMD_DATA_VA(0xAA, 0x05);
    WR_CMD_DATA_VA(0xAB, 0x02);
    WR_CMD_DATA_VA(0xAC, 0xDD);
    WR_CMD_DATA_VA(0xAD, 0x00);

    WR_CMD_DATA_VA(0xAE, 0x00);
    WR_CMD_DATA_VA(0xAF, 0x00);
    WR_CMD_DATA_VA(0xB0, 0x38);
    WR_CMD_DATA_VA(0xB1, 0x00);
    WR_CMD_DATA_VA(0xB2, 0x07);
    WR_CMD_DATA_VA(0xB3, 0x02);
    WR_CMD_DATA_VA(0xB4, 0xDF);
    WR_CMD_DATA_VA(0xB5, 0x00);
    WR_CMD_DATA_VA(0xB6, 0x00);
    WR_CMD_DATA_VA(0xB7, 0x00);
    WR_CMD_DATA_VA(0xB8, 0x38);
    WR_CMD_DATA_VA(0xB9, 0x00);
    WR_CMD_DATA_VA(0xBA, 0x09);
    WR_CMD_DATA_VA(0xBB, 0x02);
    WR_CMD_DATA_VA(0xBC, 0xE1);
    WR_CMD_DATA_VA(0xBD, 0x00);
    WR_CMD_DATA_VA(0xBE, 0x00);
    WR_CMD_DATA_VA(0xBF, 0x00);
    WR_CMD_DATA_VA(0xC0, 0x22);
    WR_CMD_DATA_VA(0xC1, 0xAA);
    WR_CMD_DATA_VA(0xC2, 0x65);
    WR_CMD_DATA_VA(0xC3, 0x74);
    WR_CMD_DATA_VA(0xC4, 0x47);
    WR_CMD_DATA_VA(0xC5, 0x56);
    WR_CMD_DATA_VA(0xC6, 0x00);
    WR_CMD_DATA_VA(0xC7, 0x88);
    WR_CMD_DATA_VA(0xC8, 0x99);
    WR_CMD_DATA_VA(0xC9, 0x33);
    WR_CMD_DATA_VA(0xD0, 0x11);

    WR_CMD_DATA_VA(0xD1, 0xAA);
    WR_CMD_DATA_VA(0xD2, 0x65);
    WR_CMD_DATA_VA(0xD3, 0x74);
    WR_CMD_DATA_VA(0xD4, 0x47);
    WR_CMD_DATA_VA(0xD5, 0x56);
    WR_CMD_DATA_VA(0xD6, 0x00);
    WR_CMD_DATA_VA(0xD7, 0x88);
    WR_CMD_DATA_VA(0xD8, 0x99);
    WR_CMD_DATA_VA(0xD9, 0x33);
    WR_CMD_DATA_VA(0xF3, 0x01);
    WR_CMD_DATA_VA(0xF0, 0x00);
    WR_CMD_DATA_VA(0xF0, 0x01);
    WR_CMD_DATA_VA(0xF1, 0x01);
    WR_CMD_DATA_VA(0xA0, 0x0B);
    WR_CMD_DATA_VA(0xA3, 0x2A);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);

    WR_CMD_DATA_VA(0xA3, 0x2B);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x2C);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x2D);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x2E);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x2F);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x30);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x31);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x32);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA3, 0x33);
    WR_CMD_DATA_VA(0xA5, 0xC3);
    
    ST77916_DELAY_MS(1);
    
    WR_CMD_DATA_VA(0xA0, 0x09);
    WR_CMD_DATA_VA(0xF1, 0x10);
    WR_CMD_DATA_VA(0xF0, 0x00);
    WR_CMD_DATA_VA(0x2A
    , 0x00, 0x00
    , 0x01, 0x67);
    WR_CMD_DATA_VA(0x2B
    , 0x01, 0x68
    , 0x01, 0x68);
    WR_CMD_DATA_VA(0x4D, 0x00);

    WR_CMD_DATA_VA(0x4E, 0x00);
    WR_CMD_DATA_VA(0x4F, 0x00);
    WR_CMD_DATA_VA(0x4C, 0x01);
    
    ST77916_DELAY_MS(10);
    
    WR_CMD_DATA_VA(0x4C, 0x00);
    WR_CMD_DATA_VA(0x2A
    , 0x00, 0x00
    , 0x01, 0x67);
    WR_CMD_DATA_VA(0x2B
    , 0x00, 0x00
    , 0x01, 0x67);
#endif

    WR_CMD_DATA_VA(0x3A, 0x55); //add -240821

    WR_CMD_DATA_VA(0x21);

    WR_CMD_DATA_VA(0x11);
    ST77916_DELAY_MS(120);
    WR_CMD_DATA_VA(0x29);
#endif

//  WR_CMD_DATA_VA(0x1C); //进入自测模式
}

/**
 * @brief  设置显示窗口
 * @param  xs\xe\ys\ye : 窗口区域坐标(以 0 为起点)
 * @retval \
 */
void st77916_set_disp_area(lcd_mpu_driver_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    lcd_qspi_seq_t seq;
    ST77916_WR_CMD_DATA_VA(&seq, 0x2A, (xs >> 8) & 0xFF, xs & 0xFF, (xe >> 8) & 0xFF, xe & 0xFF );
    ST77916_WR_CMD_DATA_VA(&seq, 0x2B, (ys >> 8) & 0xFF, ys & 0xFF, (ye >> 8) & 0xFF, ye & 0xFF );
    
    uint32_t pixels = (xe - xs + 1) * (ye - ys + 1);
    ST77916_QSPI_SEQ_SET_WR_GRAM(&seq, 0x2C, pixels << 1); //RGB565: 1 pixel = 2 Bytes
    ST77916_WR_CMD(0x2C, &seq);
}

/**
 * @brief  设置屏幕旋转
 * @param  direction : 旋转方向(自定义实现)
 * @retval \
 */
void st77916_set_rotate(lcd_mpu_driver_t *self, uint8_t direction)
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
    ST77916_WR_CMD_DATA_VA(&seq, 0x36, (h4 << 4) | l4);
}

/**
 * @brief  设置屏幕背光
 * @param  val : 期望设置亮度
 * @retval 实际设置亮度
 */
uint32_t st77916_set_backlight(lcd_mpu_driver_t *self, uint32_t val)
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
void st77916_set_power(lcd_mpu_driver_t *self, uint8_t mode)
{
    /* 一些 COG 也支持以发送指令的方式来进入低功耗模式,
     * 亦可直接使用 GPIO 控制 TFT-LCD 的 Power In
     */
}
