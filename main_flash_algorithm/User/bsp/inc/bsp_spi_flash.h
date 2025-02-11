/*
*********************************************************************************************************
*
*	模块名称 : SPI接口串行FLASH 读写模块
*	文件名称 : bsp_spi_flash.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2020-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_SPI_FLASH_H
#define _BSP_SPI_FLASH_H

void sf_EraseChip(void);
void sf_EraseSector(uint32_t _uiSectorAddr);
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize);
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
