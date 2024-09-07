//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ABrobot����
//
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : ABrobot
//  ��������   : 2023-10-25
//  ����޸�   : 
//  ��������   :��ʾ����(STM32F103 TFT������ϵ��)
//              
//							��Ļ����
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   3.3v��Դ
//              SCL   PB3��SCLK��
//              SDA   PB5��MOSI��
//              RES   PB6
//              DC    PB4
//              CS    PB7
//              
//              ----------------------------------------------------------------
//
//							FLASHоƬ����
//              ----------------------------------------------------------------
//
//              GND   ��Դ��
//              VCC   3.3v��Դ
//              CS    PB12
//              CLK   PB13
//              DO    PB14
//              DI    PB15
//             
//              
//              ----------------------------------------------------------------
//��Ȩ���У�����ؾ���
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

/* ��ȡ�������ĳ��� */
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

// ����ԭ������
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);
extern const GUI_BITMAP bm111;
int main(void)
{
	float t=0;
	__IO uint32_t Flash_Size = 0;
	delay_init();
	LED_Init();//LED��ʼ��
	Usart1_Init(115200);
	SPI_FLASH_Init();	

	LCD_HW_Init();//LCD��ʼ��
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
		LED=0; //PC13����


		delay_ms(500);
		LED=1;//PC13Ϩ��	
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
