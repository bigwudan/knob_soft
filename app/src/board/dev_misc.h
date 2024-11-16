/**
 *******************************************************************************************************************************************
 * @file        dev_misc.h
 * @brief       低电压 / SWD 端口复用 / 外部晶振停振 检测 / boot 跳转 app
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-04   lzh          the first version
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __DEV_MISC_H__
#define __DEV_MISC_H__

#include <stdint.h>
#include <cmsis_compiler.h>

/* 低压具体阈值请以最新数据手册描述为准 */

/** 低压中断触发电平: 0 2.0v   1 2.3v   2 2.7v   3 3.0v   4 3.7v   5 4.0v   6 4.3v */
enum {
    BOD_INT_2000_MV = 0,
    BOD_INT_2300_MV,
    BOD_INT_2700_MV,
    BOD_INT_3000_MV,
    BOD_INT_3700_MV,
    BOD_INT_4000_MV,
    BOD_INT_4300_MV,
};

/** 低压复位电平: 0 1.8v   1 2.0v   2 2.5v   3 3.5v */
enum {
    BOD_RST_1800_MV = 0,
    BOD_RST_2000_MV,
    BOD_RST_2500_MV,
    BOD_RST_3500_MV,
};

/**
 * @brief  欠压检测配置
 * @note   该功能默认为常开状态, 上电后默认配置 1.65V 产生复位, 失能中断.
 * @param  rst_mv : 低电压复位阈值
 * @param  int_mv : 低电压触发阈值
 * @param  int_en : 0-失能低电压触发中断    1-使能低电压触发中断
 * @retval /
 */
void bod_config(uint8_t rst_mv, uint8_t int_mv, uint8_t int_en);

/**
 * @brief  配置 SWD 端口的功能
 * @note   SWD 端口功能切换未在固件库中开放给用户, 避免用户配置端口错误而导致无法下载, 重新上电后, SWD 端口总会自动恢复为 SWD 功能.
 * 1、SWD 端口切换为 GPIO 或 其它非 SWD 功能前必须加上 *((volatile uint32_t *)0x40000190) = 0; 
 * 2、SWD 端口恢复为 SWD 功能前必须加上 *((volatile uint32_t *)0x40000190) = 1; 
 * @param  fun_mux: 0- SWD 功能      1-除 SWD 外的功能
 * @return /
 */
void swd_port_config(uint8_t fun_mux);

/**
 * @brief 外部晶振停振检测初始化
 */
void xtal_stop_check_init(void);

__NO_RETURN void jump_to_app(uint32_t addr);
void Flash_remap(uint32_t addr);

#endif //__DEV_MISC_H__

