/**
 *******************************************************************************************************************************************
 * @file        dev_lcd_mpu_port.c
 * @brief       LCD MPU display [port]
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * 2024-08-18   lzh          modify the import method of driver
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include "dev_lcd_mpu.h"

#define IMPORT_LCD_MPU_DRIVER(__cog_name)                                                                                                \
    extern void __DECL_LCD_NAME(__cog_name, timseq_init)(lcd_mpu_driver_t * self, lcd_mpu_cfg_t * cfg);                                  \
    extern void __DECL_LCD_NAME(__cog_name, set_disp_area)(lcd_mpu_driver_t * self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye); \
    extern void __DECL_LCD_NAME(__cog_name, set_rotate)(lcd_mpu_driver_t * self, uint8_t dir);                                           \
    extern uint32_t __DECL_LCD_NAME(__cog_name, set_backlight)(lcd_mpu_driver_t * self, uint32_t val);                                   \
    extern void __DECL_LCD_NAME(__cog_name, set_power)(lcd_mpu_driver_t * self, uint8_t mode)

/* LCD COG Driver Prototype */
IMPORT_LCD_MPU_DRIVER(st7789);
IMPORT_LCD_MPU_DRIVER(ili9488);
IMPORT_LCD_MPU_DRIVER(gx9307);
IMPORT_LCD_MPU_DRIVER(gc9307);
IMPORT_LCD_MPU_DRIVER(gc9a01);
IMPORT_LCD_MPU_DRIVER(st7365);
IMPORT_LCD_MPU_DRIVER(st77916);
IMPORT_LCD_MPU_DRIVER(nv3041);
IMPORT_LCD_MPU_DRIVER(nv3023);

#define __DECL_LCD_DRIVER_BASE(__cog_name)      \
    __DECL_LCD_NAME(__cog_name, timseq_init),   \
    __DECL_LCD_NAME(__cog_name, set_disp_area), \
    NULL, NULL, NULL, NULL

#define __DECL_LCD_DRIVER_ALL(__cog_name)       \
    __DECL_LCD_NAME(__cog_name, timseq_init),   \
    __DECL_LCD_NAME(__cog_name, set_disp_area), \
    __DECL_LCD_NAME(__cog_name, set_rotate),    \
    __DECL_LCD_NAME(__cog_name, set_backlight), \
    __DECL_LCD_NAME(__cog_name, set_power)

static const struct
{
    const char *name;
    uint16_t hres, vres;
    lcd_mpu_adapter_t adapter;
} LCD_Table[] = {
    /* 1.8 inch [360*360](ST77916), 中正威 */
    {"ZZW180WBS", 360, 360,
     __DECL_LCD_DRIVER_ALL(st77916),
     0, 0},

    /* 4.3 inch [480*272](NV3041), 鑫洪泰 */
    {"H043A28", 480, 272,
     __DECL_LCD_DRIVER_ALL(nv3041),
     0, 0},

    /* 2.4 inch [240*320](ST7789), 中景园 */
    {"ZJY240IT008", 240, 320,
     __DECL_LCD_DRIVER_ALL(st7789),
     0, 0},
};

/**
 * @brief  LCD MPU 屏适配
 * @param  adapter     : see lcd_mpu_adapter_t
 * @param  hres / vres : 物理分辨率-宽 / 高
 * @param  name        : LCD 模组型号名
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t lcd_mpu_adapter(lcd_mpu_adapter_t *adapter, uint16_t hres, uint16_t vres, const char *name)
{
    if (!adapter) {
        printf("[%s]: lcd_mpu_adapter is NULL, You must provide lcd_mpu_adapter!\r\n", __FUNCTION__);
        return 1; // assert
    }
    uint16_t i = 0;
    for (i = 0; i < sizeof(LCD_Table) / sizeof(LCD_Table[0]); ++i)
    {
        if (0 == strcmp(name, LCD_Table[i].name) 
        && ((hres == LCD_Table[i].hres && vres == LCD_Table[i].vres) 
            || (vres == LCD_Table[i].hres && hres == LCD_Table[i].vres)
            )
            ) // 允许宽高互换
        {
            break;
        }
    }
    if (i >= sizeof(LCD_Table) / sizeof(LCD_Table[0])) {
        printf("[%s]: No find [%s][%d * %d]LCD Driver of correct, You must provide your customized LCD Driver!\r\n", __FUNCTION__, name, hres, vres);
        return 2;
    }
    memcpy(adapter, &LCD_Table[i].adapter, sizeof(lcd_mpu_adapter_t));
    return 0;
}
