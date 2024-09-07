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
	
	LED_Init();//LED��ʼ��
	Usart1_Init(115200);
	SPI_FLASH_Init();	

	LCD_HW_Init();//LCD��ʼ��
	
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



//		/* ���� SPI Flash ID */
//		if (FlashID == W25Q16ID || W25Q32ID || W25Q64ID || W25Q80ID)
//		{	
//			
//				printf("\r\n ��⵽����flashоƬ!\r\n");
//			  switch(FlashID)
//					{
//					case W25Q16ID :
//						printf("\r\n flashоƬ�ͺ�ΪW25Q16ID!\r\n");
//						Flash_Size = 2;
//						break;
//					case W25Q32ID :
//						printf("\r\n flashоƬ�ͺ�ΪW25Q32!\r\n");
//						Flash_Size = 4;
//						break;
//					case W25Q64ID :
//						printf("\r\n flashоƬ�ͺ�ΪW25Q64!\r\n");
//						Flash_Size = 8;
//						break;
//					case W25Q80ID :
//						printf("\r\n flashоƬ�ͺ�ΪW25Q80!\r\n");
//						Flash_Size = 1;
//						break;
//					default :
//						printf("\r\n flashоƬ�ͺ�Ϊ����!\r\n");
//						Flash_Size = 0;
//						break;
//					}

//				/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
//				// �������4K����һ����������������С��λ������
//				SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 

//				/* �����ͻ�����������д��flash�� */
//				// ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�
//				SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
//				printf("\r\n д�������Ϊ��%s \r\t", Tx_Buffer);

//			
//				/* ���ո�д������ݶ������ŵ����ջ������� */
//				SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
//				printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);
//			

//				/* ���д�������������������Ƿ���� */
//				TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

//				if( PASSED == TransferStatus1 )
//				{ 

//						printf("\r\n ����flash���Գɹ�!\n\r");
//				}
//				else
//				{        

//						printf("\r\n ����flash����ʧ��!\n\r");
//				}
//		}// if (FlashID == sFLASH_ID)
//		else// if (FlashID == sFLASH_ID)
//		{ 

//				printf("\r\n ��ȡ���� W25Q64 ID!\n\r");
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
