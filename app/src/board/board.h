/**
 *******************************************************************************************************************************************
 * @file        board.h
 * @brief       板级硬件外设相关
 * @since       Change Logs:
 * Date         Author       Notes
 * 2022-12-08   lzh          the first version
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __BOARD_H__
#define __BOARD_H__

#include "SWM221.h"

#include "dev_systick.h"
#include "dev_uart.h"
#include "dev_misc.h"
#include "dev_sleep_stop.h"
#include "dev_key.h"
#include "dev_lcd_mpu.h"
#include "dev_tp.h"
#include "dev_qspi_flash.h"

/**
 * @brief 初始化所有的硬件设备, 该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量(只需要调用一次)
 */
void board_init(void);

#endif //__BOARD_H__
