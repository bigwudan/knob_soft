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

#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp_spi_bus.h"
#include "bsp_spi_flash.h"

#include "delay.h"
#include "sys.h"


/* 提供给其他C文件调用的函数 */
int SystemClock_Config(void);

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
#define SF_CS_CLK_ENABLE() 			0
#define SF_CS_GPIO					GPIOB
#define SF_CS_PIN					GPIO_Pin_12
#define SF_CS_0()					GPIO_ResetBits(SF_CS_GPIO,SF_CS_PIN)
#define SF_CS_1()					GPIO_SetBits(SF_CS_GPIO,SF_CS_PIN)

/* SPI SCK时钟 *******/
#define RCC_SCK_CLK_ENABLE()  		0
#define PORT_SCK					GPIOB
#define PIN_SCK						GPIO_Pin_13
#define SCK_0()						GPIO_ResetBits(PORT_SCK,PIN_SCK)
#define SCK_1()						GPIO_SetBits(PORT_SCK,PIN_SCK)

/* SPI MOSI *********/
#define RCC_MOSI_CLK_ENABLE()  		0
#define PORT_MOSI					GPIOB
#define PIN_MOSI					GPIO_Pin_15
#define MOSI_0()					GPIO_ResetBits(PORT_MOSI,PIN_MOSI)
#define MOSI_1()					GPIO_SetBits(PORT_MOSI,PIN_MOSI)

/* SPI MISO *********/
#define RCC_MISO_CLK_ENABLE()  		0
#define PORT_MISO					GPIOB
#define PIN_MISO					GPIO_Pin_14
#define MISO_IS_HIGH()	        	(GPIO_ReadInputDataBit(PORT_MISO, PIN_MISO) == Bit_SET)

#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
