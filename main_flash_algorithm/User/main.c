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
#include "bsp.h"


#include "FlashOS.h"
#include "bsp.h"



unsigned char _aux_buf[2048];
unsigned char _aux_write_buf[3] = {0x2,0x2,0x2};


static void _write_flash(){

	sf_EraseSector(0x00000);
	
	sf_PageWrite(_aux_write_buf,0x00000,  3);
}

static void _read_flash(){
	sf_ReadBuffer(_aux_buf, 0, 7);
	__NOP;
}
extern void sf_SetCS(uint8_t _Level);

int main(void)
{
	Init(0,0,0);
	_write_flash();
	while(1){
		__NOP;	
		__NOP;	
		_read_flash();
		__NOP;	
		__NOP;	
	}
	
}

