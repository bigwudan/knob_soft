/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.c
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			   bsp = Borad surport packet �弶֧�ְ�
*	�޸ļ�¼ :
*		�汾��  ����         ����       ˵��
*		V1.0    2022-05-29  Eric203    ��ʽ����
*
*	Copyright (C), 2020-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"
/** SysTick ticks */
static volatile uint32_t Systick_Tick = 0;
void systick_init(void)
{
    Systick_Tick = 0;
    /* SystemFrequency / 1000  =>  1ms???? */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1) __NOP();
    }
#if defined(__PERFORMANCE_COUNTER_H__)
    init_cycle_counter(true);
#endif
}
static uint8_t QSPI_Multiplex_Flag = 0;//0: Init    1: LCD    2: Flash
void qspi_multiplex_flash(void)
{
#if CFG_LCD_IF == CFG_LCD_IF_QSPI
    if (2 != QSPI_Multiplex_Flag) {
        QSPI_Multiplex_Flag = 2;
				qspi_flash_init();
    }
#endif
}


/**
 * @brief ??????????????
 */
void uart_debug_init(void)
{

}

static void nvic_config(void)
{

}
/**
 * @brief 
 */
void board_init(void)
{
    SystemInit();
    nvic_config();
    uart_debug_init();
    systick_init();
		qspi_multiplex_flash();
}



uint32_t HAL_GetTick (void) 
{
	static uint32_t ticks = 0U;
//	uint32_t i;
//	
//	for (i = (SystemCoreClock >> 14U); i > 0U; i--) 
//	{
//		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
//		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
//	}
	
	return ++ticks;
}

void HAL_Delay(uint32_t Delay)
{

}

/*
*********************************************************************************************************
*	�� �� ��: SystemClock_Config
*	����˵��: ��ʼ��ϵͳʱ��
*            	System Clock source            = PLL (HSE)
*            	SYSCLK(Hz)                     = 400000000 (CPU Clock)
*           	HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
*            	AHB Prescaler                  = 2
*            	D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
*            	D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
*            	D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
*            	D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
*            	HSE Frequency(Hz)              = 25000000
*           	PLL_M                          = 5
*            	PLL_N                          = 160
*            	PLL_P                          = 2
*            	PLL_Q                          = 4
*            	PLL_R                          = 2
*            	VDD(V)                         = 3.3
*            	Flash Latency(WS)              = 4
*	��    ��: ��
*	�� �� ֵ: 1 ��ʾʧ�ܣ�0 ��ʾ�ɹ�
*********************************************************************************************************
*/
int SystemClock_Config(void)
{
	/* ʹ���ⲿʱ�� #################################################################################*/

    
    return 0;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
