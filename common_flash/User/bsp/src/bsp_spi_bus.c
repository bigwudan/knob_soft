/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动
*	文件名称 : bsp_spi_bus.c
*	版    本 : V1.0
*	说    明 : 模拟SPI实现
*	修改记录 :
*		版本号   日期        作者      说明
*		V1.0	2022-05-29  Eric2013   首发。
*
*	Copyright (C), 2020-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPIBus
*	功能说明: 配置SPI总线。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{	
#if 0	
	/* 配置 SPI总线GPIO : CS SCK MOSI MISO */
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
		
	/* 时钟 */
	SF_CS_CLK_ENABLE();
	RCC_SCK_CLK_ENABLE();
	RCC_MOSI_CLK_ENABLE();
	RCC_MISO_CLK_ENABLE();

	/* SPI CS ##############################*/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	/* 设置推挽输出 */
	GPIO_InitStruct.Pull = GPIO_NOPULL;			/* 上下拉电阻不使能 */
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;  	/* GPIO速度等级 */	
	GPIO_InitStruct.Pin = SF_CS_PIN;	
	HAL_GPIO_Init(SF_CS_GPIO, &GPIO_InitStruct);	
	
	/* SPI SCK ##############################*/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = PIN_SCK;
	HAL_GPIO_DeInit(PORT_SCK, PIN_SCK);
	HAL_GPIO_Init(PORT_SCK, &GPIO_InitStruct);

	/* SPI MISO ##############################*/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = PIN_MISO;
	HAL_GPIO_DeInit(PORT_MISO, PIN_MISO);
	HAL_GPIO_Init(PORT_MISO, &GPIO_InitStruct);
	
	/* SPI MOSI ##############################*/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = PIN_MOSI;
	HAL_GPIO_DeInit(PORT_MOSI, PIN_MOSI);
	HAL_GPIO_Init(PORT_MOSI, &GPIO_InitStruct);
#endif	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SpiDelay
*	功能说明: 时序延迟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiDelay(void)
{
#if 0
	uint32_t i;

	/*
		延迟5时， F407 (168MHz主频） GPIO模拟，实测 SCK 周期 = 480ns (大约2M)
	*/
	for (i = 0; i < 5; i++);
#else
	/*
		不添加延迟语句， F407 (168MHz主频） GPIO模拟，实测 SCK 周期 = 200ns (大约5M)
	*/
//	__NOP();
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiWrite0
*	功能说明: 向SPI总线发送一个字节。SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiWrite0(uint8_t _ucByte)
{
	uint8_t i;

//	for(i = 0; i < 8; i++)
//	{
//		if (_ucByte & 0x80)
//		{
//			MOSI_1();
//		}
//		else
//		{
//			MOSI_0();
//		}
//		bsp_spiDelay();
//		SCK_1();
//		_ucByte <<= 1;
//		bsp_spiDelay();
//		SCK_0();
//	}
//	bsp_spiDelay();
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead0
*	功能说明: 从SPI总线接收8个bit数据。 SCK上升沿采集数据, SCK空闲时为低电平。
*	形    参: 无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t bsp_spiRead0(void)
{
	uint8_t i;
	uint8_t read = 0;

//	for (i = 0; i < 8; i++)
//	{
//		read = read<<1;

//		if (MISO_IS_HIGH())
//		{
//			read++;
//		}
//		SCK_1();
//		bsp_spiDelay();
//		SCK_0();
//		bsp_spiDelay();
//	}
	return read;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
