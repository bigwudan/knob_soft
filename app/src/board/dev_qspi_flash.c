/**
 *******************************************************************************************************************************************
 * @file        dev_qspi_flash.c
 * @brief       QSPI Nor Flash Driver
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-08-18   lzh          the first version
 * 2024-10-10   lzh          fix QSPI_line-4 DMA read
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include "SWM221.h"
#include "dev_qspi_flash.h"

void flash_qspi_port_init(void)
{
    PORT_Init(PORTC, PIN2,  PORTC_PIN2_QSPI0_SSEL,  0);
    
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_QSPI0_SCLK,  0);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_QSPI0_MOSI, 1);
	PORT_Init(PORTB, PIN0,  PORTB_PIN0_QSPI0_MISO,  1);
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_QSPI0_D2,    1);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_QSPI0_D3,    1);
}
void flash_qspi_port_deinit(void)
{
    GPIO_INIT(GPIOC, PIN2, GPIO_OUTPUT); // 片选必须拉高
    GPIO_AtomicSetBit(GPIOC, PIN2);

    /* 出于辐射考量，可以在此将以下端口设为普通 GPIO 输入 & 无上/下拉电阻 
    GPIO_INIT(GPIOC, PIN3, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN15, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN0, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN1, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN2, GPIO_INPUT);*/
}

void qspi_flash_init(void)
{
	flash_qspi_port_init();
	
    QSPI_InitStructure QSPI_initStruct;
	QSPI_initStruct.Size = QSPI_Size_16MB;
	QSPI_initStruct.ClkDiv = 2;
	QSPI_initStruct.ClkMode = QSPI_ClkMode_3;
	QSPI_initStruct.SampleShift = (2 == QSPI_initStruct.ClkDiv) ? QSPI_SampleShift_1_SYSCLK : QSPI_SampleShift_NONE;
	QSPI_initStruct.IntEn = 0;
	QSPI_Init(QSPI0, &QSPI_initStruct);
	QSPI_Open(QSPI0);
	
	uint32_t id = QSPI_ReadJEDEC(QSPI0);
	printf("SPI Flash JEDEC: %06X\n", id);
	
	int quad = QSPI_QuadState(QSPI0);
	if(quad == 0)
	{
		QSPI_QuadSwitch(QSPI0, 1);
		
		quad = QSPI_QuadState(QSPI0);
	}
	printf("SPI Flash Quad %s\n", quad ? "enabled" : "disabled");
    
    if (QSPI_initStruct.Size > QSPI_Size_16MB)
    {
        printf("SPI Flash 4Byte addr Enable\n");
        QSPI_4ByteAddrEnable(QSPI0);
    }
}

void qspi_dma_read(uint32_t addr, void *data, uint32_t cnt, uint8_t addr_width, uint8_t data_width)
{
/* 221 的 DMA 单次传输最大支持 65535 个 Unit */
#define DMA_SINGLE_CNT_MAX    ( 0xFFFF )
    if (cnt > DMA_SINGLE_CNT_MAX) {
        printf("[%s] param [%p][%d].\r\n", __FUNCTION__, data, cnt);
        //while (1) __NOP();
        return ;
    }
    while (QSPI_Busy(QSPI0)) __NOP();
    
    if (cnt < 16) {
        QSPI_Read_(QSPI0, addr, data, cnt, addr_width, data_width, 1);
        return ;
    }
    QSPI_DMAEnable(QSPI0, QSPI_Mode_IndirectRead);
    
    DMA_InitStructure DMA_initStruct;
    DMA_initStruct.Mode = DMA_MODE_SINGLE;
    DMA_initStruct.Unit = DMA_UNIT_BYTE; /* Bytes */
    DMA_initStruct.Count = cnt;
    DMA_initStruct.MemoryAddr = (uint32_t)data;
    DMA_initStruct.MemoryAddrInc = 1;
    DMA_initStruct.PeripheralAddr = (uint32_t)&QSPI0->DRB;
    DMA_initStruct.PeripheralAddrInc = 0;
    DMA_initStruct.Handshake = DMA_CH1_QSPI0RX;
    DMA_initStruct.Priority = DMA_PRI_LOW;
    DMA_initStruct.INTEn = 0; /* 失能中断 */
    
    /* 对齐时可加速 */
    if (0 == ((uint32_t)data & (4 - 1)) && 0 == (cnt & (4 - 1))) {
        DMA_initStruct.Unit = DMA_UNIT_WORD; /* Word */
        DMA_initStruct.Count = cnt >> 2;
        DMA_initStruct.PeripheralAddr = (uint32_t)&QSPI0->DRW;
    }
    else if (0 == ((uint32_t)data & (2 - 1)) && 0 == (cnt & (2 - 1))) {
        DMA_initStruct.Unit = DMA_UNIT_HALFWORD; /* Half_Word */
        DMA_initStruct.Count = cnt >> 1;
        DMA_initStruct.PeripheralAddr = (uint32_t)&QSPI0->DRH;
    }
    DMA_CH_Init(DMA_CH1, &DMA_initStruct);
    DMA_CH_Open(DMA_CH1);
    
    QSPI_Read_(QSPI0, addr, data, cnt, addr_width, data_width, 0);

    while (0 == DMA_CH_INTStat(DMA_CH1, DMA_IT_DONE)) __NOP();
    DMA_CH_INTClr(DMA_CH1, DMA_IT_DONE);
    
    QSPI_Abort(QSPI0);
    
    //在 QSPI busy 时，写 QSPI->CR 寄存器无效
    while (QSPI_Busy(QSPI0)) __NOP();
    
    QSPI_DMADisable(QSPI0);
}
