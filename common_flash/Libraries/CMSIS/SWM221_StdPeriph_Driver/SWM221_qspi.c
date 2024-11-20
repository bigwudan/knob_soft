/****************************************************************************************************************************************** 
* �ļ�����: SWM342_qspi.c
* ����˵��:	SWM342��Ƭ����QSPIģ��������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������: 
* �汾����:	V1.1.0		2017��10��25��
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
#include "SWM221_qspi.h"


static uint8_t AddressSize;


/****************************************************************************************************************************************** 
* ��������:	QSPI_Init()
* ����˵��:	QSPI ��ʼ��
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			QSPI_InitStructure * initStruct		����QSPI�ӿ�����趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_Init(QSPI_TypeDef * QSPIx, QSPI_InitStructure * initStruct)
{
	switch((uint32_t)QSPIx)
	{
	case ((uint32_t)QSPI0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_QSPI0_Pos);
		break;
	}
	
	QSPI_Close(QSPIx);
	
	QSPIx->CR = (0						<< QSPI_CR_SSHIFT_Pos) |
				(7						<< QSPI_CR_FFTHR_Pos)  |
				(initStruct->IntEn		<< QSPI_CR_ERRIE_Pos)  |
				((initStruct->ClkDiv-1)	<< QSPI_CR_CLKDIV_Pos);
	
	QSPIx->DCR = (initStruct->ClkMode 	<< QSPI_DCR_CLKMOD_Pos) |
				 (3						<< QSPI_DCR_CSHIGH_Pos) |
				 (initStruct->Size		<< QSPI_DCR_FLSIZE_Pos);
	
	AddressSize = initStruct->Size / 8;
	
	QSPIx->SSHIFT = ((initStruct->SampleShift & 0x0F) << QSPI_SSHIFT_CYCLE_Pos) |
					(2								  << QSPI_SSHIFT_SPACE_Pos);
	
	QSPIx->FCR = 0x1B;
	if(initStruct->IntEn)
	{
		switch((uint32_t)QSPIx)
		{
		case ((uint32_t)QSPI0):	NVIC_EnableIRQ(QSPI0_IRQn); break;
		}
	}
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_Open()
* ����˵��:	QSPI�ӿڴ�
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_Open(QSPI_TypeDef * QSPIx)
{
	QSPIx->CR |= QSPI_CR_EN_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_Close()
* ����˵��:	QSPI�ӿڹر�
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_Close(QSPI_TypeDef * QSPIx)
{
	QSPIx->CR &= ~QSPI_CR_EN_Msk;
}


void QSPI_CmdStructClear(QSPI_CmdStructure * cmdStruct)
{
	cmdStruct->Instruction		  = 0;
	cmdStruct->InstructionMode	  = 0;
	cmdStruct->Address 			  = 0;
	cmdStruct->AddressMode		  = 0;
	cmdStruct->AddressSize		  = 0;
	cmdStruct->AlternateBytes 	  = 0;
	cmdStruct->AlternateBytesMode = 0;
	cmdStruct->AlternateBytesSize = 0;
	cmdStruct->DummyCycles		  = 0;
	cmdStruct->DataMode			  = 0;
	cmdStruct->DataCount		  = 0;
	cmdStruct->SendInstOnlyOnce   = 0;
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_Command()
* ����˵��:	QSPI����ִ��
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			QSPI_CmdStructure * cmdStruct	Ҫִ�����������
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_Command(QSPI_TypeDef * QSPIx, uint8_t cmdMode, QSPI_CmdStructure * cmdStruct)
{
	if(cmdStruct->AlternateBytesMode != QSPI_PhaseMode_None)
		QSPIx->ABR = cmdStruct->AlternateBytes;
	
	if(cmdStruct->DataMode != QSPI_PhaseMode_None)
		QSPIx->DLR = cmdStruct->DataCount - 1;
	
	QSPIx->CCR = (cmdStruct->Instruction		<< QSPI_CCR_CODE_Pos)   |
				 (cmdStruct->InstructionMode	<< QSPI_CCR_IMODE_Pos)  |
				 (cmdStruct->AddressMode		<< QSPI_CCR_AMODE_Pos)  |
				 (cmdStruct->AddressSize		<< QSPI_CCR_ASIZE_Pos)  |
				 (cmdStruct->AlternateBytesMode	<< QSPI_CCR_ABMODE_Pos) |
				 (cmdStruct->AlternateBytesSize	<< QSPI_CCR_ABSIZE_Pos) |
				 (cmdStruct->DummyCycles		<< QSPI_CCR_DUMMY_Pos)  |
				 (cmdStruct->DataMode			<< QSPI_CCR_DMODE_Pos)  |
				 (cmdMode						<< QSPI_CCR_MODE_Pos)   |
				 (cmdStruct->SendInstOnlyOnce	<< QSPI_CCR_SIOO_Pos);
	
	if(cmdStruct->AddressMode != QSPI_PhaseMode_None)
		QSPIx->AR = cmdStruct->Address;
    
	for(int i = 0; i < 3; i++) __NOP();
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_Erase()
* ����˵��:	QSPI Flash ����
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			uint8_t cmd				�������� ����Чֵ���� QSPI_CMD_ERASE_SECTOR��QSPI_CMD_ERASE_BLOCK32KB��QSPI_CMD_ERASE_BLOCK64KB
*			uint32_t addr			Ҫ������ SPI Flash ��ַ
*			uint8_t wait			�Ƿ�ȴ� SPI Flash ��ɲ���������1 �ȴ����   0 ��������
* ��    ��: ��
* ע������: �� wait == 0 �������أ���Ҫ�������� QSPI_FlashBusy() ��� SPI Flash ��ɲ�������ִ�ж���д����
******************************************************************************************************************************************/
void QSPI_Erase(QSPI_TypeDef * QSPIx, uint8_t cmd, uint32_t addr, uint8_t wait)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = cmd;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_1bit;
	cmdStruct.AddressSize		 = AddressSize;
	cmdStruct.Address			 = addr;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = QSPI_PhaseMode_None;
	
	QSPI_WriteEnable(QSPIx);
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectWrite, &cmdStruct);
	
	while(QSPI_Busy(QSPIx)) __NOP();
	
	if(wait)
		while(QSPI_FlashBusy(QSPIx)) __NOP();
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_Write_()
* ����˵��:	QSPI Flash д��
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			uint32_t addr			Ҫд�뵽�� SPI Flash ��ַ
*			uint8_t buff[]			Ҫд�� SPI Flash ������
*			uint32_t count			Ҫд�� SPI Flash �����ݸ�������� 256����д�����ݲ��ܿ�ҳ
*			uint8_t data_width		д��ʹ�õ������߸�������Чֵ���� 1��4
*			uint8_t data_phase		�Ƿ��ڴ˺�����ִ�����ݽ׶Σ����񣬿��ں���ͨ�� DMA ʵ�ָ���Ч��д��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_Write_(QSPI_TypeDef * QSPIx, uint32_t addr, uint8_t buff[], uint32_t count, uint8_t data_width, uint8_t data_phase)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	uint8_t instruction, dataMode;
	switch(data_width)
	{
	case 1:
		instruction = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_PAGE_PROGRAM      : QSPI_CMD_PAGE_PROGRAM;
		dataMode 	= QSPI_PhaseMode_1bit;
		break;
	
	case 4:
		instruction = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_PAGE_PROGRAM_4bit : QSPI_CMD_PAGE_PROGRAM_4bit;
		dataMode 	= QSPI_PhaseMode_4bit;
		break;
	}
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = instruction;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_1bit;
	cmdStruct.AddressSize 		 = AddressSize;
	cmdStruct.Address 			 = addr;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = dataMode;
	cmdStruct.DataCount 		 = count;
	
	QSPI_WriteEnable(QSPIx);
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectWrite, &cmdStruct);
	
	if(data_phase == 0)
		return;
	
	if(((uint32_t)buff & (4 - 1)) == 0)	// word aligned
	{
		uint32_t n_word = count >> 2;
		
		for(int i = 0; i < n_word; i++)
		{
			uint32_t * p_word = (uint32_t *)buff;
			
			while(QSPI_FIFOSpace(QSPIx) < 4) __NOP();
			
			QSPIx->DRW = p_word[i];
		}
		
		if((count & (4 - 1)) >> 1)
		{
			uint16_t * p_half = (uint16_t *)&buff[n_word << 2];
			
			while(QSPI_FIFOSpace(QSPIx) < 2) __NOP();
			
			QSPIx->DRH = p_half[0];
		}
		
		if(count & (2 - 1))
		{
			while(QSPI_FIFOSpace(QSPIx) < 1) __NOP();
			
			QSPIx->DRB = buff[count - 1];
		}
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			while(QSPI_FIFOSpace(QSPIx) < 1) __NOP();
			
			QSPIx->DRB = buff[i];
		}
	}
	
	while(QSPI_Busy(QSPIx)) __NOP();
	
	while(QSPI_FlashBusy(QSPIx)) __NOP();
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_Read_()
* ����˵��:	QSPI Flash ��ȡ
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			uint32_t addr			Ҫ��ȡ�Ե� SPI Flash ��ַ
*			uint8_t buff[]			��ȡ��������д���������
*			uint32_t count			Ҫ��ȡ���ݵĸ���
*			uint8_t addr_width		��ȡʹ�õĵ�ַ�߸�������Чֵ���� 1��2��4
*			uint8_t data_width		��ȡʹ�õ������߸�������Чֵ���� 1��2��4
*			uint8_t data_phase		�Ƿ��ڴ˺�����ִ�����ݽ׶Σ����񣬿��ں���ͨ�� DMA ʵ�ָ���Ч�Ķ�ȡ
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_Read_(QSPI_TypeDef * QSPIx, uint32_t addr, uint8_t buff[], uint32_t count, uint8_t addr_width, uint8_t data_width, uint8_t data_phase)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	uint8_t instruction, addressMode, dataMode, dummyCycles;
	uint8_t alternateBytesMode, alternateBytesSize, alternateBytes;
	switch((addr_width << 4) | data_width)
	{
	case 0x11:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ        : QSPI_CMD_FAST_READ;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_1bit;
		break;
	
	case 0x12:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_2bit   : QSPI_CMD_FAST_READ_2bit;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_2bit;
		break;
	
	case 0x22:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_IO2bit : QSPI_CMD_FAST_READ_IO2bit;
		addressMode 	   = QSPI_PhaseMode_2bit;
		alternateBytesMode = QSPI_PhaseMode_2bit;
		alternateBytesSize = QSPI_PhaseSize_8bit;
		alternateBytes     = 0xFF;
		dummyCycles        = 0;
		dataMode 		   = QSPI_PhaseMode_2bit;
		break;
	
	case 0x14:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_4bit   : QSPI_CMD_FAST_READ_4bit;
		addressMode 	   = QSPI_PhaseMode_1bit;
		alternateBytesMode = QSPI_PhaseMode_None;
		alternateBytesSize = 0;
		dummyCycles        = 8;
		dataMode 		   = QSPI_PhaseMode_4bit;
		break;
	
	case 0x44:
		instruction 	   = (AddressSize == QSPI_PhaseSize_32bit) ? QSPI_C4B_FAST_READ_IO4bit : QSPI_CMD_FAST_READ_IO4bit;
		addressMode 	   = QSPI_PhaseMode_4bit;
		alternateBytesMode = QSPI_PhaseMode_4bit;
		alternateBytesSize = QSPI_PhaseSize_8bit;
		alternateBytes     = 0xFF;
		dummyCycles        = 4;
		dataMode 		   = QSPI_PhaseMode_4bit;
		break;
	}
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = instruction;
	cmdStruct.AddressMode 		 = addressMode;
	cmdStruct.AddressSize 		 = AddressSize;
	cmdStruct.Address 			 = addr;
	cmdStruct.AlternateBytesMode = alternateBytesMode;
	cmdStruct.AlternateBytesSize = alternateBytesSize;
	cmdStruct.AlternateBytes 	 = alternateBytes;
	cmdStruct.DummyCycles 		 = dummyCycles;
	cmdStruct.DataMode 			 = dataMode;
	cmdStruct.DataCount 		 = count;
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectRead, &cmdStruct);
	
	if(data_phase == 0)
		return;
	
	if(((uint32_t)buff & (4 - 1)) == 0)	// word aligned
	{
		uint32_t n_word = count >> 2;
		
		for(int i = 0; i < n_word; i++)
		{
			uint32_t * p_word = (uint32_t *)buff;
			
			while(QSPI_FIFOCount(QSPIx) < 4) __NOP();
			
			p_word[i] = QSPIx->DRW;
		}
		
		if((count & (4 - 1)) >> 1)
		{
			uint16_t * p_half = (uint16_t *)&buff[n_word << 2];
			
			while(QSPI_FIFOCount(QSPIx) < 2) __NOP();
			
			p_half[0] = QSPIx->DRH;
		}
		
		if(count & (2 - 1))
		{
			while(QSPI_FIFOCount(QSPIx) < 1) __NOP();
			
			buff[count - 1] = QSPIx->DRB;
		}
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			while(QSPI_FIFOCount(QSPIx) < 1) __NOP();
			
			buff[i] = QSPIx->DRB;
		}
	}
    
	QSPI_Abort(QSPIx);
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_FlashBusy()
* ����˵��:	QSPI Flash æ��ѯ
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* ��    ��: bool					SPI Flash �Ƿ�����ִ���ڲ�����д����
* ע������: ��
******************************************************************************************************************************************/
bool QSPI_FlashBusy(QSPI_TypeDef * QSPIx)
{
	uint16_t reg = QSPI_ReadReg(QSPIx, QSPI_CMD_READ_STATUS_REG1, 1);
	
	bool busy = (reg & (1 << QSPI_STATUS_REG1_BUSY_Pos));
	
	return busy;
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_QuadState()
* ����˵��:	QSPI Flash QE ��ѯ
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* ��    ��: uint8_t					1 QE ʹ��   0 QE ��ֹ
* ע������: ��ͬƷ�� SPI Flash �У�QE �� Status Register �е�λ�ÿ��ܲ�ͬ
******************************************************************************************************************************************/
uint8_t QSPI_QuadState(QSPI_TypeDef * QSPIx)
{
	uint8_t reg = QSPI_ReadReg(QSPIx, QSPI_CMD_READ_STATUS_REG2, 1);
	
	return (reg & (1 << QSPI_STATUS_REG2_QUAD_Pos)) ? 1 : 0;
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_QuadSwitch()
* ����˵��:	QSPI Flash QE ����
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			uint8_t on				1 QE ʹ��   0 QE ��ֹ
* ��    ��: ��
* ע������: ��ͬƷ�� SPI Flash �У�QE �� Status Register �е�λ�ÿ��ܲ�ͬ
******************************************************************************************************************************************/
void QSPI_QuadSwitch(QSPI_TypeDef * QSPIx, uint8_t on)
{
	uint8_t reg = QSPI_ReadReg(QSPIx, QSPI_CMD_READ_STATUS_REG2, 1);
	
	if(on)
		reg |=  (1 << QSPI_STATUS_REG2_QUAD_Pos);
	else
		reg &= ~(1 << QSPI_STATUS_REG2_QUAD_Pos);
	
	QSPI_WriteEnable(QSPIx);
	
	QSPI_WriteReg(QSPIx, QSPI_CMD_WRITE_STATUS_REG2, reg, 1);
	
	while(QSPI_FlashBusy(QSPIx)) __NOP();
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_SendCmd()
* ����˵��:	QSPI ����ͣ�������ֻ������׶ε����
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			uint8_t cmd				Ҫ���͵�����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_SendCmd(QSPI_TypeDef * QSPIx, uint8_t cmd)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = cmd;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_None;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = QSPI_PhaseMode_None;
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectWrite, &cmdStruct);
	
	while(QSPI_Busy(QSPIx)) __NOP();
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_ReadReg()
* ����˵��:	QSPI Flash �Ĵ�����ȡ��������ֻ��������ݽ׶Σ������ݲ�����4�ֽڵ����
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			uint8_t cmd				�Ĵ�����ȡ����
*			uint8_t n_bytes			Ҫ��ȡ���ֽ�������ȡֵ 1��2��3��4
* ��    ��: uint32_t				��ȡ�������ݣ�MSB
* ע������: ��
******************************************************************************************************************************************/
uint32_t QSPI_ReadReg(QSPI_TypeDef * QSPIx, uint8_t cmd, uint8_t n_bytes)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = cmd;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_None;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = QSPI_PhaseMode_1bit;
	cmdStruct.DataCount 		 = n_bytes;
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectRead, &cmdStruct);
	
	while(QSPI_FIFOCount(QSPIx) < n_bytes) __NOP();
	
	uint32_t data = 0;
	for(int i = n_bytes; i > 0; i--)
		((uint8_t *)&data)[i-1] = QSPIx->DRB;
	
	return data;
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_WriteReg()
* ����˵��:	QSPI Flash �Ĵ���д�룬������ֻ��������ݽ׶Σ������ݲ�����4�ֽڵ����
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
*			uint8_t cmd				�Ĵ���д������
*			uint32_t data			Ҫд��Ĵ��������ݣ�MSB
*			uint8_t n_bytes			Ҫд��Ĵ������ֽ�������ȡֵ 1��2��3��4
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_WriteReg(QSPI_TypeDef * QSPIx, uint8_t cmd, uint32_t data, uint8_t n_bytes)
{
	QSPI_CmdStructure cmdStruct;
	QSPI_CmdStructClear(&cmdStruct);
	
	cmdStruct.InstructionMode 	 = QSPI_PhaseMode_1bit;
	cmdStruct.Instruction 		 = cmd;
	cmdStruct.AddressMode 		 = QSPI_PhaseMode_None;
	cmdStruct.AlternateBytesMode = QSPI_PhaseMode_None;
	cmdStruct.DummyCycles 		 = 0;
	cmdStruct.DataMode 			 = QSPI_PhaseMode_1bit;
	cmdStruct.DataCount 		 = n_bytes;
	
	QSPI_Command(QSPIx, QSPI_Mode_IndirectWrite, &cmdStruct);
	
	for(int i = n_bytes; i > 0; i--)
		QSPIx->DRB = ((uint8_t *)&data)[i-1];
	
	while(QSPI_Busy(QSPIx)) __NOP();
}


/****************************************************************************************************************************************** 
* ��������:	QSPI_INTEn()
* ����˵��:	�ж�ʹ��
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* 			uint32_t it				interrupt type����ȡֵ QSPI_IT_ERR��QSPI_IT_DONE��QSPI_IT_FFTHR��QSPI_IT_PSMAT��QSPI_IT_TO ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_INTEn(QSPI_TypeDef * QSPIx, uint32_t it)
{
	QSPIx->CR |=  (it << 16);
}

/****************************************************************************************************************************************** 
* ��������:	QSPI_INTDis()
* ����˵��:	�жϽ�ֹ
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* 			uint32_t it				interrupt type����ȡֵ QSPI_IT_ERR��QSPI_IT_DONE��QSPI_IT_FFTHR��QSPI_IT_PSMAT��QSPI_IT_TO ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_INTDis(QSPI_TypeDef * QSPIx, uint32_t it)
{
	QSPIx->CR &= ~(it << 16);
}

/****************************************************************************************************************************************** 
* ��������:	QSPI_INTClr()
* ����˵��:	�жϱ�־���
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* 			uint32_t it				interrupt type����ȡֵ QSPI_IT_ERR��QSPI_IT_DONE��QSPI_IT_PSMAT��QSPI_IT_TO ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void QSPI_INTClr(QSPI_TypeDef * QSPIx, uint32_t it)
{
	QSPIx->FCR = it;
}

/****************************************************************************************************************************************** 
* ��������:	QSPI_INTStat()
* ����˵��:	�ж�״̬��ѯ
* ��    ��: QSPI_TypeDef * QSPIx	ָ��Ҫ�����õ�QSPI�ӿڣ���Чֵ����QSPI0
* 			uint32_t it				interrupt type����ȡֵ QSPI_IT_ERR��QSPI_IT_DONE��QSPI_IT_FFTHR��QSPI_IT_PSMAT��QSPI_IT_TO ���䡰��
* ��    ��: uint32_t				0 �ж�δ����    ��0 �ж��ѷ���
* ע������: ��
******************************************************************************************************************************************/
uint32_t QSPI_INTStat(QSPI_TypeDef * QSPIx, uint32_t it)
{
	return QSPIx->SR & it;
}
