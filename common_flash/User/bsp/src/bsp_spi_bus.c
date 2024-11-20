/*
*********************************************************************************************************
*
*	ģ������ : SPI��������
*	�ļ����� : bsp_spi_bus.c
*	��    �� : V1.0
*	˵    �� : ģ��SPIʵ��
*	�޸ļ�¼ :
*		�汾��   ����        ����      ˵��
*		V1.0	2022-05-29  Eric2013   �׷���
*
*	Copyright (C), 2020-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSPIBus
*	����˵��: ����SPI���ߡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{	
#if 0	
	/* ���� SPI����GPIO : CS SCK MOSI MISO */
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
		
	/* ʱ�� */
	SF_CS_CLK_ENABLE();
	RCC_SCK_CLK_ENABLE();
	RCC_MOSI_CLK_ENABLE();
	RCC_MISO_CLK_ENABLE();

	/* SPI CS ##############################*/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	/* ����������� */
	GPIO_InitStruct.Pull = GPIO_NOPULL;			/* ���������費ʹ�� */
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;  	/* GPIO�ٶȵȼ� */	
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
*	�� �� ��: bsp_SpiDelay
*	����˵��: ʱ���ӳ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spiDelay(void)
{
#if 0
	uint32_t i;

	/*
		�ӳ�5ʱ�� F407 (168MHz��Ƶ�� GPIOģ�⣬ʵ�� SCK ���� = 480ns (��Լ2M)
	*/
	for (i = 0; i < 5; i++);
#else
	/*
		������ӳ���䣬 F407 (168MHz��Ƶ�� GPIOģ�⣬ʵ�� SCK ���� = 200ns (��Լ5M)
	*/
//	__NOP();
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiWrite0
*	����˵��: ��SPI���߷���һ���ֽڡ�SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_spiRead0
*	����˵��: ��SPI���߽���8��bit���ݡ� SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ����������
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
