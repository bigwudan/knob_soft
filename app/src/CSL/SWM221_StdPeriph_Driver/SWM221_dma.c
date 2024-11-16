/****************************************************************************************************************************************** 
* �ļ�����: SWM342_dma.c
* ����˵��:	SWM342��Ƭ����DMA����������
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
#include "SWM221_dma.h"


/****************************************************************************************************************************************** 
* ��������: DMA_CH_Init()
* ����˵��:	DMAͨ����ʼ��
* ��    ��: uint32_t chn			ָ��Ҫ���õ�ͨ������Чֵ��DMA_CH0��DMA_CH1
*			DMA_InitStructure * initStruct    ����DMAͨ������趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DMA_CH_Init(uint32_t chn, DMA_InitStructure * initStruct)
{	
	DMA_CH_Close(chn);		//�رպ�����
	
	DMA->CH[chn].CR = (initStruct->Mode 			 << DMA_CR_CIRC_Pos) |
					  (initStruct->Unit				 << DMA_CR_MSIZ_Pos) |
					  (initStruct->Unit				 << DMA_CR_PSIZ_Pos) |
					  (initStruct->MemoryAddrInc 	 << DMA_CR_MINC_Pos) |
					  (initStruct->PeripheralAddrInc << DMA_CR_PINC_Pos) |
					  (initStruct->Priority			 << DMA_CR_PL_Pos);
	
	DMA->CH[chn].NDT = ( initStruct->Count 		<< DMA_NDT_LEN_Pos) |
					   ((initStruct->Count / 2) << DMA_NDT_HALF_Pos);
	
	DMA->CH[chn].MAR = initStruct->MemoryAddr;
	DMA->CH[chn].PAR = initStruct->PeripheralAddr;
	
	switch(initStruct->Handshake & DMA_HS_MSK)
	{
	case DMA_HS_NO:
		DMA->CH[chn].MUX = 0;
		DMA->CH[chn].CR |= DMA_CR_DIR_Msk | DMA_CR_MEM2MEM_Msk;
		break;
	
	case DMA_HS_MRD:
		DMA->CH[chn].MUX = ((initStruct->Handshake & 0xF) << DMA_MUX_MRDHSSIG_Pos) | (1 << DMA_MUX_MRDHSEN_Pos);
		DMA->CH[chn].CR |= DMA_CR_DIR_Msk;
		break;
	
	case DMA_HS_MWR:
		DMA->CH[chn].MUX = ((initStruct->Handshake & 0xF) << DMA_MUX_MWRHSSIG_Pos) | (1 << DMA_MUX_MWRHSEN_Pos);
		DMA->CH[chn].CR &= ~DMA_CR_DIR_Msk;
		break;
	
	case DMA_HS_EXT | DMA_HS_MRD:
		DMA->CH[chn].MUX = ((initStruct->Handshake & 0xF) << DMA_MUX_EXTHSSIG_Pos) | (1 << DMA_MUX_EXTHSEN_Pos);
		DMA->CH[chn].CR |= DMA_CR_DIR_Msk;
		break;
	
	case DMA_HS_EXT | DMA_HS_MWR:
		DMA->CH[chn].MUX = ((initStruct->Handshake & 0xF) << DMA_MUX_EXTHSSIG_Pos) | (1 << DMA_MUX_EXTHSEN_Pos);
		DMA->CH[chn].CR &= ~DMA_CR_DIR_Msk;
		break;
	}

	DMA_CH_INTClr(chn, initStruct->INTEn);
	DMA_CH_INTEn(chn, initStruct->INTEn);
	
	if(initStruct->INTEn) NVIC_EnableIRQ(GPIOB1_GPIOA9_DMA_IRQn);
}

/****************************************************************************************************************************************** 
* ��������: DMA_CH_Open()
* ����˵��:	DMAͨ�������������������ͨ�����������������䣻����Ӳ������ͨ������������ȳ��ִ����źź�ſ�ʼ����
* ��    ��: uint32_t chn			ָ��Ҫ���õ�ͨ������Чֵ��DMA_CH0��DMA_CH1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DMA_CH_Open(uint32_t chn)
{
	DMA->CH[chn].CR |= DMA_CR_EN_Msk;
}

/****************************************************************************************************************************************** 
* ��������: DMA_CH_Close()
* ����˵��:	DMAͨ���ر�
* ��    ��: uint32_t chn			ָ��Ҫ���õ�ͨ������Чֵ��DMA_CH0��DMA_CH1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DMA_CH_Close(uint32_t chn)
{
	DMA->CH[chn].CR &= ~DMA_CR_EN_Msk;
}


/****************************************************************************************************************************************** 
* ��������: DMA_CH_INTEn()
* ����˵��:	DMA�ж�ʹ��
* ��    ��: uint32_t chn			ָ��Ҫ���õ�ͨ������Чֵ��DMA_CH0��DMA_CH1
*			uint32_t it				interrupt type����Чֵ�� DMA_IT_DONE��DMA_IT_HALF��DMA_IT_ERROR
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DMA_CH_INTEn(uint32_t chn, uint32_t it)
{
	DMA->CH[chn].CR |= it;
}

/****************************************************************************************************************************************** 
* ��������: DMA_CH_INTDis()
* ����˵��:	DMA�жϽ�ֹ
* ��    ��: uint32_t chn			ָ��Ҫ���õ�ͨ������Чֵ��DMA_CH0��DMA_CH1
*			uint32_t it				interrupt type����Чֵ�� DMA_IT_DONE��DMA_IT_HALF��DMA_IT_ERROR
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DMA_CH_INTDis(uint32_t chn, uint32_t it)
{
	DMA->CH[chn].CR &= ~it;
}

/****************************************************************************************************************************************** 
* ��������: DMA_CH_INTClr()
* ����˵��:	DMA�жϱ�־���
* ��    ��: uint32_t chn			ָ��Ҫ���õ�ͨ������Чֵ��DMA_CH0��DMA_CH1
*			uint32_t it				interrupt type����Чֵ�� DMA_IT_DONE��DMA_IT_HALF��DMA_IT_ERROR
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DMA_CH_INTClr(uint32_t chn, uint32_t it)
{
	DMA->IFC = (it << (chn * 4));
}

/****************************************************************************************************************************************** 
* ��������: DMA_CH_INTStat()
* ����˵��:	DMA�ж�״̬��ѯ
* ��    ��: uint32_t chn			ָ��Ҫ���õ�ͨ������Чֵ��DMA_CH0��DMA_CH1
*			uint32_t it				interrupt type����Чֵ�� DMA_IT_DONE��DMA_IT_HALF��DMA_IT_ERROR
* ��    ��: uint32_t				 0 ָ���ж�δ����    ��0 ָ���ж��ѷ���   
* ע������: ��
******************************************************************************************************************************************/
uint32_t DMA_CH_INTStat(uint32_t chn, uint32_t it)
{
	return DMA->IF & (it << (chn * 4));
}
