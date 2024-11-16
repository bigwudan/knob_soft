/**
 *******************************************************************************************************************************************
 * @file        dev_uart.h
 * @brief       串口打印调试日志
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
#ifndef __DEV_UART_H__
#define __DEV_UART_H__

#include <stdint.h>
#include "SWM221.h"

//__WEAK
extern void uart_debug_handler_readbyte_hook(uint8_t chr);
extern void uart_debug_handler_timeout_hook(void);

/**
 * @brief 配置串口打印调试日志的端口
 */
void uart_debug_port_init(void);

/**
 * @brief 初始化用作打印调试日志的串口
 */
void uart_debug_init(void);

/**
 * @brief  串口发送数据
 * @param  uart_x : 串口号
 * @param  data   : 数据源
 * @param  bytes  : 数据源大小 / Bytes
 * @retval /
 */
void uart_send_msg(UART_TypeDef *uart_x, const uint8_t *data, uint32_t bytes);

#endif //__DEV_UART_H__
