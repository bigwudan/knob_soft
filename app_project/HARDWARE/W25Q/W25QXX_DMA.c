/**********************************************************************************
 * 文件名  ：spi_flash.c
 * 描述    ：spi 底层应用函数库         

 * 硬件连接 ----------------------------
 *         | PA4-SPI1-NSS  : W25X16-CS  |
 *         | PA5-SPI1-SCK  : W25X16-CLK |
 *         | PA6-SPI1-MISO : W25X16-DO  |
 *         | PA7-SPI1-MOSI : W25X16-DIO |
 *          ----------------------------

**********************************************************************************/
#include "W25QXX.h"


/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF


#define SPI2_RX_DMA_BUFF_SIZE        240*2
static u8 SPI2_RX_DMA_Buff[SPI2_RX_DMA_BUFF_SIZE] = {0};


static u8 SPI2_TX_DMA_Buff[SPI2_RX_DMA_BUFF_SIZE] = {0};

static u8 _flash_flag = 0;

void W25QXX_RX_DMA_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    
    DMA_DeInit(DMA1_Channel4);
    
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&SPI2->DR);
    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)SPI2_RX_DMA_Buff;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = 9; //9
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(DMA1_Channel4,&DMA_InitStruct);
    
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
    DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,DISABLE);
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,DISABLE);
    
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);

    DMA_Cmd(DMA1_Channel4,DISABLE);    
		
		
    
    
		
		
		//tx
    DMA_DeInit(DMA1_Channel5);
		
		
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&SPI2->DR);
    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)SPI2_TX_DMA_Buff;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize = 9; //9
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(DMA1_Channel5,&DMA_InitStruct);
    
    DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
    DMA_ITConfig(DMA1_Channel5,DMA_IT_HT,DISABLE);
    DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,DISABLE);
    
		


    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);

    DMA_Cmd(DMA1_Channel5,DISABLE);    
		
		
		


}



void W25QXX_read_data(uint32_t len, uint32_t tx_len, u8 *rx_buf, u8 *tx_buf){
	SPI_FLASH_CS_LOW();
	DMA_Cmd(DMA1_Channel4,DISABLE);
	DMA_Cmd(DMA1_Channel5,DISABLE);
	
	memcpy(SPI2_TX_DMA_Buff, tx_buf, tx_len);
	DMA_SetCurrDataCounter(DMA1_Channel4, len);
	DMA_SetCurrDataCounter(DMA1_Channel5, len);
	
	DMA_Cmd(DMA1_Channel4,ENABLE);
	DMA_Cmd(DMA1_Channel5,ENABLE);	
	while(_flash_flag != 1);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);	
	memcpy(rx_buf, SPI2_RX_DMA_Buff, len);
	DMA_Cmd(DMA1_Channel4,DISABLE);
	DMA_Cmd(DMA1_Channel5,DISABLE);	

	SPI_FLASH_CS_HIGH();
}

void W25QXX_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u8 tx_buf[4] = {0};
	tx_buf[0]	 = W25X_ReadData;
	tx_buf[1]	 = ((ReadAddr & 0xFF0000) >> 16);
	tx_buf[2]	 = ((ReadAddr& 0xFF00) >> 8);
	tx_buf[3]	 = (ReadAddr & 0xFF);
	
	W25QXX_read_data(NumByteToRead + 4, 4, pBuffer, tx_buf);
	
}

void DMA1_Channel4_IRQHandler()
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET){
    DMA_ClearFlag(DMA1_FLAG_TC4);
		_flash_flag = 1;
	}
}


void DMA1_Channel5_IRQHandler()
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5) != RESET){
    DMA_ClearFlag(DMA1_FLAG_TC5);
		_flash_flag = 2;
	}
}

#define W25X_DeviceID			        0xAB 

#define W25X_JedecDeviceID		    0x9F 
#define Dummy_Byte                0xFF



void W25QXX_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, byte);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == SET);
}

u32 _ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  W25QXX_SendByte(W25X_DeviceID);
  W25QXX_SendByte(Dummy_Byte);
  W25QXX_SendByte(Dummy_Byte);
  W25QXX_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  W25QXX_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}

u32 _SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  W25QXX_SendByte(W25X_JedecDeviceID);

  /* Read a byte from the FLASH */
  W25QXX_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  W25QXX_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  W25QXX_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/******************************END OF FILE*****************************/



