#ifndef __SWM221_USART_H__
#define __SWM221_USART_H__


typedef struct {
	uint32_t Baudrate;
	
	uint8_t  DataBits;			//����λλ������ȡֵUSART_DATA_5BIT��USART_DATA_6BIT��USART_DATA_7BIT��USART_DATA_8BIT��USART_DATA_9BIT
	
	uint8_t  Parity;			//��żУ��λ����ȡֵUSART_PARITY_NONE��USART_PARITY_EVEN��USART_PARITY_ODD��USART_PARITY_ZERO��USART_PARITY_ONE
	
	uint8_t  StopBits;			//ֹͣλλ������ȡֵUSART_STOP_1BIT��USART_STOP_1BIT_5��USART_STOP_2BIT
	
	uint8_t  RXReadyIEn;		//RHR �ǿգ����Զ�ȡ
	uint8_t  TXReadyIEn;		//THR   �գ�����д��
	uint8_t  TimeoutIEn;		//��ʱ�жϣ����� TimeoutTime/Baudrate ��û����RX���Ͻ��յ�����ʱ�����ж�
	uint16_t TimeoutTime;		//��ʱʱ�� = TimeoutTime/Baudrate ��
} USART_InitStructure;


#define USART_DATA_5BIT		0
#define USART_DATA_6BIT		1
#define USART_DATA_7BIT		2
#define USART_DATA_8BIT		3
#define USART_DATA_9BIT		(1 << 11)

#define USART_PARITY_NONE	4
#define USART_PARITY_EVEN	0
#define USART_PARITY_ODD	1
#define USART_PARITY_ZERO	2
#define USART_PARITY_ONE	3

#define USART_STOP_1BIT		0
#define USART_STOP_1BIT_5	1	//1.5bit
#define USART_STOP_2BIT		2

#define USART_LIN_MASTER	10
#define USART_LIN_SLAVE		11

#define USART_LIN_PUBLISH	0
#define USART_LIN_SUBSCRIBE	1
#define USART_LIN_IGNORE	2

#define USART_CHECKSUM_LIN13	1
#define USART_CHECKSUM_LIN20	0


/* Interrupt Type */
#define USART_IT_RX_RDY			USART_IER_RXRDY_Msk		//RHR �ǿգ����Զ�ȡ
#define USART_IT_TX_RDY			USART_IER_TXRDY_Msk		//THR   �գ�����д��
#define USART_IT_RX_TO			USART_IER_RXTO_Msk		//���ճ�ʱ
#define USART_IT_TX_EMPTY		USART_IER_TXEMPTY_Msk	//THR �� ������λ�Ĵ����Կ�
#define USART_IT_ERR_OVR		USART_IER_OVRERR_Msk	//�Ƴ�����
#define USART_IT_ERR_FRAME		USART_IER_FRAMERR_Msk	//֡��ʽ����
#define USART_IT_ERR_PARITY		USART_IER_PARITYERR_Msk	//У�����
#define USART_IT_LIN_BRK		USART_IER_BRK_Msk		//LIN Break Sent or Received
#define USART_IT_LIN_ID			USART_IER_ID_Msk		//LIN Identifier Sent or Received
#define USART_IT_LIN_DONE		USART_IER_DONE_Msk
#define USART_IT_LIN_BITERR		USART_ISR_BITERR_Msk
#define USART_IT_LIN_SYNCERR	USART_ISR_SYNCERR_Msk
#define USART_IT_LIN_IDERR		USART_ISR_IDERR_Msk
#define USART_IT_LIN_CHKERR		USART_ISR_CHKERR_Msk
#define USART_IT_LIN_NAKERR		USART_ISR_NAKERR_Msk
#define USART_IT_LIN_HDRTO		USART_ISR_HDRTO_Msk


void USART_Init(USART_TypeDef * UARTx, USART_InitStructure * initStruct);	//UART���ڳ�ʼ��
void USART_Open(USART_TypeDef * UARTx);
void USART_Close(USART_TypeDef * UARTx);

void USART_SetBaudrate(USART_TypeDef * UARTx, uint32_t baudrate);			//���ò�����
uint32_t USART_GetBaudrate(USART_TypeDef * UARTx);			 				//��ȡ��ǰʹ�õĲ�����

void USART_LINConfig(USART_TypeDef * USARTx, uint32_t mode, uint32_t checksum, uint32_t it);
void USART_LINStart(USART_TypeDef * USARTx, uint32_t slave_id, uint32_t action, uint32_t count, uint32_t checksum);
void USART_LINResponse(USART_TypeDef * USARTx, uint32_t action, uint32_t count, uint32_t checksum);


static inline void USART_Write(USART_TypeDef * USARTx, uint32_t data)
{
	USARTx->THR = data;
}


static inline uint32_t USART_Read(USART_TypeDef * USARTx)
{
	return USARTx->RHR & USART_RHR_DATA_Msk;
}


static inline void USART_INTEn(USART_TypeDef * USARTx, uint32_t it)
{
	USARTx->IER = it;
}


static inline void USART_INTDis(USART_TypeDef * USARTx, uint32_t it)
{
	USARTx->IDR = it;
}


static inline void USART_INTClr(USART_TypeDef * USARTx, uint32_t it)
{
	if(it & USART_IT_RX_TO)
	{
		USARTx->CR = USART_CR_STTTO_Msk;
	}
	
	if(it & (USART_IT_ERR_OVR |
			 USART_IT_ERR_FRAME |
			 USART_IT_ERR_PARITY |
			 USART_IT_LIN_BRK |
			 USART_IT_LIN_ID |
			 USART_IT_LIN_DONE |
			 USART_IT_LIN_BITERR |
			 USART_IT_LIN_SYNCERR |
			 USART_IT_LIN_IDERR |
			 USART_IT_LIN_CHKERR |
			 USART_IT_LIN_NAKERR |
			 USART_IT_LIN_HDRTO))
	{
		USARTx->CR = USART_CR_RSTSTA_Msk;
	}
}


static inline uint32_t USART_INTStat(USART_TypeDef * USARTx, uint32_t it)
{
	return USARTx->ISR & it;
}


#endif //__SWM221_USART_H__
