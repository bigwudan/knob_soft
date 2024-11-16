/**
 *******************************************************************************************************************************************
 * @file        dev_uart.c
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
#include "dev_uart.h"

/** 用作调试日志打印的串口号 */
#define DEBUG_UART_X              UART1

/** 用作调试日志打印的串口波特率（data-8, stop-1, no parity） */
#define DEBUG_UART_BAUD           (115200)

/**
 * @brief 配置串口打印调试日志的端口
 */
void uart_debug_port_init(void)
{ 
    PORT_Init(PORTB, PIN8, PORTB_PIN8_UART1_RX, 1);
    PORT_Init(PORTB, PIN7, PORTB_PIN7_UART1_TX, 0);
}

/**
 * @brief 初始化用作打印调试日志的串口
 */
void uart_debug_init(void)
{
    uart_debug_port_init();

    UART_InitStructure UART_initStruct;
    UART_initStruct.Baudrate = DEBUG_UART_BAUD;
    UART_initStruct.DataBits = UART_DATA_8BIT;
    UART_initStruct.Parity = UART_PARITY_NONE;
    UART_initStruct.StopBits = UART_STOP_1BIT;
    UART_initStruct.RXThreshold = 3;
    UART_initStruct.RXThresholdIEn = 1;
    UART_initStruct.TXThreshold = 3;
    UART_initStruct.TXThresholdIEn = 0;
    UART_initStruct.TimeoutTime = 10;
    UART_initStruct.TimeoutIEn = 1;
    UART_Init(DEBUG_UART_X, &UART_initStruct);
    UART_Open(DEBUG_UART_X);
}

__WEAK 
void uart_debug_handler_readbyte_hook(uint8_t chr)
{
}
__WEAK 
void uart_debug_handler_timeout_hook(void)
{
}

void UART1_Handler(void)
{
	uint32_t chr = 0;
	if (UART_INTStat(DEBUG_UART_X, UART_IT_RX_THR | UART_IT_RX_TOUT))
	{
		while (UART_IsRXFIFOEmpty(DEBUG_UART_X) == 0)
		{
			if (UART_ReadByte(DEBUG_UART_X, &chr) == 0)
			{
				uart_debug_handler_readbyte_hook(chr & 0xFF);
			}
		}
		if (UART_INTStat(DEBUG_UART_X, UART_IT_RX_TOUT))
		{
			UART_INTClr(DEBUG_UART_X, UART_IT_RX_TOUT);
            uart_debug_handler_timeout_hook();
		}
	}
}

/**
 * @brief  串口发送数据
 * @param  uart_x : 串口号
 * @param  data   : 数据源
 * @param  bytes  : 数据源大小 / Bytes
 * @retval /
 */
void uart_send_msg(UART_TypeDef *uart_x, const uint8_t *data, uint32_t bytes)
{
    for (uint32_t i = 0; i < bytes; ++i)
    {
        while (UART_IsTXFIFOFull(uart_x)) __NOP();
        UART_WriteByte(uart_x, data[i]);
        while (UART_IsTXBusy(uart_x)) __NOP();
    }
}
