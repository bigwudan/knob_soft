//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ABrobot智能
//
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : ABrobot
//  生成日期   : 2023-10-25
//  最近修改   : 
//  功能描述   :演示例程(STM32F103 TFT开发板系列)
//              
//							屏幕接线
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   3.3v电源
//              SCL   PB3（SCLK）
//              SDA   PB5（MOSI）
//              RES   PB6
//              DC    PB4
//              CS    PB7
//              
//              ----------------------------------------------------------------
//
//							FLASH芯片接线
//              ----------------------------------------------------------------
//
//              GND   电源地
//              VCC   3.3v电源
//              CS    PB12
//              CLK   PB13
//              DO    PB14
//              DI    PB15
//             
//              
//              ----------------------------------------------------------------
//版权所有，盗版必究。
//All rights reserved
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "usart_1.h"
#include "W25QXX.h"
#include "GUI.h"

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

#define Erase 0
uint8_t Tx_Buffer[] = "123456";


uint8_t Rx_Buffer[BufferSize];
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);
extern const GUI_BITMAP bm111;
int main(void)
{
	float t=0;
	__IO uint32_t Flash_Size = 0;
	delay_init();
	LED_Init();//LED初始化
	Usart1_Init(115200);
	SPI_FLASH_Init();	

	LCD_HW_Init();//LCD初始化
	GUI_Init();
	GUI_SetBkColor(WHITE); 
	GUI_Clear();	
	
	//LCD_Fill(10, 10, 40-1, 10+1, RED);
	
//	GUI_SetBkColor(RED); 
//	GUI_Clear();
//	LED=0;

//	
	GUI_SetColor(RED);
//	
//	
	GUI_DrawHLine(10,10,40);
	GUI_SetFont(&GUI_Font8_ASCII);
	GUI_DispStringAt("Line ",0,1);
	
	GUI_DrawBitmap(&bm111,0,0);

	while(1)
	{
		LED=0; //PC13点亮


		delay_ms(500);
		LED=1;//PC13熄灭	
		delay_ms(500);
		printf("SPI_FLASH_PageWrite too large!\n\r");
	}
	
}

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
