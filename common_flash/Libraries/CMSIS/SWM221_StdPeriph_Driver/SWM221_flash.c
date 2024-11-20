/****************************************************************************************************************************************** 
* �ļ�����:	SWM221_flash.c
* ����˵��:	ʹ��оƬ��IAP���ܽ�Ƭ��Flashģ���EEPROM���������ݣ�����󲻶�ʧ
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.0.0		2016��1��30��
* ������¼: 
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology  
*******************************************************************************************************************************************/
#include "SWM221.h"
#include "SWM221_flash.h"


typedef int  (*IAP_Flash_Param_t)(uint32_t param, uint32_t flag);
typedef int  (*IAP_Flash_Erase_t)(uint32_t sector, uint32_t flag);
typedef int  (*IAP_Flash_Write_t)(uint32_t flash_addr, uint32_t ram_addr, uint32_t count, uint32_t flag);


IAP_Flash_Erase_t IAP_Flash_Erase = (IAP_Flash_Erase_t)0x01000401;
IAP_Flash_Write_t IAP_Flash_Write = (IAP_Flash_Write_t)0x01000461;
IAP_Flash_Param_t IAP_Flash_Param = (IAP_Flash_Param_t)0x010004D1;


/****************************************************************************************************************************************** 
* ��������: FLASH_Erase()
* ����˵��:	Ƭ��Flash����
* ��    ��: uint32_t addr		������ַ��������СΪ512 Byte
* ��    ��: uint32_t			FLASH_RES_OK��FLASH_RES_TO��FLASH_RES_ERR
* ע������: ��
******************************************************************************************************************************************/
uint32_t FLASH_Erase(uint32_t addr)
{
	__disable_irq();
	
	IAP_Flash_Erase(addr / 0x200, 0x0B11FFAC);
	
	Cache_Clear();
	
	__enable_irq();
	
	return FLASH_RES_OK;
}


/****************************************************************************************************************************************** 
* ��������: FLASH_Write()
* ����˵��:	Ƭ��Flashд��
* ��    ��: uint32_t addr		д���ַ
*			uint32_t buff[]		Ҫд�������
*			uint32_t count		Ҫд�����ݵĸ���������Ϊ��λ���ұ�����2����������������д��2����
* ��    ��: uint32_t			FLASH_RES_OK��FLASH_RES_TO��FLASH_RES_ERR
* ע������: д�����ݸ���������2����������������д��2����
******************************************************************************************************************************************/
uint32_t FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t count)
{
	__disable_irq();
	
	IAP_Flash_Write(addr, (uint32_t)buff, count/2, 0x0B11FFAC);
	
	Cache_Clear();
	
	__enable_irq();
	
	return FLASH_RES_OK;
}


/****************************************************************************************************************************************** 
* ��������: Flash_Param_at_xMHz()
* ����˵��:	��Flash�������ó�xMHz��Ƶ������ʱ����Ĳ���
* ��    ��: uint32_t x		ϵͳ��Ƶ����λMHz
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void Flash_Param_at_xMHz(uint32_t x)
{
	__disable_irq();
	
	IAP_Flash_Param(1000 / x, 0x0B11FFAC);
	
	__enable_irq();
}

#include "compiler.h"
#if __IS_COMPILER_ARM_COMPILER_6__

__attribute__((used, section(".SRAM"))) void Cache_Clear(void)
{
	__NOP(); __NOP(); __NOP(); __NOP();
	
	FMC->CACHE |= FMC_CACHE_CCLR_Msk;	// Cache Clear
	
	__NOP(); __NOP(); __NOP(); __NOP();
}

#elif   defined ( __CC_ARM )

/* Code_Cache_Clear �������Ǵ˺����������ɵ�ָ��
__asm void Cache_Clear(void)
{
	NOP
	NOP
	NOP
	NOP
	MOVS R0, #0x40		// 0x40045000
	LSLS R0, R0, #8
	ADDS R0, R0, #0x04
	LSLS R0, R0, #8
	ADDS R0, R0, #0x50
	LSLS R0, R0, #8
	LDR  R1,[R0, #0xC]
	MOVS R2, #1
	LSLS R2, R2, #31
	ORRS R1, R1, R2
	STR  R1,[R0, #0xC]
	NOP
	NOP
	NOP
	NOP
	BX LR
}
*/
uint16_t Code_Cache_Clear[] = {
	0xBF00, 0xBF00, 0xBF00, 0xBF00, 0x2040, 0x0200, 0x1D00, 0x0200,
	0x3050, 0x0200, 0x68C1, 0x2201, 0x07D2, 0x4311, 0x60C1, 0xBF00,
	0xBF00, 0xBF00, 0xBF00, 0x4770, 
};

__asm void Cache_Clear(void)
{

}

#elif defined ( __ICCARM__ )

__ramfunc void Cache_Clear(void)
{
	__NOP(); __NOP(); __NOP(); __NOP();
	
	FMC->CACHE |= FMC_CACHE_CCLR_Msk;	// Cache Clear
	
	__NOP(); __NOP(); __NOP(); __NOP();
}

#endif
