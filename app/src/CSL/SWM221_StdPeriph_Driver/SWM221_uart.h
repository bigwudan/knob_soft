#ifndef __SWM221_UART_H__
#define __SWM221_UART_H__

typedef struct {
	uint32_t Baudrate;
	
	uint8_t  DataBits;			//����λλ������ȡֵUART_DATA_8BIT��UART_DATA_9BIT
	
	uint8_t  Parity;			//��żУ��λ����ȡֵUART_PARITY_NONE��UART_PARITY_ODD��UART_PARITY_EVEN��UART_PARITY_ONE��UART_PARITY_ZERO
	
	uint8_t  StopBits;			//ֹͣλλ������ȡֵUART_STOP_1BIT��UART_STOP_2BIT
	
	uint8_t  RXThreshold;		//ȡֵ0--7
	uint8_t  RXThresholdIEn;	//��RX FIFO�����ݸ��� >  RXThresholdʱ�����ж�
	
	uint8_t  TXThreshold;		//ȡֵ0--7
	uint8_t  TXThresholdIEn;	//��TX FIFO�����ݸ��� <= TXThresholdʱ�����ж�
	
	uint8_t  TimeoutTime;		//��ʱʱ�� = TimeoutTime/(Baudrate/10) ��
	uint8_t  TimeoutIEn;		//��ʱ�жϣ�RX FIFO�ǿգ��ҳ��� TimeoutTime/(Baudrate/10) ��û����RX���Ͻ��յ�����ʱ�����ж�
} UART_InitStructure;


#define UART_DATA_8BIT		0
#define UART_DATA_9BIT		1

#define UART_PARITY_NONE	0
#define UART_PARITY_ODD		1
#define UART_PARITY_EVEN	3
#define UART_PARITY_ONE		5
#define UART_PARITY_ZERO	7

#define UART_STOP_1BIT		0
#define UART_STOP_2BIT		1

#define UART_RTS_1BYTE		0
#define UART_RTS_2BYTE		1
#define UART_RTS_4BYTE		2
#define UART_RTS_6BYTE		3

#define UART_ABR_RES_OK		1
#define UART_ABR_RES_ERR	2

#define UART_ERR_FRAME		1
#define UART_ERR_PARITY		2
#define UART_ERR_NOISE		3


/* Interrupt Type */
#define UART_IT_RX_THR		(1 << UART_CTRL_RXIE_Pos)	//RX FIFO Threshold, RX FIFO�����ݸ��� >  RXThreshold
#define UART_IT_RX_TOUT		(1 << UART_CTRL_TOIE_Pos)	//RX Timeout, ���� TimeoutTime/(Baudrate/10) ��û����RX���Ͻ��յ�����
#define UART_IT_TX_THR		(1 << UART_CTRL_TXIE_Pos)	//TX FIFO Threshold, TX FIFO�����ݸ��� <= TXThreshold
#define UART_IT_TX_DONE		(1 << UART_CTRL_TXDOIE_Pos)	//TX Done, ����FIFO���ҷ��ͷ�����λ�Ĵ����ѽ����һλ���ͳ�ȥ

#define UART_IT_LIN_DET		(1 << UART_LINCR_BRKDETIE_Pos)
#define UART_IT_LIN_GEN		(1 << UART_LINCR_GENBRKIE_Pos)


void UART_Init(UART_TypeDef * UARTx, UART_InitStructure * initStruct);	//UART���ڳ�ʼ��
void UART_Open(UART_TypeDef * UARTx);
void UART_Close(UART_TypeDef * UARTx);

void UART_WriteByte(UART_TypeDef * UARTx, uint8_t data);				//����һ���ֽ�����
uint32_t UART_ReadByte(UART_TypeDef * UARTx, uint32_t * data);			//��ȡһ���ֽ����ݣ���ָ�������Ƿ�Valid

uint32_t UART_IsTXBusy(UART_TypeDef * UARTx);
uint32_t UART_IsRXFIFOEmpty(UART_TypeDef * UARTx);						//����FIFO�Ƿ�գ������������Լ���UART_ReadByte()
uint32_t UART_IsTXFIFOFull(UART_TypeDef * UARTx);						//����FIFO�Ƿ����������������Լ���UART_WriteByte()


void UART_SetBaudrate(UART_TypeDef * UARTx, uint32_t baudrate);			//���ò�����
uint32_t UART_GetBaudrate(UART_TypeDef * UARTx);			 			//��ȡ��ǰʹ�õĲ�����

void UART_CTSConfig(UART_TypeDef * UARTx, uint32_t enable, uint32_t polarity);
uint32_t UART_CTSLineState(UART_TypeDef * UARTx);

void UART_RTSConfig(UART_TypeDef * UARTx, uint32_t enable, uint32_t polarity, uint32_t threshold);
uint32_t UART_RTSLineState(UART_TypeDef * UARTx);

void UART_LINConfig(UART_TypeDef * UARTx, uint32_t detectedLen, uint32_t detectedIEn, uint32_t generatedLen, uint32_t generatedIEn);
void UART_LINGenerate(UART_TypeDef * UARTx);

void UART_LININTEn(UART_TypeDef * UARTx, uint32_t it);
void UART_LININTDis(UART_TypeDef * UARTx, uint32_t it);
void UART_LININTClr(UART_TypeDef * UARTx, uint32_t it);
uint32_t UART_LININTStat(UART_TypeDef * UARTx, uint32_t it);

uint8_t UART_LIN_IDParity(uint8_t lin_id);
uint8_t UART_LIN_Checksum(uint8_t lin_id, uint8_t data[], uint32_t count, bool enhanced_checksum);

void UART_ABRStart(UART_TypeDef * UARTx, uint32_t detectChar);
uint32_t UART_ABRIsDone(UART_TypeDef * UARTx);


void UART_INTEn(UART_TypeDef * UARTx, uint32_t it);
void UART_INTDis(UART_TypeDef * UARTx, uint32_t it);
void UART_INTClr(UART_TypeDef * UARTx, uint32_t it);
uint32_t UART_INTStat(UART_TypeDef * UARTx, uint32_t it);

#endif //__SWM221_UART_H__