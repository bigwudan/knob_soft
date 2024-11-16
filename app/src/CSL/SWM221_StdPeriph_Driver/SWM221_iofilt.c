/****************************************************************************************************************************************** 
* �ļ�����:	SWM221_iofilt.c
* ����˵��:	SWM221��Ƭ����IO�˲�������ģ�飬��PAD��ģ���������ź��˲���խ��ָ����ȵ���������ë�̣�����
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
#include "SWM221_iofilt.h"


/****************************************************************************************************************************************** 
* ��������:	IOFILT_Init()
* ����˵��:	IO�˲�����ʼ��
* ��    ��: uint32_t IOFILTn	Ҫ��ʼ�����˲�������ȡֵ 0-1
*			uint32_t signal		Ҫ����Щ�źŽ����˲����� IOFILTn = 0 ʱ����ȡֵ IOFILT0_PB14��IOFILT0_PB4��IOFILT0_PB5��IOFILT0_PB6 ���䡰��
*													  �� IOFILTn = 1 ʱ����ȡֵ IOFILT1_ACMP0��IOFILT1_ACMP1 ���䡰��
*			uint32_t width		��ѡ�ź��Ͽ��С�� width �����屻����ë�̣����˵�����ȡֵ IOFILT_WIDTH_250ns��IOFILT_WIDTH_500ns��...
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void IOFILT_Init(uint32_t IOFILTn, uint32_t signal, uint32_t width)
{
	SYS->CLKSEL &= ~SYS_CLKSEL_IOFILT_Msk;
	SYS->CLKSEL |= (0 << SYS_CLKSEL_IOFILT_Pos);	//�˲���ʱ��Դ��HRC
	
	SYS->CLKEN0 |= SYS_CLKEN0_IOFILT_Msk;
	for(int i = 0; i < 10; i++)  __NOP();
	
	*(&SYS->IOFILT0 + IOFILTn) = (signal << SYS_IOFILT_IO0EN_Pos)  |
								 (0      << SYS_IOFILT_CLKDIV_Pos) |
								 (width  << SYS_IOFILT_TIM_Pos);
}
