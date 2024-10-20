/****************************************************************************************************************************************** 
* �ļ�����:	SWM221_sleep.c
* ����˵��:	SWM221��Ƭ����Sleep����������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����:	V1.0.0		2016��1��30��
* ������¼: 
*
*
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
#include "SWM221_sleep.h"


/* ע�⣺EnterSleepMode() ������RAM��ִ�У�Keil��ʵ�ַ����У�
   ����һ��Scatter file
   ��������SWM221_sleep.c���Ҽ� =��Options for File "SWM221_sleep.c" =��Properties =��Memory Assignment =��Code/Conts ѡ�� IRAM1
*/


#if defined ( __ICCARM__ )
__ramfunc
#endif
void EnterSleepMode(void)
{	
	__NOP();__NOP();__NOP();
	SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);
}
