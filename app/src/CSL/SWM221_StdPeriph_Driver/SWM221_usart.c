/****************************************************************************************************************************************** 
* �ļ�����:	SWM221_usart.c
* ����˵��:	SWM221��Ƭ����USART���ڹ���������
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
#include "SWM221_usart.h"


/****************************************************************************************************************************************** 
* ��������:	USART_Init()
* ����˵��:	USART���ڳ�ʼ��
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�UART���ڣ���Чֵ����USART0
*			USART_InitStructure * initStruct    ����USART��������趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void USART_Init(USART_TypeDef * USARTx, USART_InitStructure * initStruct)
{
	switch((uint32_t)USARTx)
	{
	case ((uint32_t)USART0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_USART0_Pos);
		break;
	}
	
	USART_Close(USARTx);	//һЩ�ؼ��Ĵ���ֻ���ڴ��ڹر�ʱ����
	
	USARTx->MR = (0 << USART_MR_MODE_Pos)  |
				 (0 << USART_MR_CLKS_Pos)  |
				 (1 << USART_MR_OVER8_Pos) |
				 (initStruct->DataBits << USART_MR_NBDATA_Pos) |
				 (initStruct->Parity   << USART_MR_PARITY_Pos) |
				 (initStruct->StopBits << USART_MR_NBSTOP_Pos);
	
	USARTx->BAUD = ((SystemCoreClock/initStruct->Baudrate / 8) << USART_BAUD_IDIV_Pos) |
				   ((SystemCoreClock/initStruct->Baudrate % 8) << USART_BAUD_FDIV_Pos);
	
	USARTx->RXTO = initStruct->TimeoutTime;
	
	USARTx->IER = (initStruct->RXReadyIEn << USART_IER_RXRDY_Pos) |
				  (initStruct->TXReadyIEn << USART_IER_TXRDY_Pos) |
				  (initStruct->TimeoutIEn << USART_IER_RXTO_Pos);
	
	if(initStruct->RXReadyIEn | initStruct->TXReadyIEn | initStruct->TimeoutIEn)
	{
		switch((uint32_t)USARTx)
		{
		case ((uint32_t)USART0): NVIC_EnableIRQ(USART0_IRQn); break;
		}
	}
}


/****************************************************************************************************************************************** 
* ��������:	USART_Open()
* ����˵��:	USART���ڴ�
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�USART���ڣ���Чֵ����USART0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void USART_Open(USART_TypeDef * USARTx)
{
	USARTx->CR = USART_CR_RXEN_Msk |
				 USART_CR_TXEN_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	USART_Close()
* ����˵��:	USART���ڹر�
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�USART���ڣ���Чֵ����USART0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void USART_Close(USART_TypeDef * USARTx)
{
	USARTx->CR = USART_CR_RXDIS_Msk |
				 USART_CR_TXDIS_Msk |
				 USART_CR_RSTRX_Msk |
				 USART_CR_RSTTX_Msk |
				 USART_CR_RSTSTA_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	USART_SetBaudrate()
* ����˵��:	���ò�����
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�USART���ڣ���Чֵ����USART0
*			uint32_t baudrate		Ҫ���õĲ�����
* ��    ��: ��
* ע������: ��Ҫ�ڴ��ڹ���ʱ���Ĳ����ʣ�ʹ�ô˺���ǰ���ȵ���USART_Close()�رմ���
******************************************************************************************************************************************/
void USART_SetBaudrate(USART_TypeDef * USARTx, uint32_t baudrate)
{	
	USARTx->BAUD = ((SystemCoreClock/baudrate / 8) << USART_BAUD_IDIV_Pos) |
				   ((SystemCoreClock/baudrate % 8) << USART_BAUD_FDIV_Pos);
}


/****************************************************************************************************************************************** 
* ��������:	USART_GetBaudrate()
* ����˵��:	��ѯ������
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�USART���ڣ���Чֵ����USART0
* ��    ��: uint32_t				��ǰ������
* ע������: ��
******************************************************************************************************************************************/
uint32_t USART_GetBaudrate(USART_TypeDef * USARTx)
{	
	return SystemCoreClock/(((USARTx->BAUD & USART_BAUD_IDIV_Msk) >> USART_BAUD_IDIV_Pos) * 8 +
							((USARTx->BAUD & USART_BAUD_FDIV_Msk) >> USART_BAUD_FDIV_Pos));
}


/****************************************************************************************************************************************** 
* ��������:	USART_LINConfig()
* ����˵��:	USART LIN ģʽ����
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�USART���ڣ���Чֵ����USART0
*			uint32_t mode			LIN ģʽѡ�񣬿�ȡֵ�� USART_LIN_MASTER��USART_LIN_SLAVE
*			uint32_t checksum		У������ͣ���ȡֵ USART_CHECKSUM_LIN13��USART_CHECKSUM_LIN20
*			uint32_t it				interrupt type����Чֵ�� USART_IT_LIN_ID��USART_IT_LIN_DONE ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void USART_LINConfig(USART_TypeDef * USARTx, uint32_t mode, uint32_t checksum, uint32_t it)
{
	USARTx->MR &= ~USART_MR_MODE_Msk;
	USARTx->MR |= (mode << USART_MR_MODE_Pos);
	
	USARTx->LINMR = (USART_LIN_IGNORE << USART_LINMR_NACT_Pos)   |
					(0				  << USART_LINMR_PARDIS_Pos) |
					(0				  << USART_LINMR_CHKDIS_Pos) |
					(checksum		  << USART_LINMR_CHKTYP_Pos) |
					(0				  << USART_LINMR_RDLMOD_Pos) |
					(0				  << USART_LINMR_FSMDIS_Pos) |
					(0				  << USART_LINMR_SYNCDIS_Pos);
	
	USART_INTEn(USARTx, it);
}


/****************************************************************************************************************************************** 
* ��������:	USART_LINStart()
* ����˵��:	USART LIN Master ��������
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�USART���ڣ���Чֵ����USART0
*			uint32_t slave_id		�ӻ� ID
*			uint32_t action			��Ӧ�׶ε�������������ȡֵ�� USART_LIN_PUBLISH��USART_LIN_SUBSCRIBE��USART_LIN_IGNORE
*			uint32_t count			��Ӧ�׶����ݸ���
*			uint32_t checksum		У������ͣ���ȡֵ USART_CHECKSUM_LIN13��USART_CHECKSUM_LIN20
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void USART_LINStart(USART_TypeDef * USARTx, uint32_t slave_id, uint32_t action, uint32_t count, uint32_t checksum)
{
	USARTx->LINMR &= ~(USART_LINMR_NACT_Msk | USART_LINMR_DLC_Msk | USART_LINMR_CHKTYP_Msk);
	USARTx->LINMR |= (action 		<< USART_LINMR_NACT_Pos) |
					 ((count - 1)	<< USART_LINMR_DLC_Pos)  |
					 (checksum 		<< USART_LINMR_CHKTYP_Pos);
	
	USARTx->LINID = slave_id;
}


/****************************************************************************************************************************************** 
* ��������:	USART_LINResponse()
* ����˵��:	USART LIN Slave ��Ӧ����
* ��    ��: USART_TypeDef * USARTx	ָ��Ҫ�����õ�USART���ڣ���Чֵ����USART0
*			uint32_t action			��Ӧ�׶εĴӻ���������ȡֵ�� USART_LIN_PUBLISH��USART_LIN_SUBSCRIBE��USART_LIN_IGNORE
*			uint32_t count			��Ӧ�׶����ݸ���
*			uint32_t checksum		У������ͣ���ȡֵ USART_CHECKSUM_LIN13��USART_CHECKSUM_LIN20
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void USART_LINResponse(USART_TypeDef * USARTx, uint32_t action, uint32_t count, uint32_t checksum)
{
	USARTx->LINMR &= ~(USART_LINMR_NACT_Msk | USART_LINMR_DLC_Msk | USART_LINMR_CHKTYP_Msk);
	USARTx->LINMR |= (action 		<< USART_LINMR_NACT_Pos) |
					 ((count - 1)	<< USART_LINMR_DLC_Pos)  |
					 (checksum 		<< USART_LINMR_CHKTYP_Pos);
}
