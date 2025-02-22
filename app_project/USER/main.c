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
#include "lcd_op.h"
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


#define  FLASH_1_Sector    0x1E000

#define Erase 0
uint8_t Tx_Buffer[] = "123456";


uint8_t Rx_Buffer[BufferSize];
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12x12;
static void _ucgui(){
  GUI_Init();
//	GUI_SetBkColor( GUI_RED);
//	GUI_SetColor( GUI_GREEN);
//	GUI_Clear();	

//	GUI_SetPenSize(80);//鐢荤瑪绮楃粏璁剧疆

//	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
//	
////	GUI_DrawBitmap(&bmucgui_1, 80,80);
//	
//	GUI_SetFont(&GUI_FontHZ12x12);
//	
//	char test[] = {0x31,0x32,0xce,0xd2,0xb5,0xa4,0x00};
//	GUI_DispStringAt(test, 0, 0);
	
}


static void _write_flash_test(u8* pBuffer, u32 WriteAddr ){
	u32 addr = 0;
	for(int i=0; i< 29; i++){
		addr = WriteAddr + i*4096;
		printf("sect [%02X] ", addr);
		SPI_FLASH_SectorErase(addr);
		for(int j=0; j < 16; j++){
			SPI_FLASH_PageWrite(pBuffer, addr + j*256, 256);
			printf("page [%02X] ", addr + j*256);
		}
		printf("\r\n");
	}
	printf("xxxxxxx\n");
}


static void _read_flash_test(u32 WriteAddr, int len){
	u32 addr = 0;
	u8 t_buf[256] = {0};
	
	for(int i=0; i < 115200; i=i+len){
		if(115200 - i >= len){
			SPI_FLASH_BufferRead(t_buf, WriteAddr+i, len);
			for(int j=0; j<len; j++){
				printf("[%02X]", t_buf[j]);
			}
			printf(" end\r\n");
			
		}else{
			SPI_FLASH_BufferRead(t_buf, WriteAddr+i, 115200 - i);
			
			for(int j=0; j<115200 - i; j++){
				printf("[%02X]", t_buf[j]);
			}
			printf(" end\r\n");
			
			break;
		}
	}
}

static int g_open = 0;

static void _flash(){
	
		__IO uint32_t Flash_Size = 0;
		/* 获取 Flash Device ID */
		DeviceID = SPI_FLASH_ReadDeviceID();	
		
		/* 获取 SPI Flash ID */
		FlashID = SPI_FLASH_ReadID();	
		printf("\r\n FlashID is 0x%X \r\n Device ID is 0x%X\r\n", FlashID, DeviceID);
	

		/* 检验 SPI Flash ID */
		if (FlashID == W25Q16ID || W25Q32ID || W25Q64ID || W25Q80ID)
		{	
			
				printf("\r\n 检测到串行flash芯片!\r\n");
			  switch(FlashID)
					{
					case W25Q16ID :
						printf("\r\n flash芯片型号为W25Q16ID!\r\n");
						Flash_Size = 2;
						break;
					case W25Q32ID :
						printf("\r\n flash芯片型号为W25Q32!\r\n");
						Flash_Size = 4;
						break;
					case W25Q64ID :
						printf("\r\n flash芯片型号为W25Q64!\r\n");
						Flash_Size = 8;
						break;
					case W25Q80ID :
						printf("\r\n flash芯片型号为W25Q80!\r\n");
						Flash_Size = 1;
						break;
					default :
						printf("\r\n flash芯片型号为其他!\r\n");
						Flash_Size = 0;
						break;
					}

#if 0					
				/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
				// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
				SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 

				/* 将发送缓冲区的数据写到flash中 */
				// 这里写一页，一页的大小为256个字节
				SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
				printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);

			
				/* 将刚刚写入的数据读出来放到接收缓冲区中 */
				SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
				printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
#endif

				uint8_t t_buf[256] = {0};
				


				if(g_open == 1){
					memset(t_buf, 0xcc, 256);	
					_write_flash_test(t_buf, FLASH_SectorToErase);
					
				}	
				

				if(g_open == 2){
				memset(t_buf, 0x66, 256);	
					_write_flash_test(t_buf, FLASH_1_Sector);
					
				}	

				
				
				if(g_open == 3){
					_read_flash_test(FLASH_SectorToErase, 256);
				}


				if(g_open == 4){
					_read_flash_test(FLASH_1_Sector, 256);
				}				
				g_open = 0;

		}// if (FlashID == sFLASH_ID)
		else// if (FlashID == sFLASH_ID)
		{ 

				printf("\r\n 获取不到 W25Q64 ID!\n\r");
		}

}

void show_pic(){
//	extern void lcd_send_data(uint32_t len, const u8 *tx_buf)	;
//	extern const unsigned char gImage_111[4800];
//	for(int i=0; i < 4800; i = i + 240*2){
//		if(4800 -i >= 240*2){
//			lcd_send_data(240*2, gImage_111 + i);
//		}else{
//			lcd_send_data(4800 - i, gImage_111 + i);
//			break;
//		}
//	}
}


//


void flash2lcd_dma(uint32_t addr_var){
extern void lcd_send_data(uint32_t len, const u8 *tx_buf)	;
	
extern void W25QXX_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);	
	
	LCD_Address_Set(0,0,239,239);//设置显示范围
	int len = 240*2;
	uint32_t WriteAddr = addr_var;
	uint8_t t_buf[500] = {0};
	
	
	for(int i=0; i < 115200; i=i+len){
		if(115200 - i >= len){
			//SPI_FLASH_BufferRead(t_buf, WriteAddr+i, len);
			W25QXX_FLASH_BufferRead(t_buf, WriteAddr+i, len);
			lcd_send_data(len, t_buf +4);
		}else{
			//SPI_FLASH_BufferRead(t_buf, WriteAddr+i, 115200 - i);
			W25QXX_FLASH_BufferRead(t_buf, WriteAddr+i, len);
			lcd_send_data(115200 - i, t_buf +4);
			break;
		}
	}	
}



int main(void)
{
	float t=0;
	__IO uint32_t Flash_Size = 0;
	
	u8 tx_buf[8] = {0};
	u8 rx_buf[8] = {0};
	
	u8 rx1_buf[8] = {0};
	
	delay_init();
	LED_Init();//LED初始化
	Usart1_Init(115200);
	
	
	
	SPI_FLASH_Init();	

#if 1

extern void W25QXX_RX_DMA_Init(void);	
extern void W25QXX_read_data(uint32_t len, uint32_t tx_len, u8 *rx_buf, u8 *tx_buf);
	
	
	W25QXX_RX_DMA_Init();
	

	tx_buf[0] = 0xAB;
	tx_buf[1] = 0xFF;
	tx_buf[2] = 0xFF;
	tx_buf[3] = 0xFF;
	tx_buf[4] = 0xFF;
	
	W25QXX_read_data(5,5,rx_buf, tx_buf);
	
	tx_buf[0] = 0x9F;
	tx_buf[1] = 0xFF;
	tx_buf[2] = 0xFF;
	tx_buf[3] = 0xFF;
	W25QXX_read_data(4,4,rx1_buf, tx_buf);
	
	
#if 0	
	DeviceID = _ReadDeviceID();	
	FlashID = _SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X \r\n Device ID is 0x%X\r\n", FlashID, DeviceID);	
#endif
	
#else	
	_flash();	
#endif	
		
	_flash();	
	
	LCD_Init_Op();//LCD初始化
	char test[] = {0x31,0x32,0xce,0xd2,0xb5,0xa4,0x00};
	_ucgui();

	GUI_SetBkColor( GUI_RED);			
	GUI_Clear();	
	delay_ms(1000);



	//show_pic();
	flash2lcd_dma(FLASH_SectorToErase);
	
	flash2lcd_dma(FLASH_1_Sector);


	while(1)
	{

#if 0		
		LED=0; //PC13点亮

		delay_ms(500);
		LED=1;//PC13熄灭	
		delay_ms(500);


		GUI_Clear();	

		GUI_DispStringAt(test, 0, 0);
#else

#if 0		
		GUI_SetBkColor( GUI_RED);			
		GUI_Clear();
		delay_ms(1000);
		GUI_SetBkColor( GUI_YELLOW);			
		GUI_Clear();	
		delay_ms(1000);		
		GUI_SetBkColor( GUI_BLACK);			
		GUI_Clear();			
		
		delay_ms(1000);
#endif

#endif		
		flash2lcd_dma(FLASH_SectorToErase);
		delay_ms(500);
		flash2lcd_dma(FLASH_1_Sector);
		delay_ms(500);
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
