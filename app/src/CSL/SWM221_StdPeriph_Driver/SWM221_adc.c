/****************************************************************************************************************************************** 
* �ļ�����:	SWM341_adc.c
* ����˵��:	SWM341��Ƭ����ADC��ģת��������������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.0.0		2016��1��30��
* ������¼: 
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
#include "SWM221_adc.h"


/****************************************************************************************************************************************** 
* ��������: ADC_Init()
* ����˵��:	ADCģ��ת������ʼ��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			ADC_InitStructure * initStruct		����ADC����ض�ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Init(ADC_TypeDef * ADCx, ADC_InitStructure * initStruct)
{
	switch((uint32_t)ADCx)
	{
	case ((uint32_t)ADC0):
	case ((uint32_t)ADC1):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_ADC0_Pos);
		break;
	}
	
	ADC_Close(ADCx);		//һЩ�ؼ��Ĵ���ֻ����ADC�ر�ʱ����
	
	ADCx->CR &= ~(ADC_CR_CLKDIV_Msk | ADC_CR_AVG_Msk | ADC_CR_BITS_Pos);
	ADCx->CR |= ((initStruct->clkdiv - 1) << ADC_CR_CLKDIV_Pos) |
				(initStruct->samplAvg	  << ADC_CR_AVG_Pos)    |
				(0					 	  << ADC_CR_BITS_Pos);
	
	if(initStruct->refsrc & (1 << 0))
	{
		PORT_Init(PORTA, PIN11, PORTA_PIN11_ADC_REFP, 0);
		
		if(initStruct->refsrc & (1 << 1))
		{
			SYS->VRFCR &=~SYS_VRFCR_LVL_Msk;
			SYS->VRFCR |= (initStruct->refsrc >> 2) << SYS_VRFCR_LVL_Pos;
			
			SYS->VRFCR |= SYS_VRFCR_EN_Msk;
		}
		else
		{
			SYS->VRFCR &=~SYS_VRFCR_EN_Msk;
		}
		
		SYS->ADCREF |= SYS_ADCREF_REFSEL_Msk;
	}
	else
	{
		SYS->ADCREF &=~SYS_ADCREF_REFSEL_Msk;
	}
}

static uint32_t ADC_seq2pos(uint32_t seq)
{
	uint32_t pos = 0;
	
	switch(seq)
	{
	case ADC_SEQ0: pos = 0;  break;
	case ADC_SEQ1: pos = 8;  break;
	}
	
	return pos;
}

/****************************************************************************************************************************************** 
* ��������: ADC_SEQ_Init()
* ����˵��:	ADC���г�ʼ��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t seq 			ָ��Ҫ���õ����У���Чֵ����ADC_SEQ0��ADC_SEQ1
*			ADC_SEQ_InitStructure * initStruct		����ADC�����趨��ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_SEQ_Init(ADC_TypeDef * ADCx, uint32_t seq, ADC_SEQ_InitStructure * initStruct)
{
	uint32_t pos = ADC_seq2pos(seq);
	
	ADCx->SEQTRG &= ~(0xFFu << pos);
	ADCx->SEQTRG |= (initStruct->trig_src << pos);
	
	ADCx->SMPTIM &= ~(0xFFu << pos);
	ADCx->SMPTIM |= ((initStruct->samp_tim - 4) << pos);
	
	ADCx->SMPNUM &= ~(0xFFu << pos);
	ADCx->SMPNUM |= ((initStruct->conv_cnt - 1) << pos);
	
	ADCx->IE |= (initStruct->EOCIntEn << pos);
	
	if(initStruct->EOCIntEn)
		NVIC_EnableIRQ(ADC_IRQn);
	
	__IO uint32_t * SEQxCHN = (seq == ADC_SEQ0) ? &ADCx->SEQ0CHN : &ADCx->SEQ1CHN;
	*SEQxCHN = 0;
	for(int i = 0; i < 8; i++)
	{
		*SEQxCHN |= initStruct->channels[i] << (i * 4);
		
		if(initStruct->channels[i] == 0xF)
			break;
	}
}

/****************************************************************************************************************************************** 
* ��������: ADC_CMP_Init()
* ����˵��:	ADC�ȽϹ��ܳ�ʼ��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t seq 			ָ��Ҫ���õ����У���Чֵ����ADC_SEQ0��ADC_SEQ1
*			ADC_CMP_InitStructure * initStruct		����ADC�ȽϹ����趨��ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_CMP_Init(ADC_TypeDef * ADCx, uint32_t seq, ADC_CMP_InitStructure * initStruct)
{
	if(seq == ADC_SEQ0)
		ADCx->SEQ0CHK = (initStruct->UpperLimit << ADC_SEQ0CHK_MAX_Pos) |
						(initStruct->LowerLimit << ADC_SEQ0CHK_MIN_Pos);
	else
		ADCx->SEQ1CHK = (initStruct->UpperLimit << ADC_SEQ1CHK_MAX_Pos) |
						(initStruct->LowerLimit << ADC_SEQ1CHK_MIN_Pos);
	
	if(initStruct->UpperLimitIEn)
		ADC_INTEn(ADCx, seq, ADC_IT_CMP_MAX);
	
	if(initStruct->LowerLimitIEn)
		ADC_INTEn(ADCx, seq, ADC_IT_CMP_MIN);
	
	if(initStruct->UpperLimitIEn || initStruct->LowerLimitIEn)
		NVIC_EnableIRQ(ADC_IRQn);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Open()
* ����˵��:	ADC���������������������Ӳ������ADCת��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Open(ADC_TypeDef * ADCx)
{
	ADCx->CR &= ~ADC_CR_PWDN_Msk;
	
	int clkdiv = ((ADCx->CR & ADC_CR_CLKDIV_Msk) >> ADC_CR_CLKDIV_Pos) + 1;
	
	for(int i = 0; i < clkdiv * 32; i++) {}	// �˳� Powerdown 32 ������ʱ�����ں���ܹ���
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Close()
* ����˵��:	ADC�رգ��޷������������Ӳ������ADCת��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Close(ADC_TypeDef * ADCx)
{
	ADCx->CR |=  ADC_CR_PWDN_Msk;
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Start()
* ����˵��:	�������ģʽ������ADCת��
* ��    ��: uint32_t ADC0_seq		ָ��Ҫ���õ�ADC0���У���ЧֵΪADC_SEQ0��ADC_SEQ1������ϣ�������λ�����㣩
*			uint32_t ADC1_seq		ָ��Ҫ���õ�ADC1���У���ЧֵΪADC_SEQ0��ADC_SEQ1������ϣ�������λ�����㣩
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Start(uint32_t ADC0_seq, uint32_t ADC1_seq)
{
	ADC0->START |= (ADC0_seq << ADC_START_ADC0SEQ0_Pos) |
				   (ADC1_seq << ADC_START_ADC1SEQ0_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Stop()
* ����˵��:	�������ģʽ��ֹͣADCת��
* ��    ��: uint32_t ADC0_seq		ָ��Ҫ���õ�ADC0���У���ЧֵΪADC_SEQ0��ADC_SEQ1������ϣ�������λ�����㣩
*			uint32_t ADC1_seq		ָ��Ҫ���õ�ADC1���У���ЧֵΪADC_SEQ0��ADC_SEQ1������ϣ�������λ�����㣩
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_Stop(uint32_t ADC0_seq, uint32_t ADC1_seq)
{
	ADC0->START &= ~((ADC0_seq << ADC_START_ADC0SEQ0_Pos) |
					 (ADC1_seq << ADC_START_ADC1SEQ0_Pos));
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Busy()
* ����˵��:	ADC æ��ѯ
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
* ��    ��: bool					true ����ת��	false ����
* ע������: ��
******************************************************************************************************************************************/
bool ADC_Busy(ADC_TypeDef * ADCx)
{
	if(ADCx == ADC0)
		return ADC0->START & ADC_START_ADC0BUSY_Msk;
	else
		return ADC0->START & ADC_START_ADC1BUSY_Msk;
}

/****************************************************************************************************************************************** 
* ��������:	ADC_Read()
* ����˵��:	��ȡָ��ͨ����ת�����
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t chn			Ҫ��ȡ��ͨ������ЧֵΪADC_CH0��ADC_CH1��... ...��ADC_CH10��ADC_CH11
* ��    ��: uint32_t				��ȡ����ת�����
* ע������: ��
******************************************************************************************************************************************/
uint32_t ADC_Read(ADC_TypeDef * ADCx, uint32_t chn)
{
	return ADCx->DATA[chn] & ADC_DATA_DATA_Msk;
}

/****************************************************************************************************************************************** 
* ��������:	ADC_DataAvailable()
* ����˵��:	ָ�������Ƿ������ݿɶ�ȡ
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t chn			Ҫ��ȡ��ͨ������ЧֵΪADC_CH0��ADC_CH1��... ...��ADC_CH10��ADC_CH11
* ��    ��: uint32_t				1 �����ݿɶ�ȡ    0 ������
* ע������: ���� ADC ת�������е��ô˺�������һ��������ת�����ʱ ADCx->DATA.FLAG λ�޷�����
******************************************************************************************************************************************/
uint32_t ADC_DataAvailable(ADC_TypeDef * ADCx, uint32_t chn)
{
	return (ADCx->DATA[chn] & ADC_DATA_FLAG_Msk) != 0;
}


/****************************************************************************************************************************************** 
* ��������:	ADC_INTEn()
* ����˵��:	�ж�ʹ��
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t seq			ָ��Ҫ���õ�ADC���У���ЧֵΪADC_SEQ0��ADC_SEQ1
* 			uint32_t it				interrupt type����Чֵ����ADC_IT_EOC��ADC_IT_CMP_MAX��ADC_IT_CMP_MIN ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_INTEn(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it)
{
	uint32_t pos = ADC_seq2pos(seq);
	
	ADCx->IE |= (it << pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_INTDis()
* ����˵��:	�жϽ�ֹ
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t seq			ָ��Ҫ���õ�ADC���У���ЧֵΪADC_SEQ0��ADC_SEQ1
* 			uint32_t it				interrupt type����Чֵ����ADC_IT_EOC��ADC_IT_CMP_MAX��ADC_IT_CMP_MIN ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_INTDis(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it)
{
	uint32_t pos = ADC_seq2pos(seq);
	
	ADCx->IE &= ~(it << pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_INTClr()
* ����˵��:	�жϱ�־���
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t seq			ָ��Ҫ���õ�ADC���У���ЧֵΪADC_SEQ0��ADC_SEQ1
* 			uint32_t it				interrupt type����Чֵ����ADC_IT_EOC��ADC_IT_CMP_MAX��ADC_IT_CMP_MIN ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void ADC_INTClr(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it)
{
	uint32_t pos = ADC_seq2pos(seq);
	
	ADCx->IF = (it << pos);
}

/****************************************************************************************************************************************** 
* ��������:	ADC_INTStat()
* ����˵��:	�ж�״̬��ѯ
* ��    ��: ADC_TypeDef * ADCx		ָ��Ҫ���õ�ADC����Чֵ����ADC0��ADC1
*			uint32_t seq			ָ��Ҫ��ѯ��ADC���У���ЧֵΪADC_SEQ0��ADC_SEQ1
* 			uint32_t it				interrupt type����Чֵ����ADC_IT_EOC��ADC_IT_CMP_MAX��ADC_IT_CMP_MIN ���䡰��
* ��    ��: uint32_t					1 �жϷ���    0 �ж�δ����
* ע������: ��
******************************************************************************************************************************************/
uint32_t ADC_INTStat(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it)
{
	uint32_t pos = ADC_seq2pos(seq);
	
	return (ADCx->IF & (it << pos)) ? 1 : 0;
}
