/*
*********************************************************************************************************
*
*	ģ������ : BSP����
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			   bsp = Borad surport packet �弶֧�ְ�
*	�޸ļ�¼ :
*		�汾��  ����         ����       ˵��
*		V1.0    2022-05-29  Eric203    ��ʽ����
*
*	Copyright (C), 2020-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
#include "bsp_spi_bus.h"
#include "bsp_spi_flash.h"
#include "SWM221.h"

/* �ṩ������C�ļ����õĺ��� */
int SystemClock_Config(void);


void board_init(void);

/*
*********************************************************************************************************
*	                             SPI Flash ������ַ
*********************************************************************************************************
*/
#define SPI_FLASH_MEM_ADDR         0xC0000000    /* ���в�����������������ַ���� */

/*
*********************************************************************************************************
*	                             SPI Flash ����
*********************************************************************************************************
*/
#define CMD_PAGEWR    0x02  		/* ҳ��� Page Program */
#define CMD_DISWR	  0x04			/* ��ֹд Write Disable */
#define CMD_WREN      0x06			/* дʹ������ Write Enable */
#define CMD_READ      0x03  		/* ������������ Read Data */
#define CMD_RDSR      0x05			/* ��״̬�Ĵ������� Read Status Register-1 */
#define CMD_SE        0x20			/* ������������ Sector Erase */
#define CMD_BE        0xC7			/* ��Ƭ�������� Chip Erase */

#define WIP_FLAG      0x01			/* ״̬�Ĵ����е����ڱ�̱�־��WIP) Write Status Register */

/*
*********************************************************************************************************
*	                             SPI Flash ����
*********************************************************************************************************
*/
/* SPI CSƬѡ *******/
#define SF_CS_CLK_ENABLE() 			__HAL_RCC_GPIOD_CLK_ENABLE()
#define SF_CS_GPIO					GPIOD
#define SF_CS_PIN					GPIO_PIN_13
#define SF_CS_0()					SF_CS_GPIO->BSRR = ((uint32_t)SF_CS_PIN << 16U) 
#define SF_CS_1()					SF_CS_GPIO->BSRR = SF_CS_PIN

/* SPI SCKʱ�� *******/
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
