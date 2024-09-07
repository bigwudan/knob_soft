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
extern const unsigned char gImage_pic1[25600];
extern const unsigned char gImage_pic2[25600];
unsigned char buf_gImage_pic2[160*40*2] = {0};


static void _flash_write(){

	
//	SPI_FLASH_BulkErase();
	
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_0);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_1);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_2);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_3);	
	
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_4);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_5);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_6);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_7);

	
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_8);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_9);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_10);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_11);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_12);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_13);	
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_14);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_0_15);
	
	
	SPI_FLASH_BufferWrite((u8 *)gImage_pic1, FLASH_SectionAddress_0_0, 12800);		
	SPI_FLASH_BufferWrite((u8 *)(gImage_pic1 + 12800), FLASH_SectionAddress_0_4, 12800);

	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_0);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_1);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_2);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_3);	
	
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_4);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_5);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_6);	 	 
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_7);

	
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_8);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_9);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_10);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_11);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_12);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_13);	
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_14);
	SPI_FLASH_SectorErase(FLASH_SectionAddress_1_15);
	
	
	SPI_FLASH_BufferWrite((u8 *)gImage_pic2, FLASH_SectionAddress_1_0, 12800);		
	SPI_FLASH_BufferWrite((u8 *)(gImage_pic2 + 12800), FLASH_SectionAddress_1_4, 12800);
	
			
}

static void _flash_read(u32 address){
	SPI_FLASH_BufferRead((u8 *)buf_gImage_pic2, address, countof(buf_gImage_pic2));
}

static void _show_picfromflash(u16 x,u16 y,u16 length,u16 width){
	LCD_ShowPicture(x,y,length, width,buf_gImage_pic2);
}


static void _test_pic(u8 index){
	
	if(index == 0){
		_flash_write();
	}else if(index == 1){
		_flash_read(FLASH_SectionAddress_0_0);
		_show_picfromflash(0, 0, 160, 40);
		_flash_read(FLASH_SectionAddress_0_4);
		_show_picfromflash(0, 40, 160, 40);
	}else if(index == 2){
		_flash_read(FLASH_SectionAddress_1_0);
		_show_picfromflash(0, 0, 160, 40);
		_flash_read(FLASH_SectionAddress_1_4);
		_show_picfromflash(0, 40, 160, 40);	
	}
	

}

int main(void)
{
	float t=0;
	__IO uint32_t Flash_Size = 0;
	delay_init();
	
	LED_Init();//LED初始化
	Usart1_Init(115200);
	SPI_FLASH_Init();	

	LCD_HW_Init();//LCD初始化
	
/*	
	GUI_Init();
	GUI_SetBkColor(WHITE); 
	GUI_Clear();	
	


	GUI_SetColor(RED);

	GUI_DrawHLine(10,10,40);
	GUI_SetFont(&GUI_Font8_ASCII);
	GUI_DispStringAt("Line ",0,1);
	
	GUI_DrawBitmap(&bm111,0,0);
*/	



//		/* 检验 SPI Flash ID */
//		if (FlashID == W25Q16ID || W25Q32ID || W25Q64ID || W25Q80ID)
//		{	
//			
//				printf("\r\n 检测到串行flash芯片!\r\n");
//			  switch(FlashID)
//					{
//					case W25Q16ID :
//						printf("\r\n flash芯片型号为W25Q16ID!\r\n");
//						Flash_Size = 2;
//						break;
//					case W25Q32ID :
//						printf("\r\n flash芯片型号为W25Q32!\r\n");
//						Flash_Size = 4;
//						break;
//					case W25Q64ID :
//						printf("\r\n flash芯片型号为W25Q64!\r\n");
//						Flash_Size = 8;
//						break;
//					case W25Q80ID :
//						printf("\r\n flash芯片型号为W25Q80!\r\n");
//						Flash_Size = 1;
//						break;
//					default :
//						printf("\r\n flash芯片型号为其他!\r\n");
//						Flash_Size = 0;
//						break;
//					}

//				/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
//				// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
//				SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 

//				/* 将发送缓冲区的数据写到flash中 */
//				// 这里写一页，一页的大小为256个字节
//				SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
//				printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);

//			
//				/* 将刚刚写入的数据读出来放到接收缓冲区中 */
//				SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
//				printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
//			

//				/* 检查写入的数据与读出的数据是否相等 */
//				TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

//				if( PASSED == TransferStatus1 )
//				{ 

//						printf("\r\n 串行flash测试成功!\n\r");
//				}
//				else
//				{        

//						printf("\r\n 串行flash测试失败!\n\r");
//				}
//		}// if (FlashID == sFLASH_ID)
//		else// if (FlashID == sFLASH_ID)
//		{ 

//				printf("\r\n 获取不到 W25Q64 ID!\n\r");
//		}

	_test_pic(0);
	

	while(1)
	{
		_test_pic(1);
		delay_ms(1000);	
		_test_pic(2);
		delay_ms(1000);
		
		
		
		
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
