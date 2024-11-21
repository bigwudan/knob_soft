/*
*********************************************************************************************************
*
*	模块名称 : FlashPrg
*	文件名称 : FlashPrg.c
*	版    本 : V1.0
*	说    明 : Flash编程接口
*
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2020-11-06  Eric2013   正式发布
*
*	Copyright (C), 2020-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "FlashOS.h"
#include "bsp.h"


/*
*********************************************************************************************************
*	函 数 名: Init
*	功能说明: Flash编程初始化
*	形    参: adr Flash基地址，芯片首地址。
*             clk 时钟频率
*             fnc 函数代码，1 - Erase, 2 - Program, 3 - Verify
*	返 回 值: 0 表示成功， 1表示失败
*********************************************************************************************************
*/
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) 
{
		board_init();
    return 0;
}

/*
*********************************************************************************************************
*	函 数 名: UnInit
*	功能说明: 复位初始化
*	形    参: fnc 函数代码，1 - Erase, 2 - Program, 3 - Verify
*	返 回 值: 0 表示成功， 1表示失败
*********************************************************************************************************
*/
int UnInit (unsigned long fnc) 
{ 
    return (0);
}

/*
*********************************************************************************************************
*	函 数 名: EraseChip
*	功能说明: 整个芯片擦除
*	形    参: 无
*	返 回 值: 0 表示成功， 1表示失败
*********************************************************************************************************
*/
int EraseChip (void) 
{	
    sf_EraseChip();
    return 0;      
}

/*
*********************************************************************************************************
*	函 数 名: EraseSector
*	功能说明: 扇区擦除
*	形    参: adr 擦除地址
*	返 回 值: 无
*********************************************************************************************************
*/
int EraseSector (unsigned long adr) 
{	
    adr -= SPI_FLASH_MEM_ADDR;

    sf_EraseSector(adr);
    
    return 0;     
}

/*
*********************************************************************************************************
*	函 数 名: ProgramPage
*	功能说明: 页编程
*	形    参: adr 页起始地址
*             sz  页大小
*             buf 要写入的数据地址
*	返 回 值: 无
*********************************************************************************************************
*/
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) 
{
	volatile int i = 0;
	
    adr -= SPI_FLASH_MEM_ADDR;
	
	for(i = 0; i < sz/256; i++)
	{
		sf_PageWrite(buf+256*i, adr+256*i, 256);		
	}
	
	if(sz%256)
	{
		sf_PageWrite(buf+256*i, adr+256*i, sz%256);		
	}
      
    return (0);                      
}

/*
*********************************************************************************************************
*	函 数 名: Verify
*	功能说明: 校验
*	形    参: adr 起始地址
*             sz  数据大小
*             buf 要校验的数据缓冲地址
*	返 回 值: -
*********************************************************************************************************
*/
#define bufsize  2048
unsigned char aux_buf[bufsize];
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{
    volatile int i=0, j=0;

    adr -= SPI_FLASH_MEM_ADDR;

	for(i = 0; i < sz/bufsize; i++)
	{
		sf_ReadBuffer(aux_buf, adr+bufsize*i, bufsize);
		
		for (j = 0; j< bufsize; j++) 
		{
			if (aux_buf[j] != buf[j+bufsize*i]) 
			return (adr + j + bufsize*i);              /* 校验失败 */     
		}	
	}
	
	if(sz%bufsize)
	{
		sf_ReadBuffer(aux_buf, adr+bufsize*i, sz%bufsize);
		
		for (j = 0; j< sz%bufsize; j++) 
		{
			if (aux_buf[j] != buf[j+bufsize*i]) 
			return (adr + j + bufsize*i);              /* 校验失败 */     
		}			
	}

    adr += SPI_FLASH_MEM_ADDR;
    return (adr+sz);                      /* 校验成功 */    
}

/*
*********************************************************************************************************
*	函 数 名: BlankCheck
*	功能说明: 查空检测
*	形    参: adr 块起始地址
*             sz  页=块大小
*             pat Block Pattern
*	返 回 值: 0 - OK,  1 - Failed (需要擦除）
*********************************************************************************************************
*/
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat) 
{
    volatile int i=0, j=0;
	
    adr -= SPI_FLASH_MEM_ADDR;

	for(i = 0; i < sz/bufsize; i++)
	{
		sf_ReadBuffer(aux_buf, adr+bufsize*i, bufsize);
		
		for (j = 0; j< bufsize; j++) 
		{
			if (aux_buf[j] != pat) 
			return 1;               /* 需要擦除 */     
		}	
	}
	
	if(sz%bufsize)
	{
		sf_ReadBuffer(aux_buf, adr+bufsize*i, sz%bufsize);
		
		for (j = 0; j< sz%bufsize; j++) 
		{
			if (aux_buf[j] != pat) 
			return 1;               /* 需要擦除 */      
		}			
	}

    return 0;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
