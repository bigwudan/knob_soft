/**
 *******************************************************************************************************************************************
 * @file        app_cfg.h
 * @brief       app global config
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * @note        本示例基于 Synwit 官方的 SWM221 系列 DEMO 屏驱演示板,
 * 涵盖多种规格的 TFT-LCD (MPU/MCU 接口) 屏驱应用, 并附带常用的 CTP 驱动,
 * 快速上手本示例敏捷点亮屏幕显示的方式: 
 * 1、购买本示例枚举的已适配屏幕型号, 屏幕模组可联系 Synwit 官方/代理商获取购买链接或上电商平台搜索.
 * 2、若用户手上现有一块不在本示例枚举的已适配型号内的屏幕, 可自行在本示例上添加补充对应 TFT-LCD 模组的 显示/触摸 的驱动程序.
 * @remark      由于市面上的屏幕五花八门, 人力有时尽, 若用户使用的 TFT-LCD 模组不在原厂已调试过的型号范围内, 
 * 请着重确认这几点: 屏幕支持的接口时序、FPC 外接线序、分辨率规格.
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __APP_CFG_H__
#define __APP_CFG_H__
// <<< Use Configuration Wizard in Context Menu >>> 
/* How many bytes of storage are needed for 1 pixel */
#define CFG_LCD_DEPTH_RGB232              ( 1 ) /**< 8 bit(reserve, monochrome 1 => 8) */
#define CFG_LCD_DEPTH_RGB565              ( 2 ) /**< RGB565 = 16 bit */
#define CFG_LCD_DEPTH_RGB888              ( 3 ) /**< RGB888 = 24 bit(reserve) */
#define CFG_LCD_DEPTH_ARGB888             ( 4 ) /**< ARGB888 = 32 bit(reserve) */

#define CFG_LCD_IF_QSPI                   ( 0 ) /**< [Serial] Quad SPI, support DMA(Asnyc / Sync) */
#define CFG_LCD_IF_I8080_8                ( 1 ) /**< [Parallel] Hardware LCDC, support DMA(Asnyc / Sync) */
#define CFG_LCD_IF_SPI_8                  ( 2 ) /**< [Serial] line-4, support DMA(Asnyc / Sync) */
#define CFG_LCD_IF_SPI_9                  ( 3 ) /**< [Serial] line-3, Not recommend, it is slow */

// <h> LCD Configuration
// <i> 请根据实际使用 TFT-LCD 模组型号填写相应信息
// <i> 如使用本示例未适配过的型号, 用户可自行添加补充对应的驱动程序并添加至工程.
//   <s> LCD name (区分大小写)
//   <i> Note:
//   <i> 若屏内 COG 驱动芯片支持横竖屏转换(对调分辨率), 当用户有需求时可手动在 LCD 初始化后显式调用 lcd_mpu_set_rotate() 接口进行旋转, 旋转方向请参考具体驱动 lcd_xxx.c 实现;
//   <i> ZZW180WBS   |  1.8 inch [360*360](ST77916), 中正威（QSPI）
//   <i> H043A28     |  4.3 inch [480*272](NV3041), 鑫洪泰（QSPI / i8080_8bit / SPI-4/3）
//   <i> ZJY240IT008 |  2.4 inch [240*320](ST7789), 中景园（i8080_16/8bit / SPI-4/3）
//   <o> LCD width pixel <0-480>
//   <i> LCD 分辨率-行(0~480]
//   <o1> LCD height pixel <0-480>
//   <i> LCD 分辨率-列(0~480]
//   <o2> LCD color depth <2=> RGB565  
//   <i> RGB565: default
//   <o3> LCD interface <0=> QSPI  <1=> i8080 8bit  <2=> spi 8bit  <3=> spi 9bit
//   <i> Quad_SPI    : [Serial] Quad SPI, support DMA(Asnyc / Sync), note: 屏须自带 GRAM
//   <i> i8080 8bit  : [Parallel] Hardware LCDC, support DMA(Asnyc / Sync)
//   <i> spi 8bit    : [Serial] line-4, support DMA(Asnyc / Sync)
//   <i> spi 9bit    : [Serial] line-3, Not recommend, it is slow
// </h>
#define CFG_LCD_NAME                       "ZZW180WBS"
#define CFG_LCD_HDOT                       ( 360 )
#define CFG_LCD_VDOT                       ( 360 )
#define CFG_LCD_DEPTH                      ( 2 ) //(reserve unused)通常 RGB565 在小尺寸屏上的色彩表现已足够绝大多数应用场景
#define CFG_LCD_IF                         ( 0 ) //interface

// <h> TP Configuration
// <i> 请根据实际使用 TFT-LCD 模组型号填写相应信息
// <i> 如使用本示例未适配过的型号, 用户可自行添加补充对应的驱动程序并添加至工程.
//   <s> TP name (区分大小写)
//   <i> CHSC6413         |  思迈微 系列模组
//   <i> CHSC6540         |  优奕视界 系列模组
//   <i> GT911            |  鑫洪泰 H043A28
//   <i> FT6336           |  No instances available
//   <i> CST826           |  No instances available
//   <i> CST816D          |  中正威 ZZW180WBS
//   <i> FT5206           |  No instances available
//   <i> CST328           |  No instances available
//   <i> Tango_C32        |  No instances available
//   <i> ILI2117A         |  No instances available
#define CFG_TP_NAME                        "CST816D" //允许不定义该 TP 宏以应对某些屏幕不带触摸
//   <!c1> TP enable
//   <i> 是否启用 TP (default: true)
#undef CFG_TP_NAME
//   </c>
// </h>
// <<< end of configuration section >>> 
#if !(defined(CFG_LCD_NAME)   \
    && defined(CFG_LCD_HDOT)  \
    && defined(CFG_LCD_VDOT)  \
    && defined(CFG_LCD_DEPTH) \
    && defined(CFG_LCD_IF))
# error "Please define the [CFG_LCD_NAME / CFG_LCD_HDOT / CFG_LCD_VDOT / CFG_LCD_DEPTH / CFG_LCD_IF]"
#endif
//#ifndef CFG_TP_NAME
//# warning "No define the [CFG_TP_NAME], You cannot use touch input device!"
//#endif
#endif //__APP_CFG_H__
