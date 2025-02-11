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
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  
  
  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SF_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SF_CS_GPIO, &GPIO_InitStructure);  
    
    /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = PIN_SCK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_SCK, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = PIN_MISO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
  GPIO_Init(PORT_MISO, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(PORT_MOSI, &GPIO_InitStructure);
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
#if 1
	uint32_t i;

	/*
		延迟5时， F407 (168MHz主频） GPIO模拟，实测 SCK 周期 = 480ns (大约2M)
	*/
	for (i = 0; i < 5; i++);
#else
	/*
		不添加延迟语句， F407 (168MHz主频） GPIO模拟，实测 SCK 周期 = 200ns (大约5M)
	*/
	__NOP();
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

	for(i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			MOSI_1();
		}
		else
		{
			MOSI_0();
		}
		bsp_spiDelay();
		SCK_1();
		_ucByte <<= 1;
		bsp_spiDelay();
		SCK_0();
	}
	bsp_spiDelay();
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

	for (i = 0; i < 8; i++)
	{
		read = read<<1;

		if (MISO_IS_HIGH())
		{
			read++;
		}
		SCK_1();
		bsp_spiDelay();
		SCK_0();
		bsp_spiDelay();
	}
	return read;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
