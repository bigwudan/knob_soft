/*
*********************************************************************************************************
*
*	模块名称 : BSP配置
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2022-05-29  Eric203    正式发布
*
*	Copyright (C), 2020-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp_spi_bus.h"
#include "bsp_spi_flash.h"
#include "SWM221.h"

/* 提供给其他C文件调用的函数 */
int SystemClock_Config(void);


void board_init(void);

/*
*********************************************************************************************************
*	                             SPI Flash 操作地址
*********************************************************************************************************
*/
#define SPI_FLASH_MEM_ADDR         0xC0000000    /* 串行操作，随意命名个地址即可 */

/*
*********************************************************************************************************
*	                             SPI Flash 命令
*********************************************************************************************************
*/
#define CMD_PAGEWR    0x02  		/* 页编程 Page Program */
#define CMD_DISWR	  0x04			/* 禁止写 Write Disable */
#define CMD_WREN      0x06			/* 写使能命令 Write Enable */
#define CMD_READ      0x03  		/* 读数据区命令 Read Data */
#define CMD_RDSR      0x05			/* 读状态寄存器命令 Read Status Register-1 */
#define CMD_SE        0x20			/* 擦除扇区命令 Sector Erase */
#define CMD_BE        0xC7			/* 整片擦除命令 Chip Erase */

#define WIP_FLAG      0x01			/* 状态寄存器中的正在编程标志（WIP) Write Status Register */

/*
*********************************************************************************************************
*	                             SPI Flash 命令
*********************************************************************************************************
*/
/* SPI CS片选 *******/
#define SF_CS_CLK_ENABLE() 			__HAL_RCC_GPIOD_CLK_ENABLE()
#define SF_CS_GPIO					GPIOD
#define SF_CS_PIN					GPIO_PIN_13
#define SF_CS_0()					SF_CS_GPIO->BSRR = ((uint32_t)SF_CS_PIN << 16U) 
#define SF_CS_1()					SF_CS_GPIO->BSRR = SF_CS_PIN

/* SPI SCK时钟 *******/
#define RCC_SCK_CLK_ENABLE()  		__HAL_RCC_GPIOB_CLK_ENABLE()
#define PORT_SCK					GPIOB
#define PIN_SCK						GPIO_PIN_3
#define SCK_0()						PORT_SCK->BSRR = ((uint32_t)PIN_SCK << 16U)
#define SCK_1()						PORT_SCK->BSRR = PIN_SCK

/* SPI MOSI *********/
#define RCC_MOSI_CLK_ENABLE()  		__HAL_RCC_GPIOB_CLK_ENABLE()
#define PORT_MOSI					GPIOB
#define PIN_MOSI					GPIO_PIN_5
#define MOSI_0()					PORT_MOSI->BSRR = ((uint32_t)PIN_MOSI << 16U)
#define MOSI_1()					PORT_MOSI->BSRR = PIN_MOSI

/* SPI MISO *********/
#define RCC_MISO_CLK_ENABLE()  		__HAL_RCC_GPIOB_CLK_ENABLE()
#define PORT_MISO					GPIOB
#define PIN_MISO					GPIO_PIN_4
#define MISO_IS_HIGH()	        	(0)

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
