#ifndef __SWM221_H__
#define __SWM221_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers **********************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                        */
  HardFault_IRQn	          = -13,	/*!< 3 Cortex-M0 Hard Fault Interrupt				 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                  */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                  */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M0 System Tick Interrupt              */
  
/******  Cortex-M0 specific Interrupt Numbers ************************************************/
  UART0_IRQn                = 0,
  TIMR0_IRQn                = 1,
  CAN0_IRQn                 = 2,
  UART1_IRQn                = 3,
  PWM1_IRQn                 = 4,
  TIMR1_IRQn                = 5,
  HALL_IRQn                 = 6,
  PWM0_IRQn                 = 7,
  QSPI0_IRQn                = 8,
  PWMBRK_IRQn               = 9,
  USART0_IRQn               = 10,
  WDT_IRQn                  = 11,
  I2C0_IRQn                 = 12,
  XTALSTOP_IRQn             = 13,
  ADC_IRQn                  = 14,
  ACMP_IRQn                 = 15,
  BTIMR0_IRQn               = 16,
  BTIMR1_IRQn               = 17,
  BTIMR2_IRQn               = 18,
  BTIMR3_IRQn               = 19,
  GPIOA_IRQn                = 20,
  GPIOB_IRQn                = 21,
  GPIOC_IRQn                = 22,
  GPIOA0_GPIOC0_IRQn        = 23,
  GPIOA1_GPIOC1_IRQn		= 24,
  GPIOA2_GPIOC2_MPU_IRQn    = 25,
  GPIOA3_GPIOC3_PVD_IRQn    = 26,
  GPIOB0_GPIOA8_TIMR2_IRQn  = 27,
  GPIOB1_GPIOA9_DMA_IRQn    = 28,
  GPIOB2_GPIOA10_DIV_IRQn   = 29,
  GPIOB3_GPIOA11_SPI0_IRQn  = 30,
  GPIOB4_GPIOB10_QEI_IRQn   = 31,
} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT		    0	   /*!< UART does not provide a MPU present or not	     */
#define __NVIC_PRIO_BITS		2	   /*!< UART Supports 2 Bits for the Priority Levels	 */
#define __Vendor_SysTickConfig  0	   /*!< Set to 1 if different SysTick Config is used	 */

#if   defined ( __CC_ARM )
  #pragma anon_unions
#endif

#include <stdio.h>
#include <stdbool.h>
#include "core_cm0.h"				   /* Cortex-M0 processor and core peripherals		     */
#include "system_SWM221.h"


/******************************************************************************/
/*				Device Specific Peripheral registers structures			 */
/******************************************************************************/
typedef struct {
	__IO uint32_t CLKSEL;				    //Clock Select

	__IO uint32_t CLKDIVx_ON;				//[0] CLK_DIVxʱ��Դ����

	__IO uint32_t CLKEN0;					//Clock Enable
	
		 uint32_t RESERVED;

	__IO uint32_t SLEEP;
	
		 uint32_t RESERVED2[4];
	
	__IO uint32_t RSTSR;					//Reset Status
		
		 uint32_t RESERVED3[22];
	
	__I  uint32_t CHIPID[4];
	
	__IO uint32_t BACKUP[4];				//Data Backup Register
	
		 uint32_t RESERVED4[24];
		 
	__IO uint32_t PAWKEN;				    //PORTA Wakeup Enable
	__IO uint32_t PBWKEN;	
	__IO uint32_t PCWKEN;
	
		 uint32_t RESERVED5[9];

	__IO uint32_t PAWKSR;				    //PORTA Wakeup Status��д1����
	__IO uint32_t PBWKSR;	
	__IO uint32_t PCWKSR;
	
		 uint32_t RESERVED6[(0x400-0x138)/4-1];
	
	__IO uint32_t IOFILT0;					//IO Filter 0
	__IO uint32_t IOFILT1;
	
		 uint32_t RESERVED7[(0x720-0x404)/4-1];
	
	__IO uint32_t PRSTEN;					//���踴λʹ�ܣ�ֻ�е�PRSTEN��ֵΪ0x55ʱ������дPRSTR0��PRSTR1
	__IO uint32_t PRSTR0;

    //Analog Control: 0x40045800
         uint32_t RESERVED8[(0x40045800-0x40000724)/4-1];
	
	__IO uint32_t PMUCR;
	
	__IO uint32_t VRFCR;					//Vref Control Register
	
	__IO uint32_t RCCR;						//RC Control Register
	
		 uint32_t RESERVED9;
    
	__IO uint32_t XTALCR;
	__IO uint32_t XTALSR;
		
	__IO uint32_t PLLCR;
    __IO uint32_t PLLSR;
	
	__IO uint32_t PVDCR;
	__IO uint32_t PVDSR;
	
	__IO uint32_t LVRCR;
	
    __IO uint32_t ACMP0CR;					//Analog Comparator 0 Control Register
	__IO uint32_t ACMP1CR;
	__IO uint32_t ACMPCR;
	__IO uint32_t ACMPSR;					//Analog Comparator Status Register
	
	__IO uint32_t PGA0CR;					//PGA0 Control Register
	__IO uint32_t PGA1CR;
	__IO uint32_t PGA2CR;
	__IO uint32_t PGAREF;					//PGA Vref Control Register
	
	__IO uint32_t TEMPCR;					//Temperature Sensor Control Register
	
	__IO uint32_t ADCREF;					//ADC Vref select
} SYS_TypeDef;


#define SYS_CLKSEL_SYS_Pos			0		//ϵͳʱ��ѡ��	1 HRC	0 CLK_DIVx
#define SYS_CLKSEL_SYS_Msk			(0x01 << SYS_CLKSEL_SYS_Pos)
#define SYS_CLKSEL_CLK_DIVx_Pos		1		//ѡ��CLK_DIVx  0 CLK_DIV1   1 CLK_DIV2   2 CLK_DIV4   3 CLK_DIV8
#define SYS_CLKSEL_CLK_DIVx_Msk		(0x03 << SYS_CLKSEL_CLK_DIVx_Pos)
#define SYS_CLKSEL_CLK_Pos			3		//Clock Source  0 LRC   1 PLL   2 XTAL   3 HRC
#define SYS_CLKSEL_CLK_Msk			(0x03 << SYS_CLKSEL_CLK_Pos)
#define SYS_CLKSEL_IOFILT_Pos		5		//IO Filterʱ��ѡ��0 HRC   2 XTAL   3 LRC
#define SYS_CLKSEL_IOFILT_Msk		(0x03 << SYS_CLKSEL_IOFILT_Pos)
#define SYS_CLKSEL_WDT_Pos			7		//���Ź�ʱ��ѡ��  0 HRC   1 XTAL   2 LRC
#define SYS_CLKSEL_WDT_Msk			(0x03 << SYS_CLKSEL_WDT_Pos)

#define SYS_CLKDIV_ON_Pos           0
#define SYS_CLKDIV_ON_Msk           (0x01 << SYS_CLKDIV_ON_Pos)

#define SYS_CLKEN0_GPIOA_Pos		0
#define SYS_CLKEN0_GPIOA_Msk		(0x01 << SYS_CLKEN0_GPIOA_Pos)
#define SYS_CLKEN0_GPIOB_Pos		1
#define SYS_CLKEN0_GPIOB_Msk		(0x01 << SYS_CLKEN0_GPIOB_Pos)
#define SYS_CLKEN0_GPIOC_Pos		2
#define SYS_CLKEN0_GPIOC_Msk		(0x01 << SYS_CLKEN0_GPIOC_Pos)
#define SYS_CLKEN0_UART0_Pos		3
#define SYS_CLKEN0_UART0_Msk		(0x01 << SYS_CLKEN0_UART0_Pos)
#define SYS_CLKEN0_UART1_Pos		4
#define SYS_CLKEN0_UART1_Msk		(0x01 << SYS_CLKEN0_UART1_Pos)
#define SYS_CLKEN0_USART0_Pos		5
#define SYS_CLKEN0_USART0_Msk		(0x01 << SYS_CLKEN0_USART0_Pos)
#define SYS_CLKEN0_SPI0_Pos			6
#define SYS_CLKEN0_SPI0_Msk			(0x01 << SYS_CLKEN0_SPI0_Pos)
#define SYS_CLKEN0_I2C0_Pos			7
#define SYS_CLKEN0_I2C0_Msk			(0x01 << SYS_CLKEN0_I2C0_Pos)
#define SYS_CLKEN0_QSPI0_Pos		8
#define SYS_CLKEN0_QSPI0_Msk		(0x01 << SYS_CLKEN0_QSPI0_Pos)
#define SYS_CLKEN0_TIMR_Pos			9
#define SYS_CLKEN0_TIMR_Msk			(0x01 << SYS_CLKEN0_TIMR_Pos)
#define SYS_CLKEN0_BTIMR_Pos		10
#define SYS_CLKEN0_BTIMR_Msk		(0x01 << SYS_CLKEN0_BTIMR_Pos)
#define SYS_CLKEN0_PWM_Pos			11
#define SYS_CLKEN0_PWM_Msk			(0x01 << SYS_CLKEN0_PWM_Pos)
#define SYS_CLKEN0_CRC_Pos			12
#define SYS_CLKEN0_CRC_Msk			(0x01 << SYS_CLKEN0_CRC_Pos)
#define SYS_CLKEN0_DIV_Pos			13
#define SYS_CLKEN0_DIV_Msk			(0x01 << SYS_CLKEN0_DIV_Pos)
#define SYS_CLKEN0_ANAC_Pos			14		//ģ����Ƶ�Ԫʱ��ʹ��
#define SYS_CLKEN0_ANAC_Msk			(0x01 << SYS_CLKEN0_ANAC_Pos)
#define SYS_CLKEN0_ADC0_Pos			15
#define SYS_CLKEN0_ADC0_Msk			(0x01 << SYS_CLKEN0_ADC0_Pos)
#define SYS_CLKEN0_CAN0_Pos			16
#define SYS_CLKEN0_CAN0_Msk			(0x01 << SYS_CLKEN0_CAN0_Pos)
#define SYS_CLKEN0_IOFILT_Pos		17
#define SYS_CLKEN0_IOFILT_Msk		(0x01 << SYS_CLKEN0_IOFILT_Pos)
#define SYS_CLKEN0_WDT_Pos			18
#define SYS_CLKEN0_WDT_Msk			(0x01 << SYS_CLKEN0_WDT_Pos)
#define SYS_CLKEN0_MPU_Pos			19
#define SYS_CLKEN0_MPU_Msk			(0x01 << SYS_CLKEN0_MPU_Pos)
#define SYS_CLKEN0_QEI_Pos			20
#define SYS_CLKEN0_QEI_Msk			(0x01 << SYS_CLKEN0_QEI_Pos)

#define SYS_SLEEP_SLEEP_Pos			0		//����λ��1��ϵͳ������SLEEPģʽ
#define SYS_SLEEP_SLEEP_Msk			(0x01 << SYS_SLEEP_SLEEP_Pos)

#define SYS_RSTSR_POR_Pos			0		//1 ���ֹ�POR��λ��д1����
#define SYS_RSTSR_POR_Msk			(0x01 << SYS_RSTSR_POR_Pos)
#define SYS_RSTSR_WDT_Pos			1		//1 ���ֹ�WDT��λ��д1����
#define SYS_RSTSR_WDT_Msk			(0x01 << SYS_RSTSR_WDT_Pos)

#define SYS_IOFILT_TIM_Pos			0		//�˲�����ʱ�� = Tfilter_clk * ʱ�ӷ�Ƶ * 2^TIM
#define SYS_IOFILT_TIM_Msk			(0x0F << SYS_IOFILT_TIM_Pos)
#define SYS_IOFILT_CLKDIV_Pos		4		//0 ʱ�Ӳ���Ƶ   1 ʱ��32��Ƶ
#define SYS_IOFILT_CLKDIV_Msk		(0x01 << SYS_IOFILT_CLKDIV_Pos)
#define SYS_IOFILT_IO0EN_Pos		5		//IO0 �˲�ʹ��
#define SYS_IOFILT_IO0EN_Msk		(0x01 << SYS_IOFILT_IO0EN_Pos)
#define SYS_IOFILT_IO1EN_Pos		6
#define SYS_IOFILT_IO1EN_Msk		(0x01 << SYS_IOFILT_IO1EN_Pos)
#define SYS_IOFILT_IO2EN_Pos		7
#define SYS_IOFILT_IO2EN_Msk		(0x01 << SYS_IOFILT_IO2EN_Pos)
#define SYS_IOFILT_IO3EN_Pos		8
#define SYS_IOFILT_IO3EN_Msk		(0x01 << SYS_IOFILT_IO3EN_Pos)

#define SYS_PRSTR0_GPIOA_Pos		0		//1 ��λGPIOA    0 ����λ
#define SYS_PRSTR0_GPIOA_Msk		(0x01 <<SYS_PRSTR0_GPIOA_Pos)
#define SYS_PRSTR0_GPIOB_Pos		1
#define SYS_PRSTR0_GPIOB_Msk		(0x01 <<SYS_PRSTR0_GPIOB_Pos)
#define SYS_PRSTR0_GPIOC_Pos		2
#define SYS_PRSTR0_GPIOC_Msk		(0x01 <<SYS_PRSTR0_GPIOC_Pos)
#define SYS_PRSTR0_UART0_Pos		3
#define SYS_PRSTR0_UART0_Msk		(0x01 <<SYS_PRSTR0_UART0_Pos)
#define SYS_PRSTR0_UART1_Pos		4
#define SYS_PRSTR0_UART1_Msk		(0x01 <<SYS_PRSTR0_UART1_Pos)
#define SYS_PRSTR0_USART0_Pos		5
#define SYS_PRSTR0_USART0_Msk		(0x01 <<SYS_PRSTR0_USART0_Pos)
#define SYS_PRSTR0_SPI0_Pos			6
#define SYS_PRSTR0_SPI0_Msk			(0x01 <<SYS_PRSTR0_SPI0_Pos)
#define SYS_PRSTR0_I2C0_Pos			7
#define SYS_PRSTR0_I2C0_Msk			(0x01 <<SYS_PRSTR0_I2C0_Pos)
#define SYS_PRSTR0_QSPI0_Pos		8
#define SYS_PRSTR0_QSPI0_Msk		(0x01 <<SYS_PRSTR0_QSPI0_Pos)
#define SYS_PRSTR0_TIMR_Pos			9
#define SYS_PRSTR0_TIMR_Msk			(0x01 <<SYS_PRSTR0_TIMR_Pos)
#define SYS_PRSTR0_BTIMR_Pos		10
#define SYS_PRSTR0_BTIMR_Msk		(0x01 <<SYS_PRSTR0_BTIMR_Pos)
#define SYS_PRSTR0_PWM_Pos			11
#define SYS_PRSTR0_PWM_Msk			(0x01 <<SYS_PRSTR0_PWM_Pos)
#define SYS_PRSTR0_CRC_Pos			12
#define SYS_PRSTR0_CRC_Msk			(0x01 <<SYS_PRSTR0_CRC_Pos)
#define SYS_PRSTR0_DIV_Pos			13
#define SYS_PRSTR0_DIV_Msk			(0x01 <<SYS_PRSTR0_DIV_Pos)
#define SYS_PRSTR0_ANAC_Pos			14
#define SYS_PRSTR0_ANAC_Msk			(0x01 <<SYS_PRSTR0_ANAC_Pos)
#define SYS_PRSTR0_ADC0_Pos			15
#define SYS_PRSTR0_ADC0_Msk			(0x01 <<SYS_PRSTR0_ADC0_Pos)
#define SYS_PRSTR0_CAN0_Pos			16
#define SYS_PRSTR0_CAN0_Msk			(0x01 <<SYS_PRSTR0_CAN0_Pos)
#define SYS_PRSTR0_IOFILT_Pos		17
#define SYS_PRSTR0_IOFILT_Msk		(0x01 <<SYS_PRSTR0_IOFILT_Pos)
#define SYS_PRSTR0_WDT_Pos			18
#define SYS_PRSTR0_WDT_Msk			(0x01 <<SYS_PRSTR0_WDT_Pos)
#define SYS_PRSTR0_MPU_Pos			19
#define SYS_PRSTR0_MPU_Msk			(0x01 <<SYS_PRSTR0_MPU_Pos)
#define SYS_PRSTR0_QEI_Pos			20
#define SYS_PRSTR0_QEI_Msk			(0x01 << SYS_PRSTR0_QEI_Pos)

#define SYS_VRFCR_EN_Pos			0
#define SYS_VRFCR_EN_Msk			(0x01 << SYS_VRFCR_EN_Pos)
#define SYS_VRFCR_LVL_Pos			1		//0 2.4V   1 3.6V   2 4.5V
#define SYS_VRFCR_LVL_Msk			(0x03 << SYS_VRFCR_LVL_Pos)

#define SYS_RCCR_HON_Pos			0		//High speed RC ON
#define SYS_RCCR_HON_Msk			(0x01 << SYS_RCCR_HON_Pos)
#define SYS_RCCR_LON_Pos			1		//Low speed RC ON
#define SYS_RCCR_LON_Msk			(0x03 << SYS_RCCR_LON_Pos)

#define SYS_XTALCR_ON_Pos			0		//XTAL On
#define SYS_XTALCR_ON_Msk			(0x01 << SYS_XTALCR_ON_Pos)
#define SYS_XTALCR_BYPASS_Pos		1		//��Ƶ����ʱ��ֱ��
#define SYS_XTALCR_BYPASS_Msk		(0x01 << SYS_XTALCR_BYPASS_Pos)
#define SYS_XTALCR_DET_Pos			2		//XTAL Stop Detect Enable
#define SYS_XTALCR_DET_Msk			(0x01 << SYS_XTALCR_DET_Pos)

#define SYS_XTALSR_STOP_Pos			0		//XTAL Stop��д1����
#define SYS_XTALSR_STOP_Msk			(0x01 << SYS_XTALSR_STOP_Pos)

#define SYS_PLLCR_PWRDN_Pos			0		//PLL Power Down
#define SYS_PLLCR_PWRDN_Msk			(0x01 << SYS_PLLCR_PWRDN_Pos)
#define SYS_PLLCR_OUTEN_Pos			1		//PLL Clock Out Enable
#define SYS_PLLCR_OUTEN_Msk			(0x01 << SYS_PLLCR_OUTEN_Pos)
#define SYS_PLLCR_INSEL_Pos			2		//1 XTAL	0 HRC
#define SYS_PLLCR_INSEL_Msk			(0x01 << SYS_PLLCR_INSEL_Pos)
#define SYS_PLLCR_BYPASS_Pos		3 		//1 fPLL = fIN / INDIV
#define SYS_PLLCR_BYPASS_Msk		(0x01 << SYS_PLLCR_BYPASS_Pos)
#define SYS_PLLCR_INDIV_Pos			8		//PLL ����Դʱ�ӷ�Ƶ
#define SYS_PLLCR_INDIV_Msk			(0x3F << SYS_PLLCR_INDIV_Pos)
#define SYS_PLLCR_FBDIV_Pos			14		//PLL FeedBack��Ƶ�Ĵ�����fPLL = fIN / INDIV * FBDIV
#define SYS_PLLCR_FBDIV_Msk			(0x7F << SYS_PLLCR_FBDIV_Pos)

#define SYS_PLLSR_LOCK_Pos			0		//PLL Lock indicate
#define SYS_PLLSR_LOCK_Msk			(0x01 << SYS_PLLSR_LOCK_Pos)
#define SYS_PLLSR_ENA_Pos			1
#define SYS_PLLSR_ENA_Msk			(0x01 << SYS_PLLSR_ENA_Pos)

#define SYS_PVDCR_EN_Pos		    0		//PVD Enable
#define SYS_PVDCR_EN_Msk		    (0x01 << SYS_PVDCR_EN_Pos)
#define SYS_PVDCR_LVL_Pos			1		//PVD������ƽ��0 2.0v   1 2.3v   2 2.7v   3 3.0v   4 3.7v   5 4.0v   6 4.3v
#define SYS_PVDCR_LVL_Msk			(0x07 << SYS_PVDCR_LVL_Pos)
#define SYS_PVDCR_IE_Pos			4		//PVD Interrupt Enable
#define SYS_PVDCR_IE_Msk			(0x01 << SYS_PVDCR_IE_Pos)

#define SYS_PVDSR_ST_Pos			0		//PVD Status
#define SYS_PVDSR_ST_Msk			(0x01 << SYS_PVDSR_ST_Pos)
#define SYS_PVDSR_IF_Pos			1		//�жϱ�־��д1����
#define SYS_PVDSR_IF_Msk			(0x01 << SYS_PVDSR_IF_Pos)

#define SYS_LVRCR_EN_Pos			0		//LVR Enable
#define SYS_LVRCR_EN_Msk			(0x01 << SYS_LVRCR_EN_Pos)
#define SYS_LVRCR_LVL_Pos			1		//LVR������ƽ��0 1.8v   1 2.0v   2 2.5v   3 3.5v
#define SYS_LVRCR_LVL_Msk			(0x03 << SYS_LVRCR_LVL_Pos)
#define SYS_LVRCR_WEN_Pos			3		//LVRCR дʹ�ܣ�д LVRCR ʱ��λ������ 1
#define SYS_LVRCR_WEN_Msk			(0x01 << SYS_LVRCR_WEN_Pos)

#define SYS_ACMP0CR_EN_Pos			0
#define SYS_ACMP0CR_EN_Msk			(0x01 << SYS_ACMP0CR_EN_Pos)
#define SYS_ACMP0CR_HYS_Pos			1		//���͵�ѹ��0 1mV   1 10mV   2 20mV   3 50mV
#define SYS_ACMP0CR_HYS_Msk			(0x03 << SYS_ACMP0CR_HYS_Pos)
#define SYS_ACMP0CR_VNSEL_Pos		3		//����ѡ��0 VN   1 DAC_OUT   2 VPX
#define SYS_ACMP0CR_VNSEL_Msk		(0x03 << SYS_ACMP0CR_VNSEL_Pos)
#define SYS_ACMP0CR_VPSEL_Pos		5		//����ѡ��0 VP0   1 VP1   2 VP2   3 PGA0_VP   4 PGA2_VP
#define SYS_ACMP0CR_VPSEL_Msk		(0x07 << SYS_ACMP0CR_VPSEL_Pos)
#define SYS_ACMP0CR_VPXEN_Pos		8		//1 VP0/VP1/VP2��������,���ĵ���ΪVPX
#define SYS_ACMP0CR_VPXEN_Msk		(0x01 << SYS_ACMP0CR_VPXEN_Pos)
#define SYS_ACMP0CR_IE_Pos			16		//�ж�ʹ��
#define SYS_ACMP0CR_IE_Msk			(0x01 << SYS_ACMP0CR_IE_Pos)
#define SYS_ACMP0CR_TOPWM_Pos		17		//�Ƚ��������Ϊ PWM ɲ���ź�
#define SYS_ACMP0CR_TOPWM_Msk		(0x01 << SYS_ACMP0CR_TOPWM_Pos)

#define SYS_ACMP1CR_EN_Pos			0
#define SYS_ACMP1CR_EN_Msk			(0x01 << SYS_ACMP1CR_EN_Pos)
#define SYS_ACMP1CR_HYS_Pos			1
#define SYS_ACMP1CR_HYS_Msk			(0x03 << SYS_ACMP1CR_HYS_Pos)
#define SYS_ACMP1CR_VNSEL_Pos		3		//����ѡ��0 VN   1 DAC_OUT
#define SYS_ACMP1CR_VNSEL_Msk		(0x01 << SYS_ACMP1CR_VNSEL_Pos)
#define SYS_ACMP1CR_VPSEL_Pos		5		//����ѡ��0 VP   1 PGA1_VP   2 PGA0_OUT   3 PGA1_OUT   4 PGA2_OUT
#define SYS_ACMP1CR_VPSEL_Msk		(0x07 << SYS_ACMP1CR_VPSEL_Pos)
#define SYS_ACMP1CR_IE_Pos			16
#define SYS_ACMP1CR_IE_Msk			(0x01 << SYS_ACMP1CR_IE_Pos)
#define SYS_ACMP1CR_TOPWM_Pos		17
#define SYS_ACMP1CR_TOPWM_Msk		(0x01 << SYS_ACMP1CR_TOPWM_Pos)

#define SYS_ACMPCR_DACEN_Pos		0 		//ACMP DACʹ��
#define SYS_ACMPCR_DACEN_Msk		(0x01 << SYS_ACMPCR_DACEN_Pos)
#define SYS_ACMPCR_DACVR_Pos		1 		//ACMP DAC�ο���ѹѡ��
#define SYS_ACMPCR_DACVR_Msk		(0x03 << SYS_ACMPCR_DACVR_Pos)
#define SYS_ACMPCR_DACDR_Pos		8 		//ACMP DAC���ݼĴ���
#define SYS_ACMPCR_DACDR_Msk		(0xFF << SYS_ACMPCR_DACDR_Pos)

#define SYS_ACMPSR_CMP0IF_Pos		0		//�жϱ�־��д1����
#define SYS_ACMPSR_CMP0IF_Msk		(0x01 << SYS_ACMPSR_CMP0IF_Pos)
#define SYS_ACMPSR_CMP1IF_Pos		1
#define SYS_ACMPSR_CMP1IF_Msk		(0x01 << SYS_ACMPSR_CMP1IF_Pos)
#define SYS_ACMPSR_CMP0OUT_Pos		8		//0 N > P   1 P > N
#define SYS_ACMPSR_CMP0OUT_Msk		(0x01 << SYS_ACMPSR_CMP0OUT_Pos)
#define SYS_ACMPSR_CMP1OUT_Pos		9
#define SYS_ACMPSR_CMP1OUT_Msk		(0x01 << SYS_ACMPSR_CMP1OUT_Pos)

#define SYS_PGA0CR_EN_Pos			0
#define SYS_PGA0CR_EN_Msk			(0x01 << SYS_PGA0CR_EN_Pos)
#define SYS_PGA0CR_MODE_Pos			1 	//0 OPA   1 PGA
#define SYS_PGA0CR_MODE_Msk			(0x01 << SYS_PGA0CR_MODE_Pos)
#define SYS_PGA0CR_ROUT_Pos			2 	//�������ѡ��0 open   1 100   2 1k   3 10k
#define SYS_PGA0CR_ROUT_Msk			(0x03 << SYS_PGA0CR_ROUT_Pos)
#define SYS_PGA0CR_GAIN_Pos			4 	//PGA ����ѡ��0 x1   1 x5   2 x10   3 x10
#define SYS_PGA0CR_GAIN_Msk			(0x03 << SYS_PGA0CR_GAIN_Pos)
#define SYS_PGA0CR_BUFEN_Pos		6 	//��� BUF ʹ��
#define SYS_PGA0CR_BUFEN_Msk		(0x01 << SYS_PGA0CR_BUFEN_Pos)
#define SYS_PGA0CR_BYPASS_Pos		7 	//��� BUF ��·
#define SYS_PGA0CR_BYPASS_Msk		(0x01 << SYS_PGA0CR_BYPASS_Pos)

#define SYS_PGA1CR_EN_Pos			0
#define SYS_PGA1CR_EN_Msk			(0x01 << SYS_PGA1CR_EN_Pos)
#define SYS_PGA1CR_MODE_Pos			1
#define SYS_PGA1CR_MODE_Msk			(0x01 << SYS_PGA1CR_MODE_Pos)
#define SYS_PGA1CR_ROUT_Pos			2
#define SYS_PGA1CR_ROUT_Msk			(0x03 << SYS_PGA1CR_ROUT_Pos)
#define SYS_PGA1CR_GAIN_Pos			4
#define SYS_PGA1CR_GAIN_Msk			(0x03 << SYS_PGA1CR_GAIN_Pos)
#define SYS_PGA1CR_BUFEN_Pos		6
#define SYS_PGA1CR_BUFEN_Msk		(0x01 << SYS_PGA1CR_BUFEN_Pos)
#define SYS_PGA1CR_BYPASS_Pos		7
#define SYS_PGA1CR_BYPASS_Msk		(0x01 << SYS_PGA1CR_BYPASS_Pos)

#define SYS_PGA2CR_EN_Pos			0
#define SYS_PGA2CR_EN_Msk			(0x01 << SYS_PGA2CR_EN_Pos)
#define SYS_PGA2CR_MODE_Pos			1
#define SYS_PGA2CR_MODE_Msk			(0x01 << SYS_PGA2CR_MODE_Pos)
#define SYS_PGA2CR_ROUT_Pos			2
#define SYS_PGA2CR_ROUT_Msk			(0x03 << SYS_PGA2CR_ROUT_Pos)
#define SYS_PGA2CR_GAIN_Pos			4
#define SYS_PGA2CR_GAIN_Msk			(0x03 << SYS_PGA2CR_GAIN_Pos)
#define SYS_PGA2CR_BUFEN_Pos		6
#define SYS_PGA2CR_BUFEN_Msk		(0x01 << SYS_PGA2CR_BUFEN_Pos)
#define SYS_PGA2CR_BYPASS_Pos		7
#define SYS_PGA2CR_BYPASS_Msk		(0x01 << SYS_PGA2CR_BYPASS_Pos)

#define SYS_PGAREF_REFSEL_Pos		0 		//PGA �ο���ѹѡ��0 1.2v   1 1.5v   2 2.25v   3 ADCVREF/2
#define SYS_PGAREF_REFSEL_Msk		(0x03 << SYS_PGAREF_REFSEL_Pos)

#define SYS_TEMPCR_EN_Pos			0
#define SYS_TEMPCR_EN_Msk			(0x03 << SYS_TEMPCR_EN_Pos)

#define SYS_ADCREF_REFSEL_Pos		0 		//ADC �ο���ѹѡ��0 VDD   1 VREF
#define SYS_ADCREF_REFSEL_Msk		(0x01 << SYS_ADCREF_REFSEL_Pos)




typedef struct {
	__IO uint32_t FUNC0;					//���Ź���ѡ��
	
	__IO uint32_t FUNC1;
	
		 uint32_t RESERVED[62];
	
    __IO uint32_t PULLU;              		//����ʹ��
    
         uint32_t RESERVED2[63];
    
    __IO uint32_t PULLD;	              	//����ʹ��
    
         uint32_t RESERVED3[63];
    
    __IO uint32_t INEN;               		//����ʹ��
    
         uint32_t RESERVED4[63];

	__IO uint32_t OPEND;              		//��©ʹ��
} PORT_TypeDef;




typedef struct {
	__IO uint32_t ODR;
#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7
#define PIN8    8
#define PIN9    9
#define PIN10   10
#define PIN11   11
#define PIN12   12
#define PIN13   13
#define PIN14   14
#define PIN15   15

	__IO uint32_t DIR;					    //0 ����	1 ���

	__IO uint32_t INTLVLTRG;				//Interrupt Level Trigger  1 ��ƽ�����ж�	0 ���ش����ж�

	__IO uint32_t INTBE;					//Both Edge����INTLVLTRG��Ϊ���ش����ж�ʱ����λ��1��ʾ�����غ��½��ض������жϣ���0ʱ����������INTRISEENѡ��

	__IO uint32_t INTRISEEN;				//Interrupt Rise Edge Enable   1 ������/�ߵ�ƽ�����ж�	0 �½���/�͵�ƽ�����ж�

	__IO uint32_t INTEN;					//1 �ж�ʹ��	0 �жϽ�ֹ

	__I  uint32_t INTRAWSTAT;			    //�жϼ�ⵥԪ�Ƿ��⵽�˴����жϵ����� 1 ��⵽���жϴ�������	0 û�м�⵽�жϴ�������

	__I  uint32_t INTSTAT;				    //INTSTAT.PIN0 = INTRAWSTAT.PIN0 & INTEN.PIN0

	__O  uint32_t INTCLR;				    //д1����жϱ�־��ֻ�Ա��ش����ж�����
	
		 uint32_t RESERVED[3];
	
	__I  uint32_t IDR;
	
		 uint32_t RESERVED2[3];
	
	__IO uint32_t DATAPIN0;					//PIN0���ŵ�DATA�Ĵ������������Ŷ�Ӧ����32λ�Ĵ���������ʵ��ԭ��д����
	__IO uint32_t DATAPIN1;
	__IO uint32_t DATAPIN2;
	__IO uint32_t DATAPIN3;
	__IO uint32_t DATAPIN4;
	__IO uint32_t DATAPIN5;
	__IO uint32_t DATAPIN6;
	__IO uint32_t DATAPIN7;
	__IO uint32_t DATAPIN8;
	__IO uint32_t DATAPIN9;
	__IO uint32_t DATAPIN10;
	__IO uint32_t DATAPIN11;
	__IO uint32_t DATAPIN12;
	__IO uint32_t DATAPIN13;
	__IO uint32_t DATAPIN14;
	__IO uint32_t DATAPIN15;
} GPIO_TypeDef;




typedef struct {
	__IO uint32_t LOAD;						//��ʱ������ֵ��ʹ�ܺ�ʱ���Ӵ���ֵ��ʼ���µݼ�����

	__I  uint32_t VALUE;					//��ʱ����ǰֵ��LDVAL-CVAL �ɼ������ʱʱ��

	__IO uint32_t CR;
	
		 uint32_t RESERVED;
	
	__IO uint32_t IE;
		
	__IO uint32_t IF;
	
	__IO uint32_t HALT;						//[0] 1 ��ͣ����    0 �ָ�����
	
	__IO uint32_t OCCR;
	
	__IO uint32_t OCMAT;
	__IO uint32_t RESERVED2;
	
	__IO uint32_t ICLOW;
	__IO uint32_t ICHIGH;
	
	__IO uint32_t PREDIV;					//Ԥ��Ƶ��8λ
} TIMR_TypeDef;


#define TIMR_LOAD_VALUE_Pos			0
#define TIMR_LOAD_VALUE_Msk			(0xFFFFFF << TIMR_LOAD_VALUE_Pos)
#define TIMR_LOAD_RELOAD_Pos		24		//1 ��������������д���LOADֵ��ʼ������ֻ��BTIMR�д˹���
#define TIMR_LOAD_RELOAD_Msk		(0x01 << TIMR_LOAD_RELOAD_Pos)

#define TIMR_CR_CLKSRC_Pos			0		//ʱ��Դ��  0 �ڲ�ϵͳʱ��	2 �ⲿ�����������
#define TIMR_CR_CLKSRC_Msk			(0x03 << TIMR_CR_CLKSRC_Pos)
#define TIMR_CR_MODE_Pos			2		//����ģʽ��0 ��ʱ��    1 ���벶��    2 ����Ƚ�
#define TIMR_CR_MODE_Msk			(0x03 << TIMR_CR_MODE_Pos)
#define TIMR_CR_ICEDGE_Pos			4		//���벶��ģʽ�¼����������أ�0 ˫����   1 ������   2 �½���
#define TIMR_CR_ICEDGE_Msk			(0x03 << TIMR_CR_ICEDGE_Pos)

#define TIMR_IE_TO_Pos				0		//Time out
#define TIMR_IE_TO_Msk				(0x01 << TIMR_IE_TO_Pos)
#define TIMR_IE_OC0_Pos				1		//����Ƚϣ���һ����ת��
#define TIMR_IE_OC0_Msk				(0x01 << TIMR_IE_OC0_Pos)
#define TIMR_IE_OC1_Pos				2		//����Ƚϣ��ڶ�����ת��
#define TIMR_IE_OC1_Msk				(0x01 << TIMR_IE_OC1_Pos)
#define TIMR_IE_ICR_Pos				3		//���벶���������ж�
#define TIMR_IE_ICR_Msk				(0x01 << TIMR_IE_ICR_Pos)
#define TIMR_IE_ICF_Pos				4		//���벶���½����ж�
#define TIMR_IE_ICF_Msk				(0x01 << TIMR_IE_ICF_Pos)

#define TIMR_IF_TO_Pos				0		//��ʱ�жϱ�־��д1����
#define TIMR_IF_TO_Msk				(0x01 << TIMR_IF_TO_Pos)
#define TIMR_IF_OC0_Pos				1
#define TIMR_IF_OC0_Msk				(0x01 << TIMR_IF_OC0_Pos)
#define TIMR_IF_OC1_Pos				2
#define TIMR_IF_OC1_Msk				(0x01 << TIMR_IF_OC1_Pos)
#define TIMR_IF_ICR_Pos				3
#define TIMR_IF_ICR_Msk				(0x01 << TIMR_IF_ICR_Pos)
#define TIMR_IF_ICF_Pos				4
#define TIMR_IF_ICF_Msk				(0x01 << TIMR_IF_ICF_Pos)

#define TIMR_OCCR_FORCELVL_Pos		0		//Force Levle��ǿ�������ƽ
#define TIMR_OCCR_FORCELVL_Msk		(0x01 << TIMR_OCCR_FORCELVL_Pos)
#define TIMR_OCCR_INITLVL_Pos		1		//Initial Level, ��ʼ�����ƽ��Timerֹͣʱ����ģʽ���ǡ�����Ƚϡ�ʱ�������ƽ
#define TIMR_OCCR_INITLVL_Msk		(0x01 << TIMR_OCCR_INITLVL_Pos)
#define TIMR_OCCR_FORCEEN_Pos		2		//Force Enable, ǿ�����ʹ��
#define TIMR_OCCR_FORCEEN_Msk		(0x01 << TIMR_OCCR_FORCEEN_Pos)


typedef struct {
	__IO uint32_t HALLIE;					//[0] HALL�ж�ʹ��
	
		 uint32_t RESERVED;
	
	__IO uint32_t HALLIF;
	
	__IO uint32_t HALLEN;					//[0] HALL���ܿ���
	
	__IO uint32_t HALLDR;					//HALL���������ؽ�������������ֵ - ��ǰֵ������˼Ĵ���
	
		 uint32_t RESERVED2[2];
	
	__IO uint32_t HALLSR;
	
	__IO uint32_t ICSR;						//Input Capture Pin Status
		
		 uint32_t RESERVED3[7];
	
	__IO uint32_t EN;
} TIMRG_TypeDef;


#define TIMRG_HALLIF_IN0_Pos		0		//HALL�����ź�0�����жϱ�־��д1����
#define TIMRG_HALLIF_IN0_Msk		(0x01 << TIMRG_HALLIF_IN0_Pos)
#define TIMRG_HALLIF_IN1_Pos		1
#define TIMRG_HALLIF_IN1_Msk		(0x01 << TIMRG_HALLIF_IN1_Pos)
#define TIMRG_HALLIF_IN2_Pos		2
#define TIMRG_HALLIF_IN2_Msk		(0x01 << TIMRG_HALLIF_IN2_Pos)

#define TIMRG_HALLSR_IN0_Pos		0		//HALL�����źŵ�ǰ��ƽ
#define TIMRG_HALLSR_IN0_Msk		(0x01 << TIMRG_HALLSR_IN0_Pos)
#define TIMRG_HALLSR_IN1_Pos		1
#define TIMRG_HALLSR_IN1_Msk		(0x01 << TIMRG_HALLSR_IN1_Pos)
#define TIMRG_HALLSR_IN2_Pos		2
#define TIMRG_HALLSR_IN2_Msk		(0x01 << TIMRG_HALLSR_IN2_Pos)

#define TIMRG_ICSR_TIMR0_Pos		0
#define TIMRG_ICSR_TIMR0_Msk		(0x01 << TIMRG_ICSR_TIMR0_Pos)
#define TIMRG_ICSR_TIMR1_Pos		1
#define TIMRG_ICSR_TIMR1_Msk		(0x01 << TIMRG_ICSR_TIMR1_Pos)
#define TIMRG_ICSR_TIMR2_Pos		2
#define TIMRG_ICSR_TIMR2_Msk		(0x01 << TIMRG_ICSR_TIMR2_Pos)

#define TIMRG_EN_TIMR0_Pos			0
#define TIMRG_EN_TIMR0_Msk			(0x01 << TIMRG_EN_TIMR0_Pos)
#define TIMRG_EN_TIMR1_Pos			1
#define TIMRG_EN_TIMR1_Msk			(0x01 << TIMRG_EN_TIMR1_Pos)
#define TIMRG_EN_TIMR2_Pos			2
#define TIMRG_EN_TIMR2_Msk			(0x01 << TIMRG_EN_TIMR2_Pos)
#define TIMRG_EN_TIMR3_Pos			3
#define TIMRG_EN_TIMR3_Msk			(0x01 << TIMRG_EN_TIMR3_Pos)




typedef struct {
	__IO uint32_t DATA;
	
	__IO uint32_t CTRL;
	
	__IO uint32_t BAUD;
	
	__IO uint32_t FIFO;
	
	__IO uint32_t LINCR;
	
	union {
		__IO uint32_t CTSCR;
		
		__IO uint32_t RTSCR;
	};
	
	__IO uint32_t CFG;
	
	__IO uint32_t TOCR;						//Timeout Control Register
} UART_TypeDef;


#define UART_DATA_DATA_Pos			0
#define UART_DATA_DATA_Msk			(0x1FF << UART_DATA_DATA_Pos)
#define UART_DATA_VALID_Pos			9		//��DATA�ֶ�����Ч�Ľ�������ʱ����λӲ����1����ȡ���ݺ��Զ�����
#define UART_DATA_VALID_Msk			(0x01 << UART_DATA_VALID_Pos)
#define UART_DATA_PAERR_Pos			10		//Parity Error
#define UART_DATA_PAERR_Msk			(0x01 << UART_DATA_PAERR_Pos)

#define UART_CTRL_TXIDLE_Pos		0		//TX IDLE: 0 ���ڷ�������	1 ����״̬��û�����ݷ���
#define UART_CTRL_TXIDLE_Msk		(0x01 << UART_CTRL_TXIDLE_Pos)
#define UART_CTRL_TXFF_Pos		    1		//TX FIFO Full
#define UART_CTRL_TXFF_Msk		    (0x01 << UART_CTRL_TXFF_Pos)
#define UART_CTRL_TXIE_Pos			2		//TX �ж�ʹ��: 1 TX FF �����������趨����ʱ�����ж�
#define UART_CTRL_TXIE_Msk			(0x01 << UART_CTRL_TXIE_Pos)
#define UART_CTRL_RXNE_Pos			3		//RX FIFO Not Empty
#define UART_CTRL_RXNE_Msk			(0x01 << UART_CTRL_RXNE_Pos)
#define UART_CTRL_RXIE_Pos			4		//RX �ж�ʹ��: 1 RX FF �����ݴﵽ�趨����ʱ�����ж�
#define UART_CTRL_RXIE_Msk			(0x01 << UART_CTRL_RXIE_Pos)
#define UART_CTRL_RXOV_Pos			5		//RX FIFO Overflow��д1����
#define UART_CTRL_RXOV_Msk			(0x01 << UART_CTRL_RXOV_Pos)
#define UART_CTRL_TXDOIE_Pos		6		//TX Done �ж�ʹ�ܣ�����FIFO���ҷ��ͷ�����λ�Ĵ����ѽ����һλ���ͳ�ȥ
#define UART_CTRL_TXDOIE_Msk		(0x01 << UART_CTRL_TXDOIE_Pos)
#define UART_CTRL_EN_Pos			9
#define UART_CTRL_EN_Msk			(0x01 << UART_CTRL_EN_Pos)
#define UART_CTRL_LOOP_Pos			10
#define UART_CTRL_LOOP_Msk			(0x01 << UART_CTRL_LOOP_Pos)
#define UART_CTRL_TOIE_Pos			14		//TimeOut �ж�ʹ�ܣ����յ��ϸ��ַ��󣬳��� TOTIME/BAUDRAUD ��û�н��յ��µ�����
#define UART_CTRL_TOIE_Msk			(0x01 << UART_CTRL_TOIE_Pos)
#define UART_CTRL_DATA9b_Pos		18		//1 9λ����λ    0 8λ����λ
#define UART_CTRL_DATA9b_Msk		(0x01 << UART_CTRL_DATA9b_Pos)
#define UART_CTRL_PARITY_Pos		19		//000 ��У��    001 ��У��   011 żУ��   101 �̶�Ϊ1    111 �̶�Ϊ0
#define UART_CTRL_PARITY_Msk		(0x07 << UART_CTRL_PARITY_Pos)
#define UART_CTRL_STOP2b_Pos		22		//1 2λֹͣλ    0 1λֹͣλ
#define UART_CTRL_STOP2b_Msk		(0x03 << UART_CTRL_STOP2b_Pos)

#define UART_BAUD_BAUD_Pos			0		//���ڲ����� = SYS_Freq/16/BAUD - 1
#define UART_BAUD_BAUD_Msk			(0x3FFF << UART_BAUD_BAUD_Pos)
#define UART_BAUD_TXD_Pos			14		//ͨ����λ��ֱ�Ӷ�ȡ����TXD�����ϵĵ�ƽ
#define UART_BAUD_TXD_Msk			(0x01 << UART_BAUD_TXD_Pos)
#define UART_BAUD_RXD_Pos			15		//ͨ����λ��ֱ�Ӷ�ȡ����RXD�����ϵĵ�ƽ
#define UART_BAUD_RXD_Msk			(0x01 << UART_BAUD_RXD_Pos)
#define UART_BAUD_RXTOIF_Pos		16		//����&��ʱ���жϱ�־ = RXIF | TOIF
#define UART_BAUD_RXTOIF_Msk		(0x01 << UART_BAUD_RXTOIF_Pos)
#define UART_BAUD_TXIF_Pos			17		//�����жϱ�־ = TXTHRF & TXIE
#define UART_BAUD_TXIF_Msk			(0x01 << UART_BAUD_TXIF_Pos)
#define UART_BAUD_RXTHRF_Pos		19		//RX FIFO Threshold Flag��RX FIFO�����ݴﵽ�趨������RXLVL >  RXTHR��ʱӲ����1
#define UART_BAUD_RXTHRF_Msk		(0x01 << UART_BAUD_RXTHRF_Pos)
#define UART_BAUD_TXTHRF_Pos		20		//TX FIFO Threshold Flag��TX FIFO�����������趨������TXLVL <= TXTHR��ʱӲ����1
#define UART_BAUD_TXTHRF_Msk		(0x01 << UART_BAUD_TXTHRF_Pos)
#define UART_BAUD_TOIF_Pos			21		//TimeOut �жϱ�־������ TOTIME/BAUDRAUD ��û�н��յ��µ�����ʱ��TOIE=1����λ��Ӳ����λ
#define UART_BAUD_TOIF_Msk			(0x01 << UART_BAUD_TOIF_Pos)
#define UART_BAUD_RXIF_Pos			22		//�����жϱ�־ = RXTHRF & RXIE
#define UART_BAUD_RXIF_Msk			(0x01 << UART_BAUD_RXIF_Pos)
#define UART_BAUD_ABREN_Pos			23		//Auto Baudrate Enable��д1�����Զ�������У׼����ɺ��Զ�����
#define UART_BAUD_ABREN_Msk			(0x01 << UART_BAUD_ABREN_Pos)
#define UART_BAUD_ABRBIT_Pos		24		//Auto Baudrate Bit�����ڼ��㲨���ʵļ��λ����0 1λ��ͨ������ʼλ           ������㲨���ʣ�Ҫ���Ͷ˷���0xFF
											//                                             1 2λ��ͨ������ʼλ��1λ����λ������㲨���ʣ�Ҫ���Ͷ˷���0xFE
											//                                             1 4λ��ͨ������ʼλ��3λ����λ������㲨���ʣ�Ҫ���Ͷ˷���0xF8
											//                                             1 8λ��ͨ������ʼλ��7λ����λ������㲨���ʣ�Ҫ���Ͷ˷���0x80
#define UART_BAUD_ABRBIT_Msk		(0x03 << UART_BAUD_ABRBIT_Pos)
#define UART_BAUD_ABRERR_Pos		26		//Auto Baudrate Error��0 �Զ�������У׼�ɹ�     1 �Զ�������У׼ʧ��
#define UART_BAUD_ABRERR_Msk		(0x01 << UART_BAUD_ABRERR_Pos)
#define UART_BAUD_TXDOIF_Pos		27		//TX Done �жϱ�־������FIFO���ҷ��ͷ�����λ�Ĵ����ѽ����һλ���ͳ�ȥ
#define UART_BAUD_TXDOIF_Msk		(0x01 << UART_BAUD_TXDOIF_Pos)
#define UART_BAUD_FRAC_Pos			28		//�����ʷ�ƵֵС������
#define UART_BAUD_FRAC_Msk			(0x0Fu << UART_BAUD_FRAC_Pos)

#define UART_FIFO_RXLVL_Pos			0		//RX FIFO Level��RX FIFO ���ַ�����
#define UART_FIFO_RXLVL_Msk			(0xFF << UART_FIFO_RXLVL_Pos)
#define UART_FIFO_TXLVL_Pos			8		//TX FIFO Level��TX FIFO ���ַ�����
#define UART_FIFO_TXLVL_Msk			(0xFF << UART_FIFO_TXLVL_Pos)
#define UART_FIFO_RXTHR_Pos			16		//RX FIFO Threshold��RX�жϴ������ޣ��ж�ʹ��ʱ RXLVL >  RXTHR ����RX�ж�
#define UART_FIFO_RXTHR_Msk			(0xFF << UART_FIFO_RXTHR_Pos)
#define UART_FIFO_TXTHR_Pos			24		//TX FIFO Threshold��TX�жϴ������ޣ��ж�ʹ��ʱ TXLVL <= TXTHR ����TX�ж�
#define UART_FIFO_TXTHR_Msk			(0xFFu<< UART_FIFO_TXTHR_Pos)

#define UART_LINCR_BRKDETIE_Pos		0		//��⵽LIN Break�ж�ʹ��
#define UART_LINCR_BRKDETIE_Msk		(0x01 << UART_LINCR_BRKDETIE_Pos)
#define UART_LINCR_BRKDETIF_Pos		1		//��⵽LIN Break�ж�״̬
#define UART_LINCR_BRKDETIF_Msk		(0x01 << UART_LINCR_BRKDETIF_Pos)
#define UART_LINCR_GENBRKIE_Pos		2		//����LIN Break����ж�ʹ��
#define UART_LINCR_GENBRKIE_Msk		(0x01 << UART_LINCR_GENBRKIE_Pos)
#define UART_LINCR_GENBRKIF_Pos		3		//����LIN Break����ж�״̬
#define UART_LINCR_GENBRKIF_Msk		(0x01 << UART_LINCR_GENBRKIF_Pos)
#define UART_LINCR_GENBRK_Pos		4		//����LIN Break����������Զ�����
#define UART_LINCR_GENBRK_Msk		(0x01 << UART_LINCR_GENBRK_Pos)

#define UART_CTSCR_EN_Pos			0		//CTS����ʹ��
#define UART_CTSCR_EN_Msk			(0x01 << UART_CTSCR_EN_Pos)
#define UART_CTSCR_POL_Pos			2		//CTS�źż��ԣ�0 ����Ч��CTS����Ϊ�ͱ�ʾ���Է�������
#define UART_CTSCR_POL_Msk			(0x01 << UART_CTSCR_POL_Pos)
#define UART_CTSCR_STAT_Pos			7		//CTS�źŵĵ�ǰ״̬
#define UART_CTSCR_STAT_Msk			(0x01 << UART_CTSCR_STAT_Pos)

#define UART_RTSCR_EN_Pos			1		//RTS����ʹ��
#define UART_RTSCR_EN_Msk			(0x01 << UART_RTSCR_EN_Pos)
#define UART_RTSCR_POL_Pos			3		//RTS�źż���    0 ����Ч��RTS����Ϊ�ͱ�ʾ���Խ�������
#define UART_RTSCR_POL_Msk			(0x01 << UART_RTSCR_POL_Pos)
#define UART_RTSCR_THR_Pos			4		//RTS���صĴ�����ֵ    0 1�ֽ�    1 2�ֽ�    2 4�ֽ�    3 6�ֽ�
#define UART_RTSCR_THR_Msk			(0x07 << UART_RTSCR_THR_Pos)
#define UART_RTSCR_STAT_Pos			8		//RTS�źŵĵ�ǰ״̬
#define UART_RTSCR_STAT_Msk			(0x01 << UART_RTSCR_STAT_Pos)

#define UART_CFG_MSBF_Pos			1		//���շ���MSB First
#define UART_CFG_MSBF_Msk			(0x01 << UART_CFG_MSBF_Pos)
#define UART_CFG_BRKTXLEN_Pos		2		//1��ʾ1bit���Դ����ƣ�Ĭ��ֵ13
#define UART_CFG_BRKTXLEN_Msk		(0x0F << UART_CFG_BRKTXLEN_Pos)
#define UART_CFG_BRKRXLEN_Pos		6		//0��ʾ1bit���Դ����ƣ�Ĭ��ֵ12
#define UART_CFG_BRKRXLEN_Msk		(0x0F << UART_CFG_BRKRXLEN_Pos)
#define UART_CFG_RXINV_Pos			10		//���յ�ƽ��ת
#define UART_CFG_RXINV_Msk			(0x01 << UART_CFG_RXINV_Pos)
#define UART_CFG_TXINV_Pos			11		//���͵�ƽ��ת
#define UART_CFG_TXINV_Msk			(0x01 << UART_CFG_TXINV_Pos)

#define UART_TOCR_TIME_Pos			0		//��ʱʱ�䳤�ȣ���λΪ 10/BAUDRATE ��
#define UART_TOCR_TIME_Msk			(0xFFF<< UART_TOCR_TIME_Pos)
#define UART_TOCR_MODE_Pos			12		//0 ֻ�е�FIFO������ʱ�Ŵ�����ʱ�ж�    1 ��ʹFIFO��û����Ҳ�ɴ�����ʱ�ж�
#define UART_TOCR_MODE_Msk			(0x01 << UART_TOCR_MODE_Pos)
#define UART_TOCR_IFCLR_Pos			13		//TO Interrupt Flag Clear��д1�����ʱ�жϱ�־
#define UART_TOCR_IFCLR_Msk			(0x01 << UART_TOCR_IFCLR_Pos)




typedef struct {
	__O  uint32_t CR;
	
	__IO uint32_t MR;
	
	__O  uint32_t IER;						//Interrupt Enable Register
	
	__O  uint32_t IDR;						//Interrupt Disable Register
	
	__I  uint32_t IMR;						//Interrupt Enable value register
	
	__I  uint32_t ISR;
	
	__I  uint32_t RHR;						//Receiver Holding Register
	
	__O  uint32_t THR;						//Transmitter Holding Register
	
	__IO uint32_t BAUD;						//Baud Rate
	
	__IO uint32_t RXTO;						//Receiver Time-out, 1-65535 bit period
	
		 uint32_t RESERVED[11];
	
	__IO uint32_t LINMR;					//LIN Mode Register
	
	__IO uint32_t LINID;
	
	__I  uint32_t LINBR;					//LIN Baud Rate Register
} USART_TypeDef;


#define USART_CR_RSTRX_Pos			2		//Reset Receiver
#define USART_CR_RSTRX_Msk			(0x01 << USART_CR_RSTRX_Pos)
#define USART_CR_RSTTX_Pos			3		//Reset Transmitter
#define USART_CR_RSTTX_Msk			(0x01 << USART_CR_RSTTX_Pos)
#define USART_CR_RXEN_Pos			4		//Receiver Enable
#define USART_CR_RXEN_Msk			(0x01 << USART_CR_RXEN_Pos)
#define USART_CR_RXDIS_Pos			5		//Receiver Disable
#define USART_CR_RXDIS_Msk			(0x01 << USART_CR_RXDIS_Pos)
#define USART_CR_TXEN_Pos			6		//Transmitter Enable
#define USART_CR_TXEN_Msk			(0x01 << USART_CR_TXEN_Pos)
#define USART_CR_TXDIS_Pos			7		//Transmitter Disable
#define USART_CR_TXDIS_Msk			(0x01 << USART_CR_TXDIS_Pos)
#define USART_CR_RSTSTA_Pos			8		//Reset Status Bits
#define USART_CR_RSTSTA_Msk			(0x01 << USART_CR_RSTSTA_Pos)
#define USART_CR_STTBRK_Pos			9		//Start Break
#define USART_CR_STTBRK_Msk			(0x01 << USART_CR_STTBRK_Pos)
#define USART_CR_STPBRK_Pos			10		//Stop Break
#define USART_CR_STPBRK_Msk			(0x01 << USART_CR_STPBRK_Pos)
#define USART_CR_STTTO_Pos			11		//Start Time-out
#define USART_CR_STTTO_Msk			(0x01 << USART_CR_STTTO_Pos)
#define USART_CR_RETTO_Pos			15		//Rearm Time-out
#define USART_CR_RETTO_Msk			(0x01 << USART_CR_RETTO_Pos)
#define USART_CR_LINABT_Pos			20		//Abort the current LIN transmission.
#define USART_CR_LINABT_Msk			(0x01 << USART_CR_LINABT_Pos)
#define USART_CR_LINWKUP_Pos		21		//Sends a wakeup signal on the LIN bus.
#define USART_CR_LINWKUP_Msk		(0x01 << USART_CR_LINWKUP_Pos)

#define USART_MR_MODE_Pos			0		//0 UART   10 LIN Master   11 LIN Slave
#define USART_MR_MODE_Msk			(0x0F << USART_MR_MODE_Pos)
#define USART_MR_CLKS_Pos			4		//Clock source
#define USART_MR_CLKS_Msk			(0x03 << USART_MR_CLKS_Pos)
#define USART_MR_NBDATA_Pos			6		//Number of Data bits, 0 5bit   1 6bit   2 7bit   3 8bit
#define USART_MR_NBDATA_Msk			(0x03 << USART_MR_NBDATA_Pos)
#define USART_MR_PARITY_Pos			9		//0 Even parity   1 Odd parity   2 force 0   3 force 1   4 No parity   6 Multidrop mode
#define USART_MR_PARITY_Msk			(0x07 << USART_MR_PARITY_Pos)
#define USART_MR_NBSTOP_Pos			12		//Number of Stop bits, 0 1bit   1 1.5bit   2 2bit
#define USART_MR_NBSTOP_Msk			(0x03 << USART_MR_NBSTOP_Pos)
#define USART_MR_MSBF_Pos			16		//MSB first
#define USART_MR_MSBF_Msk			(0x01 << USART_MR_MSBF_Pos)
#define USART_MR_DATA9b_Pos			17		//1 9-bit data length
#define USART_MR_DATA9b_Msk			(0x01 << USART_MR_DATA9b_Pos)
#define USART_MR_OVER8_Pos			19		//0 16x Oversampling   1 8x Oversampling
#define USART_MR_OVER8_Msk			(0x01 << USART_MR_OVER8_Pos)

#define USART_IER_RXRDY_Pos			0
#define USART_IER_RXRDY_Msk			(0x01 << USART_IER_RXRDY_Pos)
#define USART_IER_TXRDY_Pos			1
#define USART_IER_TXRDY_Msk			(0x01 << USART_IER_TXRDY_Pos)
#define USART_IER_RXBRK_Pos			2
#define USART_IER_RXBRK_Msk			(0x01 << USART_IER_RXBRK_Pos)
#define USART_IER_OVRERR_Pos		5
#define USART_IER_OVRERR_Msk		(0x01 << USART_IER_OVRERR_Pos)
#define USART_IER_FRAMERR_Pos		6
#define USART_IER_FRAMERR_Msk		(0x01 << USART_IER_FRAMERR_Pos)
#define USART_IER_PARITYERR_Pos		7
#define USART_IER_PARITYERR_Msk		(0x01 << USART_IER_PARITYERR_Pos)
#define USART_IER_RXTO_Pos			8
#define USART_IER_RXTO_Msk			(0x01 << USART_IER_RXTO_Pos)
#define USART_IER_TXEMPTY_Pos		9
#define USART_IER_TXEMPTY_Msk		(0x01 << USART_IER_TXEMPTY_Pos)
#define USART_IER_TXBEMPTY_Pos		11
#define USART_IER_TXBEMPTY_Msk		(0x01 << USART_IER_TXBEMPTY_Pos)
#define USART_IER_RXBFULL_Pos		12
#define USART_IER_RXBFULL_Msk		(0x01 << USART_IER_RXBFULL_Pos)
#define USART_IER_BRK_Pos			13
#define USART_IER_BRK_Msk			(0x01 << USART_IER_BRK_Pos)
#define USART_IER_ID_Pos			14
#define USART_IER_ID_Msk			(0x01 << USART_IER_ID_Pos)
#define USART_IER_DONE_Pos			15
#define USART_IER_DONE_Msk			(0x01 << USART_IER_DONE_Pos)
#define USART_IER_BITERR_Pos		25
#define USART_IER_BITERR_Msk		(0x01 << USART_IER_BITERR_Pos)
#define USART_IER_SYNCERR_Pos		26
#define USART_IER_SYNCERR_Msk		(0x01 << USART_IER_SYNCERR_Pos)
#define USART_IER_IDERR_Pos			27
#define USART_IER_IDERR_Msk			(0x01 << USART_IER_IDERR_Pos)
#define USART_IER_CHKERR_Pos		28
#define USART_IER_CHKERR_Msk		(0x01 << USART_IER_CHKERR_Pos)
#define USART_IER_NAKERR_Pos		29
#define USART_IER_NAKERR_Msk		(0x01 << USART_IER_NAKERR_Pos)
#define USART_IER_HDRTO_Pos			31
#define USART_IER_HDRTO_Msk			(0x01 << USART_IER_HDRTO_Pos)

#define USART_IDR_RXRDY_Pos			0
#define USART_IDR_RXRDY_Msk			(0x01 << USART_IDR_RXRDY_Pos)
#define USART_IDR_TXRDY_Pos			1
#define USART_IDR_TXRDY_Msk			(0x01 << USART_IDR_TXRDY_Pos)
#define USART_IDR_RXBRK_Pos			2
#define USART_IDR_RXBRK_Msk			(0x01 << USART_IDR_RXBRK_Pos)
#define USART_IDR_OVRERR_Pos		5
#define USART_IDR_OVRERR_Msk		(0x01 << USART_IDR_OVRERR_Pos)
#define USART_IDR_FRAMERR_Pos		6
#define USART_IDR_FRAMERR_Msk		(0x01 << USART_IDR_FRAMERR_Pos)
#define USART_IDR_PARITYERR_Pos		7
#define USART_IDR_PARITYERR_Msk		(0x01 << USART_IDR_PARITYERR_Pos)
#define USART_IDR_RXTO_Pos			8
#define USART_IDR_RXTO_Msk			(0x01 << USART_IDR_RXTO_Pos)
#define USART_IDR_TXEMPTY_Pos		9
#define USART_IDR_TXEMPTY_Msk		(0x01 << USART_IDR_TXEMPTY_Pos)
#define USART_IDR_TXBEMPTY_Pos		11
#define USART_IDR_TXBEMPTY_Msk		(0x01 << USART_IDR_TXBEMPTY_Pos)
#define USART_IDR_RXBFULL_Pos		12
#define USART_IDR_RXBFULL_Msk		(0x01 << USART_IDR_RXBFULL_Pos)
#define USART_IDR_BRK_Pos			13
#define USART_IDR_BRK_Msk			(0x01 << USART_IDR_BRK_Pos)
#define USART_IDR_ID_Pos			14
#define USART_IDR_ID_Msk			(0x01 << USART_IDR_ID_Pos)
#define USART_IDR_DONE_Pos			15
#define USART_IDR_DONE_Msk			(0x01 << USART_IDR_DONE_Pos)
#define USART_IDR_BITERR_Pos		25
#define USART_IDR_BITERR_Msk		(0x01 << USART_IDR_BITERR_Pos)
#define USART_IDR_SYNCERR_Pos		26
#define USART_IDR_SYNCERR_Msk		(0x01 << USART_IDR_SYNCERR_Pos)
#define USART_IDR_IDERR_Pos			27
#define USART_IDR_IDERR_Msk			(0x01 << USART_IDR_IDERR_Pos)
#define USART_IDR_CHKERR_Pos		28
#define USART_IDR_CHKERR_Msk		(0x01 << USART_IDR_CHKERR_Pos)
#define USART_IDR_NAKERR_Pos		29
#define USART_IDR_NAKERR_Msk		(0x01 << USART_IDR_NAKERR_Pos)
#define USART_IDR_HDRTO_Pos			31
#define USART_IDR_HDRTO_Msk			(0x01 << USART_IDR_HDRTO_Pos)

#define USART_IMR_RXRDY_Pos			0
#define USART_IMR_RXRDY_Msk			(0x01 << USART_IMR_RXRDY_Pos)
#define USART_IMR_TXRDY_Pos			1
#define USART_IMR_TXRDY_Msk			(0x01 << USART_IMR_TXRDY_Pos)
#define USART_IMR_RXBRK_Pos			2
#define USART_IMR_RXBRK_Msk			(0x01 << USART_IMR_RXBRK_Pos)
#define USART_IMR_OVRERR_Pos		5
#define USART_IMR_OVRERR_Msk		(0x01 << USART_IMR_OVRERR_Pos)
#define USART_IMR_FRAMERR_Pos		6
#define USART_IMR_FRAMERR_Msk		(0x01 << USART_IMR_FRAMERR_Pos)
#define USART_IMR_PARITYERR_Pos		7
#define USART_IMR_PARITYERR_Msk		(0x01 << USART_IMR_PARITYERR_Pos)
#define USART_IMR_RXTO_Pos			8
#define USART_IMR_RXTO_Msk			(0x01 << USART_IMR_RXTO_Pos)
#define USART_IMR_TXEMPTY_Pos		9
#define USART_IMR_TXEMPTY_Msk		(0x01 << USART_IMR_TXEMPTY_Pos)
#define USART_IMR_TXBEMPTY_Pos		11
#define USART_IMR_TXBEMPTY_Msk		(0x01 << USART_IMR_TXBEMPTY_Pos)
#define USART_IMR_RXBFULL_Pos		12
#define USART_IMR_RXBFULL_Msk		(0x01 << USART_IMR_RXBFULL_Pos)
#define USART_IMR_BRK_Pos			13
#define USART_IMR_BRK_Msk			(0x01 << USART_IMR_BRK_Pos)
#define USART_IMR_ID_Pos			14
#define USART_IMR_ID_Msk			(0x01 << USART_IMR_ID_Pos)
#define USART_IMR_DONE_Pos			15
#define USART_IMR_DONE_Msk			(0x01 << USART_IMR_DONE_Pos)
#define USART_IMR_BITERR_Pos		25
#define USART_IMR_BITERR_Msk		(0x01 << USART_IMR_BITERR_Pos)
#define USART_IMR_SYNCERR_Pos		26
#define USART_IMR_SYNCERR_Msk		(0x01 << USART_IMR_SYNCERR_Pos)
#define USART_IMR_IDERR_Pos			27
#define USART_IMR_IDERR_Msk			(0x01 << USART_IMR_IDERR_Pos)
#define USART_IMR_CHKERR_Pos		28
#define USART_IMR_CHKERR_Msk		(0x01 << USART_IMR_CHKERR_Pos)
#define USART_IMR_NAKERR_Pos		29
#define USART_IMR_NAKERR_Msk		(0x01 << USART_IMR_NAKERR_Pos)
#define USART_IMR_HDRTO_Pos			31
#define USART_IMR_HDRTO_Msk			(0x01 << USART_IMR_HDRTO_Pos)

#define USART_ISR_RXRDY_Pos			0		//RHR �������ݣ��� RHR ����
#define USART_ISR_RXRDY_Msk			(0x01 << USART_ISR_RXRDY_Pos)
#define USART_ISR_TXRDY_Pos			1		//THR �������ݣ�д THR ����
#define USART_ISR_TXRDY_Msk			(0x01 << USART_ISR_TXRDY_Pos)
#define USART_ISR_RXBRK_Pos			2		//Break Received or End of Break detected, CR.RSTSTA д 1 ����
#define USART_ISR_RXBRK_Msk			(0x01 << USART_ISR_RXBRK_Pos)
#define USART_ISR_OVRERR_Pos		5		//�������CR.RSTSTA д 1 ����
#define USART_ISR_OVRERR_Msk		(0x01 << USART_ISR_OVRERR_Pos)
#define USART_ISR_FRAMERR_Pos		6		//֡��ʽ����CR.RSTSTA д 1 ����
#define USART_ISR_FRAMERR_Msk		(0x01 << USART_ISR_FRAMERR_Pos)
#define USART_ISR_PARITYERR_Pos		7		//У��λ����CR.RSTSTA д 1 ����
#define USART_ISR_PARITYERR_Msk		(0x01 << USART_ISR_PARITYERR_Pos)
#define USART_ISR_RXTO_Pos			8		//���ճ�ʱ����CR.STTTO д 1 ����
#define USART_ISR_RXTO_Msk			(0x01 << USART_ISR_RXTO_Pos)
#define USART_ISR_TXEMPTY_Pos		9		//THR �ͷ�����λ�Ĵ����н������ݣ�д THR ����
#define USART_ISR_TXEMPTY_Msk		(0x01 << USART_ISR_TXEMPTY_Pos)
#define USART_ISR_TXBEMPTY_Pos		11
#define USART_ISR_TXBEMPTY_Msk		(0x01 << USART_ISR_TXBEMPTY_Pos)
#define USART_ISR_RXBFULL_Pos		12
#define USART_ISR_RXBFULL_Msk		(0x01 << USART_ISR_RXBFULL_Pos)
#define USART_ISR_BRK_Pos			13		//LIN Break Sent or LIN Break Received, CR.RSTSTA д 1 ����
#define USART_ISR_BRK_Msk			(0x01 << USART_ISR_BRK_Pos)
#define USART_ISR_ID_Pos			14		//LIN Identifier Sent or LIN Identifier Received, CR.RSTSTA д 1 ����
#define USART_ISR_ID_Msk			(0x01 << USART_ISR_ID_Pos)
#define USART_ISR_DONE_Pos			15		//LIN Transfer Completed, CR.RSTSTA д 1 ����
#define USART_ISR_DONE_Msk			(0x01 << USART_ISR_DONE_Pos)
#define USART_ISR_BUSSTA_Pos		23		//LIN Bus Line Status
#define USART_ISR_BUSSTA_Msk		(0x01 << USART_ISR_BUSSTA_Pos)
#define USART_ISR_BITERR_Pos		25		//A Bit Error has been detected, CR.RSTSTA д 1 ����
#define USART_ISR_BITERR_Msk		(0x01 << USART_ISR_BITERR_Pos)
#define USART_ISR_SYNCERR_Pos		26		//LIN Slave ģʽ�£�a LIN Inconsistent Synch Field Error has been detected, CR.RSTSTA д 1 ����
#define USART_ISR_SYNCERR_Msk		(0x01 << USART_ISR_SYNCERR_Pos)
#define USART_ISR_IDERR_Pos			27		//A LIN Identifier Parity Error has been detected, CR.RSTSTA д 1 ����
#define USART_ISR_IDERR_Msk			(0x01 << USART_ISR_IDERR_Pos)
#define USART_ISR_CHKERR_Pos		28		//A LIN Checksum Error has been detected, CR.RSTSTA д 1 ����
#define USART_ISR_CHKERR_Msk		(0x01 << USART_ISR_CHKERR_Pos)
#define USART_ISR_NAKERR_Pos		29		//A LIN Slave Not Responding Error has been detected, CR.RSTSTA д 1 ����
#define USART_ISR_NAKERR_Msk		(0x01 << USART_ISR_NAKERR_Pos)
#define USART_ISR_HDRTO_Pos			31		//A LIN Header Timeout Error has been detected, CR.RSTSTA д 1 ����
#define USART_ISR_HDRTO_Msk			(0x01u<< USART_ISR_HDRTO_Pos)

#define USART_RHR_DATA_Pos			0
#define USART_RHR_DATA_Msk			(0x1FF<< USART_RHR_DATA_Pos)

#define USART_THR_DATA_Pos			0
#define USART_THR_DATA_Msk			(0x1FF<< USART_THR_DATA_Pos)

#define USART_BAUD_IDIV_Pos			0
#define USART_BAUD_IDIV_Msk			(0xFFFF << USART_BAUD_IDIV_Pos)
#define USART_BAUD_FDIV_Pos			16
#define USART_BAUD_FDIV_Msk			(0x07 << USART_BAUD_FDIV_Pos)

#define USART_LINMR_NACT_Pos		0		//Node Action, 0 transmit the response   1 receive the response   2 ignore
#define USART_LINMR_NACT_Msk		(0x03 << USART_LINMR_NACT_Pos)
#define USART_LINMR_PARDIS_Pos		2		//Parity Disable
#define USART_LINMR_PARDIS_Msk		(0x01 << USART_LINMR_PARDIS_Pos)
#define USART_LINMR_CHKDIS_Pos		3		//Checksum Disable
#define USART_LINMR_CHKDIS_Msk		(0x01 << USART_LINMR_CHKDIS_Pos)
#define USART_LINMR_CHKTYP_Pos		4		//0 LIN 2.0 Enhanced Checksum   1 LIN 1.3 Classic Checksum
#define USART_LINMR_CHKTYP_Msk		(0x01 << USART_LINMR_CHKTYP_Pos)
#define USART_LINMR_RDLMOD_Pos		5		//Response Data Length defined by: 0 DLC field   1 the bits 5 and 6 of LINID register
#define USART_LINMR_RDLMOD_Msk		(0x01 << USART_LINMR_RDLMOD_Pos)
#define USART_LINMR_FSMDIS_Pos		6		//Frame Slot Mode Disable
#define USART_LINMR_FSMDIS_Msk		(0x01 << USART_LINMR_FSMDIS_Pos)
#define USART_LINMR_WKUPTYP_Pos		7		//0 LIN 2.0 wakeup signal   1 LIN 1.3 wakeup signal
#define USART_LINMR_WKUPTYP_Msk		(0x01 << USART_LINMR_WKUPTYP_Pos)
#define USART_LINMR_DLC_Pos			8		//response data length is equal to DLC+1 bytes
#define USART_LINMR_DLC_Msk			(0xFF << USART_LINMR_DLC_Pos)
#define USART_LINMR_SYNCDIS_Pos		17		//Synchronization Disable
#define USART_LINMR_SYNCDIS_Msk		(0x01 << USART_LINMR_SYNCDIS_Pos)

#define USART_LINBR_IDIV_Pos		0		//Returns the baud rate value after the synchronization process completion.
#define USART_LINBR_IDIV_Msk		(0xFFFF << USART_LINBR_IDIV_Pos)
#define USART_LINBR_FDIV_Pos		16
#define USART_LINBR_FDIV_Msk		(0x07 << USART_LINBR_FDIV_Pos)




typedef struct {
	__IO uint32_t CTRL;

	__IO uint32_t DATA;

	__IO uint32_t STAT;

	__IO uint32_t IE;

	__IO uint32_t IF;
} SPI_TypeDef;


#define SPI_CTRL_CLKDIV_Pos			0		//Clock Divider, SPI����ʱ�� = SYS_Freq/pow(2, CLKDIV+2)
#define SPI_CTRL_CLKDIV_Msk			(0x07 << SPI_CTRL_CLKDIV_Pos)
#define SPI_CTRL_EN_Pos				3
#define SPI_CTRL_EN_Msk				(0x01 << SPI_CTRL_EN_Pos)
#define SPI_CTRL_SIZE_Pos			4		//Data Size Select, ȡֵ3--15����ʾ4--16λ
#define SPI_CTRL_SIZE_Msk			(0x0F << SPI_CTRL_SIZE_Pos)
#define SPI_CTRL_CPHA_Pos			8		//0 ��SCLK�ĵ�һ�������ز�������	1 ��SCLK�ĵڶ��������ز�������
#define SPI_CTRL_CPHA_Msk			(0x01 << SPI_CTRL_CPHA_Pos)
#define SPI_CTRL_CPOL_Pos			9		//0 ����״̬��SCLKΪ�͵�ƽ		  1 ����״̬��SCLKΪ�ߵ�ƽ
#define SPI_CTRL_CPOL_Msk			(0x01 << SPI_CTRL_CPOL_Pos)
#define SPI_CTRL_FFS_Pos			10		//Frame Format Select, 0 SPI	1 TI SSI	2 I2S	3 SPI Flash
#define SPI_CTRL_FFS_Msk			(0x03 << SPI_CTRL_FFS_Pos)
#define SPI_CTRL_MSTR_Pos			12		//Master, 1 ��ģʽ	0 ��ģʽ
#define SPI_CTRL_MSTR_Msk			(0x01 << SPI_CTRL_MSTR_Pos)
#define SPI_CTRL_FAST_Pos			13		//1 SPI����ʱ�� = SYS_Freq/2    0 SPI����ʱ����SPI->CTRL.CLKDIV����
#define SPI_CTRL_FAST_Msk			(0x01 << SPI_CTRL_FAST_Pos)
#define SPI_CTRL_DMATXEN_Pos		14		//1 ͨ��DMAдFIFO    0 ͨ��MCUдFIFO
#define SPI_CTRL_DMATXEN_Msk		(0x01 << SPI_CTRL_DMATXEN_Pos)
#define SPI_CTRL_DMARXEN_Pos		15		//1 ͨ��DMA��FIFO    0 ͨ��MCU��FIFO
#define SPI_CTRL_DMARXEN_Msk		(0x01 << SPI_CTRL_DMARXEN_Pos)
#define SPI_CTRL_FILTE_Pos			16		//1 ��SPI�����źŽ���ȥ������    0 ��SPI�����źŲ�����ȥ������
#define SPI_CTRL_FILTE_Msk			(0x01 << SPI_CTRL_FILTE_Pos)
#define SPI_CTRL_SSN_H_Pos			17		//0 ���������SSNʼ��Ϊ0    	 1 ���������ÿ�ַ�֮��ὫSSN���߰��SCLK����
#define SPI_CTRL_SSN_H_Msk			(0x01 << SPI_CTRL_SSN_H_Pos)
#define SPI_CTRL_RFTHR_Pos			18		//RX FIFO Threshold��0 ����FIFO��������1������   ...   7 ����FIFO��������8������
#define SPI_CTRL_RFTHR_Msk			(0x07 << SPI_CTRL_RFTHR_Pos)
#define SPI_CTRL_TFTHR_Pos			21		//TX FIFO Threshold��0 ����FIFO��������0������   ...   7 ����FIFO��������7������
#define SPI_CTRL_TFTHR_Msk			(0x07 << SPI_CTRL_TFTHR_Pos)
#define SPI_CTRL_RFCLR_Pos			24		//RX FIFO Clear
#define SPI_CTRL_RFCLR_Msk			(0x01 << SPI_CTRL_RFCLR_Pos)
#define SPI_CTRL_TFCLR_Pos			25		//TX FIFO Clear
#define SPI_CTRL_TFCLR_Msk			(0x01 << SPI_CTRL_TFCLR_Pos)
#define SPI_CTRL_LSBF_Pos			28		//LSB Fisrt
#define SPI_CTRL_LSBF_Msk			(0x01 << SPI_CTRL_LSBF_Pos)
#define SPI_CTRL_NSYNC_Pos			29		//1 ��SPI�����źŽ��в���ͬ��    0 ��SPI�����źŲ����в���ͬ��
#define SPI_CTRL_NSYNC_Msk			(0x01 << SPI_CTRL_NSYNC_Pos)

#define SPI_STAT_WTC_Pos			0		//Word Transmit Complete��ÿ�������һ����������Ӳ����1�����д1����
#define SPI_STAT_WTC_Msk			(0x01 << SPI_STAT_WTC_Pos)
#define SPI_STAT_TFE_Pos			1		//����FIFO Empty
#define SPI_STAT_TFE_Msk			(0x01 << SPI_STAT_TFE_Pos)
#define SPI_STAT_TFNF_Pos			2		//����FIFO Not Full
#define SPI_STAT_TFNF_Msk			(0x01 << SPI_STAT_TFNF_Pos)
#define SPI_STAT_RFNE_Pos			3		//����FIFO Not Empty
#define SPI_STAT_RFNE_Msk			(0x01 << SPI_STAT_RFNE_Pos)
#define SPI_STAT_RFF_Pos			4		//����FIFO Full
#define SPI_STAT_RFF_Msk			(0x01 << SPI_STAT_RFF_Pos)
#define SPI_STAT_RFOV_Pos			5		//����FIFO Overflow
#define SPI_STAT_RFOV_Msk			(0x01 << SPI_STAT_RFOV_Pos)
#define SPI_STAT_TFLVL_Pos			6		//����FIFO�����ݸ����� 0 TFNF=0ʱ��ʾFIFO����8�����ݣ�TFNF=1ʱ��ʾFIFO����0������	1--7 FIFO����1--7������
#define SPI_STAT_TFLVL_Msk			(0x07 << SPI_STAT_TFLVL_Pos)
#define SPI_STAT_RFLVL_Pos			9		//����FIFO�����ݸ����� 0 RFF =1ʱ��ʾFIFO����8�����ݣ�RFF =0ʱ��ʾFIFO����0������	1--7 FIFO����1--7������
#define SPI_STAT_RFLVL_Msk			(0x07 << SPI_STAT_RFLVL_Pos)
#define SPI_STAT_BUSY_Pos			15
#define SPI_STAT_BUSY_Msk			(0x01 << SPI_STAT_BUSY_Pos)

#define SPI_IE_RFOV_Pos				0
#define SPI_IE_RFOV_Msk				(0x01 << SPI_IE_RFOV_Pos)
#define SPI_IE_RFF_Pos				1
#define SPI_IE_RFF_Msk				(0x01 << SPI_IE_RFF_Pos)
#define SPI_IE_RFHF_Pos				2
#define SPI_IE_RFHF_Msk				(0x01 << SPI_IE_RFHF_Pos)
#define SPI_IE_TFE_Pos				3
#define SPI_IE_TFE_Msk				(0x01 << SPI_IE_TFE_Pos)
#define SPI_IE_TFHF_Pos				4		//����FIFO�����ݸ�������4
#define SPI_IE_TFHF_Msk				(0x01 << SPI_IE_TFHF_Pos)
#define SPI_IE_RFTHR_Pos			5		//����FIFO�����ݸ�������CTRL.RFTHR�趨ֵ�ж�ʹ��
#define SPI_IE_RFTHR_Msk			(0x01 << SPI_IE_RFTHR_Pos)
#define SPI_IE_TFTHR_Pos			6		//����FIFO�����ݸ���С��CTRL.TFTHR�趨ֵ�ж�ʹ��
#define SPI_IE_TFTHR_Msk			(0x01 << SPI_IE_TFTHR_Pos)
#define SPI_IE_WTC_Pos				8		//Word Transmit Complete
#define SPI_IE_WTC_Msk				(0x01 << SPI_IE_WTC_Pos)
#define SPI_IE_FTC_Pos				9		//Frame Transmit Complete
#define SPI_IE_FTC_Msk				(0x01 << SPI_IE_FTC_Pos)
#define SPI_IE_CSFALL_Pos			10		//�ӻ�ģʽ�£�CS�½����ж�ʹ��
#define SPI_IE_CSFALL_Msk			(0x01 << SPI_IE_CSFALL_Pos)
#define SPI_IE_CSRISE_Pos			11		//�ӻ�ģʽ�£�CS�������ж�ʹ��
#define SPI_IE_CSRISE_Msk			(0x01 << SPI_IE_CSRISE_Pos)

#define SPI_IF_RFOV_Pos				0		//д1����
#define SPI_IF_RFOV_Msk				(0x01 << SPI_IF_RFOV_Pos)
#define SPI_IF_RFF_Pos				1		//д1����
#define SPI_IF_RFF_Msk				(0x01 << SPI_IF_RFF_Pos)
#define SPI_IF_RFHF_Pos				2		//д1����
#define SPI_IF_RFHF_Msk				(0x01 << SPI_IF_RFHF_Pos)
#define SPI_IF_TFE_Pos				3		//д1����
#define SPI_IF_TFE_Msk				(0x01 << SPI_IF_TFE_Pos)
#define SPI_IF_TFHF_Pos				4		//д1����
#define SPI_IF_TFHF_Msk				(0x01 << SPI_IF_TFHF_Pos)
#define SPI_IF_RFTHR_Pos			5		//д1����
#define SPI_IF_RFTHR_Msk			(0x01 << SPI_IF_RFTHR_Pos)
#define SPI_IF_TFTHR_Pos			6		//д1����
#define SPI_IF_TFTHR_Msk			(0x01 << SPI_IF_TFTHR_Pos)
#define SPI_IF_WTC_Pos				8		//Word Transmit Complete��ÿ�������һ����������Ӳ����1
#define SPI_IF_WTC_Msk				(0x01 << SPI_IF_WTC_Pos)
#define SPI_IF_FTC_Pos				9		//Frame Transmit Complete��WTC��λʱ��TX FIFO�ǿյģ���FTC��λ
#define SPI_IF_FTC_Msk				(0x01 << SPI_IF_FTC_Pos)
#define SPI_IF_CSFALL_Pos			10
#define SPI_IF_CSFALL_Msk			(0x01 << SPI_IF_CSFALL_Pos)
#define SPI_IF_CSRISE_Pos			11
#define SPI_IF_CSRISE_Msk			(0x01 << SPI_IF_CSRISE_Pos)




typedef struct {
	__IO uint32_t CR;

	__IO uint32_t SR;

	__IO uint32_t TR;						//Transfer Register

	__IO uint32_t RXDATA;
	
	__IO uint32_t TXDATA;
	
	__IO uint32_t IF;
	
	__IO uint32_t IE;
	
		 uint32_t RESERVED1;
	
	__IO uint32_t MCR;						//Master Control Register
	
	__IO uint32_t CLK;
	
		 uint32_t RESERVED2[2];
	
	__IO uint32_t SCR;						//Slave Control Register
	
	__IO uint32_t SADDR;
} I2C_TypeDef;


#define I2C_CR_EN_Pos				0	
#define I2C_CR_EN_Msk				(0x01 << I2C_CR_EN_Pos)
#define I2C_CR_MASTER_Pos			1		//1 Master   0 Slave
#define I2C_CR_MASTER_Msk			(0x01 << I2C_CR_MASTER_Pos)
#define I2C_CR_HS_Pos				2		//1 High-Speed mode    0 Standard-mode or Fast-mode
#define I2C_CR_HS_Msk				(0x01 << I2C_CR_HS_Pos)
#define I2C_CR_DNF_Pos				3		//Digital Noise Filter, ��ȵ��� DNF+1 ���ĵ�ƽ����Ϊ��ë��
#define I2C_CR_DNF_Msk				(0x0F << I2C_CR_DNF_Pos)

#define I2C_SR_BUSY_Pos				0
#define I2C_SR_BUSY_Msk				(0x01 << I2C_SR_BUSY_Pos)
#define I2C_SR_SCL_Pos				1		//SCL Line Level
#define I2C_SR_SCL_Msk				(0x01 << I2C_SR_SCL_Pos)
#define I2C_SR_SDA_Pos				2		//SDA Line Level
#define I2C_SR_SDA_Msk				(0x01 << I2C_SR_SDA_Pos)

#define I2C_TR_TXACK_Pos			0		//��Ϊ����ʱ������ACKλ�ĵ�ƽֵ
#define I2C_TR_TXACK_Msk			(0x01 << I2C_TR_TXACK_Pos)
#define I2C_TR_RXACK_Pos			1		//��Ϊ����ʱ�����յ���ACKλ��ƽֵ
#define I2C_TR_RXACK_Msk			(0x01 << I2C_TR_RXACK_Pos)
#define I2C_TR_TXCLR_Pos			2		//TX Data Clear, �Զ�����
#define I2C_TR_TXCLR_Msk			(0x01 << I2C_TR_TXCLR_Pos)
#define I2C_TR_SLVACT_Pos			8		//Slave Active, �ӻ�ģʽ�±�ѡ��ʱ��λ
#define I2C_TR_SLVACT_Msk			(0x01 << I2C_TR_SLVACT_Pos)
#define I2C_TR_SLVRD_Pos			9		//Slave Read mode���ӻ�ģʽ�½��յ�������ʱ��λ
#define I2C_TR_SLVRD_Msk			(0x01 << I2C_TR_SLVRD_Pos)
#define I2C_TR_SLVWR_Pos			10		//Slave Write mode���ӻ�ģʽ�½��յ�д����ʱ��λ
#define I2C_TR_SLVWR_Msk			(0x01 << I2C_TR_SLVWR_Pos)
#define I2C_TR_SLVSTR_Pos			11		//Slave clock stretching
#define I2C_TR_SLVSTR_Msk			(0x01 << I2C_TR_SLVSTR_Pos)
#define I2C_TR_SLVRDS_Pos			12		//Slave RXDATA Status, 0 ��   1 ���յ���ַ   2 ���յ�����   3 ���յ�Master Code
#define I2C_TR_SLVRDS_Msk			(0x03 << I2C_TR_SLVRDS_Pos)

#define I2C_IF_TXE_Pos				0		//TX Empty��дTXDATA�����λ
#define I2C_IF_TXE_Msk				(0x01 << I2C_IF_TXE_Pos)
#define I2C_IF_RXNE_Pos				1		//RX Not Empty����RXDATA�����λ
#define I2C_IF_RXNE_Msk				(0x01 << I2C_IF_RXNE_Pos)
#define I2C_IF_RXOV_Pos				2		//RX Overflow��д1����
#define I2C_IF_RXOV_Msk				(0x01 << I2C_IF_RXOV_Pos)
#define I2C_IF_TXDONE_Pos			3		//TX Done��д1����
#define I2C_IF_TXDONE_Msk			(0x01 << I2C_IF_TXDONE_Pos)
#define I2C_IF_RXDONE_Pos			4		//RX Done��д1����
#define I2C_IF_RXDONE_Msk			(0x01 << I2C_IF_RXDONE_Pos)
#define I2C_IF_RXSTA_Pos			8		//�ӻ����յ���ʼλ��д1����
#define I2C_IF_RXSTA_Msk			(0x01 << I2C_IF_RXSTA_Pos)
#define I2C_IF_RXSTO_Pos			9		//�ӻ����յ�ֹͣλ��д1����
#define I2C_IF_RXSTO_Msk			(0x01 << I2C_IF_RXSTO_Pos)
#define I2C_IF_AL_Pos				16		//�����ٲö�ʧ���ߣ�д1����
#define I2C_IF_AL_Msk				(0x01 << I2C_IF_AL_Pos)
#define I2C_IF_MLTO_Pos				17		//Master SCL Low Timeout��д1����
#define I2C_IF_MLTO_Msk				(0x01 << I2C_IF_MLTO_Pos)

#define I2C_IE_TXE_Pos				0
#define I2C_IE_TXE_Msk				(0x01 << I2C_IE_TXE_Pos)
#define I2C_IE_RXNE_Pos				1
#define I2C_IE_RXNE_Msk				(0x01 << I2C_IE_RXNE_Pos)
#define I2C_IE_RXOV_Pos				2
#define I2C_IE_RXOV_Msk				(0x01 << I2C_IE_RXOV_Pos)
#define I2C_IE_TXDONE_Pos			3
#define I2C_IE_TXDONE_Msk			(0x01 << I2C_IE_TXDONE_Pos)
#define I2C_IE_RXDONE_Pos			4
#define I2C_IE_RXDONE_Msk			(0x01 << I2C_IE_RXDONE_Pos)
#define I2C_IE_RXSTA_Pos			8
#define I2C_IE_RXSTA_Msk			(0x01 << I2C_IE_RXSTA_Pos)
#define I2C_IE_RXSTO_Pos			9
#define I2C_IE_RXSTO_Msk			(0x01 << I2C_IE_RXSTO_Pos)
#define I2C_IE_AL_Pos				16
#define I2C_IE_AL_Msk				(0x01 << I2C_IE_AL_Pos)
#define I2C_IE_MLTO_Pos				17
#define I2C_IE_MLTO_Msk				(0x01 << I2C_IE_MLTO_Pos)

#define I2C_MCR_STA_Pos				0		//д1������ʼλ����ɺ��Զ�����
#define I2C_MCR_STA_Msk				(0x01 << I2C_MCR_STA_Pos)
#define I2C_MCR_RD_Pos				1
#define I2C_MCR_RD_Msk				(0x01 << I2C_MCR_RD_Pos)
#define I2C_MCR_WR_Pos				2
#define I2C_MCR_WR_Msk				(0x01 << I2C_MCR_WR_Pos)
#define I2C_MCR_STO_Pos				3		//д1����ֹͣλ����ɺ��Զ�����
#define I2C_MCR_STO_Msk				(0x01 << I2C_MCR_STO_Pos)

#define I2C_CLK_SCLL_Pos			0		//SCL Low Time
#define I2C_CLK_SCLL_Msk			(0xFF << I2C_CLK_SCLL_Pos)
#define I2C_CLK_SCLH_Pos			8		//SCL High Time
#define I2C_CLK_SCLH_Msk			(0xFF << I2C_CLK_SCLH_Pos)
#define I2C_CLK_DIV_Pos				16
#define I2C_CLK_DIV_Msk				(0xFF << I2C_CLK_DIV_Pos)
#define I2C_CLK_SDAH_Pos			24		//SDA Hold Time
#define I2C_CLK_SDAH_Msk			(0x0F << I2C_CLK_SDAH_Pos)

#define I2C_SCR_ADDR10_Pos			0		//1 10λ��ַ    0 7λ��ַ
#define I2C_SCR_ADDR10_Msk			(0x01 << I2C_SCR_ADDR10_Pos)
#define I2C_SCR_MCDE_Pos			1		//Master Code Detect Enable
#define I2C_SCR_MCDE_Msk			(0x01 << I2C_SCR_MCDE_Pos)
#define I2C_SCR_STRE_Pos			2		//Clock Stretching Enable
#define I2C_SCR_STRE_Msk			(0x01 << I2C_SCR_STRE_Pos)
#define I2C_SCR_ASDS_Pos			3		//Adaptive Stretching Data Setup
#define I2C_SCR_ASDS_Msk			(0x01 << I2C_SCR_ASDS_Pos)

#define I2C_SADDR_ADDR7_Pos			1		//7λ��ַģʽ�µĵ�ַ
#define I2C_SADDR_ADDR7_Msk			(0x7F << I2C_SADDR_ADDR7_Pos)
#define I2C_SADDR_ADDR10_Pos		0		//10λ��ַģʽ�µĵ�ַ
#define I2C_SADDR_ADDR10_Msk		(0x3FF<< I2C_SADDR_ADDR10_Pos)
#define I2C_SADDR_MASK7_Pos			17		//7λ��ַģʽ�µĵ�ַ���룬ADDR7 & (~MASK7) ������յ�ַ�Ƚ�
#define I2C_SADDR_MASK7_Msk			(0x7F << I2C_SADDR_MASK7_Pos)
#define I2C_SADDR_MASK10_Pos		16		//10λ��ַģʽ�µĵ�ַ���룬ֻ�����8λ
#define I2C_SADDR_MASK10_Msk		(0xFF << I2C_SADDR_MASK10_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t IE;
	
	__IO uint32_t IF;
	
	__IO uint32_t SMPNUM;
	
	__IO uint32_t SMPTIM;
	
	__IO uint32_t SEQTRG;
		
	__IO uint32_t SEQ0CHN;
	
	__IO uint32_t SEQ1CHN;
	
	__IO uint32_t SEQ0CHK;
	    
	__IO uint32_t SEQ1CHK;
	
		 uint32_t RESERVED[2];
		
	__IO uint32_t DATA[10];
	
		 uint32_t RESERVED2[6];
	
	__IO uint32_t SEQ0DMA;
	
	__IO uint32_t SEQ1DMA;
	
		 uint32_t RESERVED3[98];
	
	__IO uint32_t START;
} ADC_TypeDef;


#define ADC_CR_PWDN_Pos				0		//1 Power Down   0 ��������ģʽ��д 0 ����ȴ� 32 ����������
#define ADC_CR_PWDN_Msk				(0x01 << ADC_CR_PWDN_Pos)
#define ADC_CR_RESET_Pos			1		//ģ��IP�ڲ��߼���λ��Ӳ���Զ�����
#define ADC_CR_RESET_Msk			(0x01 << ADC_CR_RESET_Pos)
#define ADC_CR_BITS_Pos				2 		//ת�����λ����0 12-bit   1 10-bit   2 8-bit   3 6-bit
#define ADC_CR_BITS_Msk				(0x03 << ADC_CR_BITS_Pos)
#define ADC_CR_SEQ0DMAEN_Pos		4
#define ADC_CR_SEQ0DMAEN_Msk		(0x01 << ADC_CR_SEQ0DMAEN_Pos)
#define ADC_CR_SEQ1DMAEN_Pos		5
#define ADC_CR_SEQ1DMAEN_Msk		(0x01 << ADC_CR_SEQ1DMAEN_Pos)
#define ADC_CR_AVG_Pos				6 		//
#define ADC_CR_AVG_Msk				(0x03 << ADC_CR_AVG_Pos)
#define ADC_CR_CLKDIV_Pos			8
#define ADC_CR_CLKDIV_Msk			(0x1F << ADC_CR_CLKDIV_Pos)

#define ADC_IE_SEQ0EOC_Pos			0
#define ADC_IE_SEQ0EOC_Msk			(0x01 << ADC_IE_SEQ0EOC_Pos)
#define ADC_IE_SEQ0MAX_Pos			1
#define ADC_IE_SEQ0MAX_Msk			(0x01 << ADC_IE_SEQ0MAX_Pos)
#define ADC_IE_SEQ0MIN_Pos			2
#define ADC_IE_SEQ0MIN_Msk			(0x01 << ADC_IE_SEQ0MIN_Pos)
#define ADC_IE_SEQ1EOC_Pos			8
#define ADC_IE_SEQ1EOC_Msk			(0x01 << ADC_IE_SEQ1EOC_Pos)
#define ADC_IE_SEQ1MAX_Pos			9
#define ADC_IE_SEQ1MAX_Msk			(0x01 << ADC_IE_SEQ1MAX_Pos)
#define ADC_IE_SEQ1MIN_Pos			10
#define ADC_IE_SEQ1MIN_Msk			(0x01 << ADC_IE_SEQ1MIN_Pos)

#define ADC_IF_SEQ0EOC_Pos			0
#define ADC_IF_SEQ0EOC_Msk			(0x01 << ADC_IF_SEQ0EOC_Pos)
#define ADC_IF_SEQ0MAX_Pos			1
#define ADC_IF_SEQ0MAX_Msk			(0x01 << ADC_IF_SEQ0MAX_Pos)
#define ADC_IF_SEQ0MIN_Pos			2
#define ADC_IF_SEQ0MIN_Msk			(0x01 << ADC_IF_SEQ0MIN_Pos)
#define ADC_IF_SEQ0BRK_Pos			3		//CPU����������PWM������ϣ�״̬λ���������ж�
#define ADC_IF_SEQ0BRK_Msk			(0x01 << ADC_IF_SEQ0BRK_Pos)
#define ADC_IF_SEQ1EOC_Pos			8
#define ADC_IF_SEQ1EOC_Msk			(0x01 << ADC_IF_SEQ1EOC_Pos)
#define ADC_IF_SEQ1MAX_Pos			9
#define ADC_IF_SEQ1MAX_Msk			(0x01 << ADC_IF_SEQ1MAX_Pos)
#define ADC_IF_SEQ1MIN_Pos			10
#define ADC_IF_SEQ1MIN_Msk			(0x01 << ADC_IF_SEQ1MIN_Pos)
#define ADC_IF_SEQ1BRK_Pos			11
#define ADC_IF_SEQ1BRK_Msk			(0x01 << ADC_IF_SEQ1BRK_Pos)

#define ADC_SMPNUM_SEQ0_Pos			0
#define ADC_SMPNUM_SEQ0_Msk			(0xFF << ADC_SMPNUM_SEQ0_Pos)
#define ADC_SMPNUM_SEQ1_Pos			8
#define ADC_SMPNUM_SEQ1_Msk			(0xFF << ADC_SMPNUM_SEQ1_Pos)

#define ADC_SMPTIM_SEQ0_Pos			0
#define ADC_SMPTIM_SEQ0_Msk			(0xFF << ADC_SMPTIM_SEQ0_Pos)
#define ADC_SMPTIM_SEQ1_Pos			8
#define ADC_SMPTIM_SEQ1_Msk			(0xFF << ADC_SMPTIM_SEQ1_Pos)

#define ADC_SEQTRG_SEQ0_Pos			0
#define ADC_SEQTRG_SEQ0_Msk			(0xFF << ADC_SEQTRG_SEQ0_Pos)
#define ADC_SEQTRG_SEQ1_Pos			8
#define ADC_SEQTRG_SEQ1_Msk			(0xFF << ADC_SEQTRG_SEQ1_Pos)

#define ADC_SEQ0CHN_CH0_Pos			0
#define ADC_SEQ0CHN_CH0_Msk			(0x0F << ADC_SEQ0CHN_CH0_Pos)
#define ADC_SEQ0CHN_CH1_Pos			4
#define ADC_SEQ0CHN_CH1_Msk			(0x0F << ADC_SEQ0CHN_CH1_Pos)
#define ADC_SEQ0CHN_CH2_Pos			8
#define ADC_SEQ0CHN_CH2_Msk			(0x0F << ADC_SEQ0CHN_CH2_Pos)
#define ADC_SEQ0CHN_CH3_Pos			12
#define ADC_SEQ0CHN_CH3_Msk			(0x0F << ADC_SEQ0CHN_CH3_Pos)
#define ADC_SEQ0CHN_CH4_Pos			16
#define ADC_SEQ0CHN_CH4_Msk			(0x0F << ADC_SEQ0CHN_CH4_Pos)
#define ADC_SEQ0CHN_CH5_Pos			20
#define ADC_SEQ0CHN_CH5_Msk			(0x0F << ADC_SEQ0CHN_CH5_Pos)
#define ADC_SEQ0CHN_CH6_Pos			24
#define ADC_SEQ0CHN_CH6_Msk			(0x0F << ADC_SEQ0CHN_CH6_Pos)
#define ADC_SEQ0CHN_CH7_Pos			28
#define ADC_SEQ0CHN_CH7_Msk			(0x0F << ADC_SEQ0CHN_CH7_Pos)

#define ADC_SEQ1CHN_CH0_Pos			0
#define ADC_SEQ1CHN_CH0_Msk			(0x0F << ADC_SEQ1CHN_CH0_Pos)
#define ADC_SEQ1CHN_CH1_Pos			4
#define ADC_SEQ1CHN_CH1_Msk			(0x0F << ADC_SEQ1CHN_CH1_Pos)
#define ADC_SEQ1CHN_CH2_Pos			8
#define ADC_SEQ1CHN_CH2_Msk			(0x0F << ADC_SEQ1CHN_CH2_Pos)
#define ADC_SEQ1CHN_CH3_Pos			12
#define ADC_SEQ1CHN_CH3_Msk			(0x0F << ADC_SEQ1CHN_CH3_Pos)
#define ADC_SEQ1CHN_CH4_Pos			16
#define ADC_SEQ1CHN_CH4_Msk			(0x0F << ADC_SEQ1CHN_CH4_Pos)
#define ADC_SEQ1CHN_CH5_Pos			20
#define ADC_SEQ1CHN_CH5_Msk			(0x0F << ADC_SEQ1CHN_CH5_Pos)
#define ADC_SEQ1CHN_CH6_Pos			24
#define ADC_SEQ1CHN_CH6_Msk			(0x0F << ADC_SEQ1CHN_CH6_Pos)
#define ADC_SEQ1CHN_CH7_Pos			28
#define ADC_SEQ1CHN_CH7_Msk			(0x0F << ADC_SEQ1CHN_CH7_Pos)

#define ADC_SEQ0CHK_MAX_Pos			0
#define ADC_SEQ0CHK_MAX_Msk			(0xFFF<< ADC_SEQ0CHK_MAX_Pos)
#define ADC_SEQ0CHK_MIN_Pos			16
#define ADC_SEQ0CHK_MIN_Msk			(0xFFF<< ADC_SEQ0CHK_MIN_Pos)

#define ADC_SEQ1CHK_MAX_Pos			0
#define ADC_SEQ1CHK_MAX_Msk			(0xFFF<< ADC_SEQ1CHK_MAX_Pos)
#define ADC_SEQ1CHK_MIN_Pos			16
#define ADC_SEQ1CHK_MIN_Msk			(0xFFF<< ADC_SEQ1CHK_MIN_Pos)

#define ADC_DATA_DATA_Pos			0
#define ADC_DATA_DATA_Msk			(0xFFF<< ADC_DATA_DATA_Pos)
#define ADC_DATA_FLAG_Pos			16		//0 ���ϴζ�ȡ��������   1 ��������   2 ���������ݸ���
#define ADC_DATA_FLAG_Msk			(0x03 << ADC_DATA_FLAG_Pos)

#define ADC_SEQ0DMA_DATA_Pos		0
#define ADC_SEQ0DMA_DATA_Msk		(0xFFF<< ADC_SEQ0DMA_DATA_Pos)
#define ADC_SEQ0DMA_CHNUM_Pos		12
#define ADC_SEQ0DMA_CHNUM_Msk		(0x0F << ADC_SEQ0DMA_CHNUM_Pos)
#define ADC_SEQ0DMA_FLAG_Pos		16
#define ADC_SEQ0DMA_FLAG_Msk		(0x03 << ADC_SEQ0DMA_FLAG_Pos)

#define ADC_SEQ1DMA_DATA_Pos		0
#define ADC_SEQ1DMA_DATA_Msk		(0xFFF<< ADC_SEQ1DMA_DATA_Pos)
#define ADC_SEQ1DMA_CHNUM_Pos		12
#define ADC_SEQ1DMA_CHNUM_Msk		(0x0F << ADC_SEQ1DMA_CHNUM_Pos)
#define ADC_SEQ1DMA_FLAG_Pos		16
#define ADC_SEQ1DMA_FLAG_Msk		(0x03 << ADC_SEQ1DMA_FLAG_Pos)

#define ADC_START_ADC0SEQ0_Pos		0
#define ADC_START_ADC0SEQ0_Msk		(0x01 << ADC_START_ADC0SEQ0_Pos)
#define ADC_START_ADC0SEQ1_Pos		1
#define ADC_START_ADC0SEQ1_Msk		(0x01 << ADC_START_ADC0SEQ1_Pos)
#define ADC_START_ADC0BUSY_Pos		2
#define ADC_START_ADC0BUSY_Msk		(0x01 << ADC_START_ADC0BUSY_Pos)
#define ADC_START_ADC1SEQ0_Pos		8
#define ADC_START_ADC1SEQ0_Msk		(0x01 << ADC_START_ADC1SEQ0_Pos)
#define ADC_START_ADC1SEQ1_Pos		9
#define ADC_START_ADC1SEQ1_Msk		(0x01 << ADC_START_ADC1SEQ1_Pos)
#define ADC_START_ADC1BUSY_Pos		10
#define ADC_START_ADC1BUSY_Msk		(0x01 << ADC_START_ADC1BUSY_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t OCR;
	
	__IO uint32_t BRKCR;
	
	__IO uint32_t BRKIN;
	
		 uint32_t RESERVED[4];
	
	__IO uint32_t PERIOD;                   //[15:0] ����
	
	__IO uint32_t CMPA;                   	//[15:0] A·��ת��Ƚ�ֵ
	
	__IO uint32_t CMPB;						//[15:0] B·��ת��Ƚ�ֵ
	
	__IO uint32_t DZA;                      //[9:0] ����
	
	__IO uint32_t DZB;
	
	__IO uint32_t CMPA2;					//�ǶԳ����Ķ���ģʽ�£����¼��������У�A·��ת��Ƚ�ֵ
	
	__IO uint32_t CMPB2;					//�ǶԳ����Ķ���ģʽ�£����¼��������У�B·��ת��Ƚ�ֵ
	
		 uint32_t RESERVED2[5];
	
	__IO uint32_t OVFTRG;
	
	__IO uint32_t CMPTRG;
	
	__IO uint32_t CMPTRG2;
	
		 uint32_t RESERVED3;
	
	__IO uint32_t EVMUX;
	
    __IO uint32_t EVMSK;
	
		 uint32_t RESERVED4[2];
	
	__IO uint32_t IE;
	
	__IO uint32_t IF;
	
	__IO uint32_t VALUE;
	
	__IO uint32_t SR;
} PWM_TypeDef;


#define PWM_CR_MODE_Pos				0		//0 ���ض���ģʽ   1 ���Ķ���ģʽ   2 �ǶԳ����Ķ���ģʽ
#define PWM_CR_MODE_Msk				(0x03 << PWM_CR_MODE_Pos)
#define PWM_CR_MULT_Pos				2		//0 ���μ���ģʽ   1 ��μ���ģʽ
#define PWM_CR_MULT_Msk				(0x01 << PWM_CR_MULT_Pos)
#define PWM_CR_DIR_Pos				3		//�������������� 0 ���ϼ���   1 ���¼���
#define PWM_CR_DIR_Msk				(0x01 << PWM_CR_DIR_Pos)
#define PWM_CR_CLKSRC_Pos			4		//����ʱ��Դ��0 ϵͳʱ��   1 PWM_PULSE0����   2 PWM_PULSE1����
#define PWM_CR_CLKSRC_Msk			(0x03 << PWM_CR_CLKSRC_Pos)
#define PWM_CR_CLKDIV_Pos			6		//����ʱ��Ԥ��Ƶ�� 0 1��Ƶ   1 2��Ƶ   ...   1023 1024��Ƶ
#define PWM_CR_CLKDIV_Msk			(0x3FF<< PWM_CR_CLKDIV_Pos)
#define PWM_CR_RPTNUM_Pos			16		//������������ٴ�ִ��һ�μĴ������أ�0 1��   1 2��   ...   255 256��
#define PWM_CR_RPTNUM_Msk			(0xFF << PWM_CR_RPTNUM_Pos)

#define PWM_OCR_IDLEA_Pos			0		//A·����ʱ�����ƽ
#define PWM_OCR_IDLEA_Msk			(0x01 << PWM_OCR_IDLEA_Pos)
#define PWM_OCR_IDLEB_Pos			1		//B·����ʱ�����ƽ
#define PWM_OCR_IDLEB_Msk			(0x01 << PWM_OCR_IDLEB_Pos)
#define PWM_OCR_IDLEAN_Pos			2		//AN·����ʱ�����ƽ
#define PWM_OCR_IDLEAN_Msk			(0x01 << PWM_OCR_IDLEAN_Pos)
#define PWM_OCR_IDLEBN_Pos			3		//BN·����ʱ�����ƽ
#define PWM_OCR_IDLEBN_Msk			(0x01 << PWM_OCR_IDLEBN_Pos)
#define PWM_OCR_INVA_Pos			4		//A·����Ƿ�ȡ��
#define PWM_OCR_INVA_Msk			(0x01 << PWM_OCR_INVA_Pos)
#define PWM_OCR_INVB_Pos			5		//B·����Ƿ�ȡ��
#define PWM_OCR_INVB_Msk			(0x01 << PWM_OCR_INVB_Pos)
#define PWM_OCR_INVAN_Pos			6		//AN·����Ƿ�ȡ��
#define PWM_OCR_INVAN_Msk			(0x01 << PWM_OCR_INVAN_Pos)
#define PWM_OCR_INVBN_Pos			7		//BN·����Ƿ�ȡ��
#define PWM_OCR_INVBN_Msk			(0x01 << PWM_OCR_INVBN_Pos)
#define PWM_OCR_FORCEA_Pos			8		//A·ǿ�����ʹ�ܣ�ǿ�Ƶ�ƽ�� IDLEA �趨
#define PWM_OCR_FORCEA_Msk			(0x01 << PWM_OCR_FORCEA_Pos)
#define PWM_OCR_FORCEB_Pos			9
#define PWM_OCR_FORCEB_Msk			(0x01 << PWM_OCR_FORCEB_Pos)
#define PWM_OCR_FORCEAN_Pos			10
#define PWM_OCR_FORCEAN_Msk			(0x01 << PWM_OCR_FORCEAN_Pos)
#define PWM_OCR_FORCEBN_Pos			11
#define PWM_OCR_FORCEBN_Msk			(0x01 << PWM_OCR_FORCEBN_Pos)

#define PWM_BRKCR_OUTA_Pos			0		//ɲ��״̬��A·�����ƽ
#define PWM_BRKCR_OUTA_Msk			(0x01 << PWM_BRKCR_OUTA_Pos)
#define PWM_BRKCR_OFFA_Pos			1		//ɲ���źų���ʱA·�����0 �����ָ��������   1 ���ֵ�ǰ���ֱ������������ٻָ��������
#define PWM_BRKCR_OFFA_Msk			(0x01 << PWM_BRKCR_OFFA_Pos)
#define PWM_BRKCR_OUTB_Pos			4		//ɲ��״̬��B·�����ƽ
#define PWM_BRKCR_OUTB_Msk			(0x01 << PWM_BRKCR_OUTB_Pos)
#define PWM_BRKCR_OFFB_Pos			5		//ɲ���źų���ʱB·�����0 �����ָ��������   1 ���ֵ�ǰ���ֱ������������ٻָ��������
#define PWM_BRKCR_OFFB_Msk			(0x01 << PWM_BRKCR_OFFB_Pos)
#define PWM_BRKCR_OUTAN_Pos			8		//ɲ��״̬��AN·�����ƽ
#define PWM_BRKCR_OUTAN_Msk			(0x01 << PWM_BRKCR_OUTAN_Pos)
#define PWM_BRKCR_OUTBN_Pos			9		//ɲ��״̬��BN·�����ƽ
#define PWM_BRKCR_OUTBN_Msk			(0x01 << PWM_BRKCR_OUTBN_Pos)
#define PWM_BRKCR_STPCNT_Pos		10		//ɲ��״̬���Ƿ�ֹͣ��������1 ֹͣ������   0 ��������
#define PWM_BRKCR_STPCNT_Msk		(0x01 << PWM_BRKCR_STPCNT_Pos)
#define PWM_BRKCR_SWHALT_Pos		16		//��ǰ�Ƿ������ɲ��״̬
#define PWM_BRKCR_SWHALT_Msk		(0x01 << PWM_BRKCR_SWHALT_Pos)
#define PWM_BRKCR_HWHALT_Pos		17		//��ǰ�Ƿ���Ӳ��ɲ��״̬
#define PWM_BRKCR_HWHALT_Msk		(0x01 << PWM_BRKCR_HWHALT_Pos)

#define PWM_BRKIN_BRK0A_Pos			0		//A·�Ƿ���ɲ������PWM_BRK0Ӱ��
#define PWM_BRKIN_BRK0A_Msk			(0x01 << PWM_BRKIN_BRK0A_Pos)
#define PWM_BRKIN_BRK1A_Pos			1
#define PWM_BRKIN_BRK1A_Msk			(0x01 << PWM_BRKIN_BRK1A_Pos)
#define PWM_BRKIN_BRK2A_Pos			2
#define PWM_BRKIN_BRK2A_Msk			(0x01 << PWM_BRKIN_BRK2A_Pos)
#define PWM_BRKIN_BRK0B_Pos			4
#define PWM_BRKIN_BRK0B_Msk			(0x01 << PWM_BRKIN_BRK0B_Pos)
#define PWM_BRKIN_BRK1B_Pos			5
#define PWM_BRKIN_BRK1B_Msk			(0x01 << PWM_BRKIN_BRK1B_Pos)
#define PWM_BRKIN_BRK2B_Pos			6
#define PWM_BRKIN_BRK2B_Msk			(0x01 << PWM_BRKIN_BRK2B_Pos)

#define PWM_OVFTRG_UPEN_Pos			0		//�������������Triggerʹ��
#define PWM_OVFTRG_UPEN_Msk			(0x01 << PWM_OVFTRG_UPEN_Pos)
#define PWM_OVFTRG_DNEN_Pos			1		//�������������Triggerʹ��
#define PWM_OVFTRG_DNEN_Msk			(0x01 << PWM_OVFTRG_DNEN_Pos)
#define PWM_OVFTRG_MUX_Pos			2		//Trigger�������һ·��0 trig[0]   1 trig[1]   2 trig[2]   ...   7 trig[7]
#define PWM_OVFTRG_MUX_Msk			(0x07 << PWM_OVFTRG_MUX_Pos)

#define PWM_CMPTRG_CMP_Pos			0		//������ֵ��˱Ƚ�ֵ���ʱ����Trigger�ź�
#define PWM_CMPTRG_CMP_Msk			(0xFFFF<<PWM_CMPTRG_CMP_Pos)
#define PWM_CMPTRG_EN_Pos			16
#define PWM_CMPTRG_EN_Msk			(0x01 << PWM_CMPTRG_EN_Pos)
#define PWM_CMPTRG_MUX_Pos			17		//Trigger�������һ·��0 trig[0]   1 trig[1]   2 trig[2]   ...   7 trig[7]
#define PWM_CMPTRG_MUX_Msk			(0x07 << PWM_CMPTRG_MUX_Pos)
#define PWM_CMPTRG_WIDTH_Pos		20		//Trigger����źſ�ȣ�0 �����   1 4������ʱ��   2 8������ʱ��   ...   63 252������ʱ��
#define PWM_CMPTRG_WIDTH_Msk		(0x3F << PWM_CMPTRG_WIDTH_Pos)
#define PWM_CMPTRG_DIR_Pos			28		//0 ���ϼ��������в���Trigger   1 ���¼��������в���Trigger
#define PWM_CMPTRG_DIR_Msk			(0x01 << PWM_CMPTRG_DIR_Pos)
#define PWM_CMPTRG_ATP_Pos			29		//AD�����ź������ڿ��е�λ�ã�0 0/8��   1 1/8��   ...   7 7/8��
#define PWM_CMPTRG_ATP_Msk			(0x07u<< PWM_CMPTRG_ATP_Pos)

#define PWM_CMPTRG2_INTV_Pos		0		//Compare Trigger Interval��0 ÿ���ڴ���   1 ���1���ڴ���һ��   2 ���2���ڴ���һ�� ...
#define PWM_CMPTRG2_INTV_Msk		(0x07 << PWM_CMPTRG2_INTV_Pos)

#define PWM_EVMUX_START_Pos			0
#define PWM_EVMUX_START_Msk			(0x07 << PWM_EVMUX_START_Pos)
#define PWM_EVMUX_STOP_Pos			4
#define PWM_EVMUX_STOP_Msk			(0x07 << PWM_EVMUX_STOP_Pos)
#define PWM_EVMUX_PAUSE_Pos			8
#define PWM_EVMUX_PAUSE_Msk			(0x07 << PWM_EVMUX_PAUSE_Pos)
#define PWM_EVMUX_RELOAD_Pos		12
#define PWM_EVMUX_RELOAD_Msk		(0x07 << PWM_EVMUX_RELOAD_Pos)
#define PWM_EVMUX_MASKA_Pos			16
#define PWM_EVMUX_MASKA_Msk			(0x07 << PWM_EVMUX_MASKA_Pos)
#define PWM_EVMUX_MASKB_Pos			20
#define PWM_EVMUX_MASKB_Msk			(0x07 << PWM_EVMUX_MASKB_Pos)
#define PWM_EVMUX_MASKAN_Pos		24
#define PWM_EVMUX_MASKAN_Msk		(0x07 << PWM_EVMUX_MASKAN_Pos)
#define PWM_EVMUX_MASKBN_Pos		28
#define PWM_EVMUX_MASKBN_Msk		(0x07 << PWM_EVMUX_MASKBN_Pos)

#define PWM_EVMSK_OUTA_Pos			0
#define PWM_EVMSK_OUTA_Msk			(0x01 << PWM_EVMSK_OUTA_Pos)
#define PWM_EVMSK_OUTB_Pos			1
#define PWM_EVMSK_OUTB_Msk			(0x01 << PWM_EVMSK_OUTB_Pos)
#define PWM_EVMSK_OUTAN_Pos			2
#define PWM_EVMSK_OUTAN_Msk			(0x01 << PWM_EVMSK_OUTAN_Pos)
#define PWM_EVMSK_OUTBN_Pos			3
#define PWM_EVMSK_OUTBN_Msk			(0x01 << PWM_EVMSK_OUTBN_Pos)
#define PWM_EVMSK_IMME_Pos			4		//1 MASK������Ч   0 ���������ʱ��Ч
#define PWM_EVMSK_IMME_Msk			(0x01 << PWM_EVMSK_IMME_Pos)
#define PWM_EVMSK_STPCLR_Pos		8		//�ⲿ�¼����¼�����ֹͣʱ�������Ƿ����㣬1 ����   0 ���ֵ�ǰֵ
#define PWM_EVMSK_STPCLR_Msk		(0x01 << PWM_EVMSK_STPCLR_Pos)

#define PWM_IE_UPOVF_Pos			0		//���ϼ���ʱ����������ж�ʹ��
#define PWM_IE_UPOVF_Msk			(0x01 << PWM_IE_UPOVF_Pos)
#define PWM_IE_DNOVF_Pos			1		//���¼���ʱ����������ж�ʹ��
#define PWM_IE_DNOVF_Msk			(0x01 << PWM_IE_DNOVF_Pos)
#define PWM_IE_UPCMPA_Pos			2		//���ϼ���ʱ������ֵ��CMPA����ж�ʹ��
#define PWM_IE_UPCMPA_Msk			(0x01 << PWM_IE_UPCMPA_Pos)
#define PWM_IE_UPCMPB_Pos			3		//���ϼ���ʱ������ֵ��CMPB����ж�ʹ��
#define PWM_IE_UPCMPB_Msk			(0x01 << PWM_IE_UPCMPB_Pos)
#define PWM_IE_DNCMPA_Pos			4		//���¼���ʱ������ֵ��CMPA����ж�ʹ��
#define PWM_IE_DNCMPA_Msk			(0x01 << PWM_IE_DNCMPA_Pos)
#define PWM_IE_DNCMPB_Pos			5		//���¼���ʱ������ֵ��CMPB����ж�ʹ��
#define PWM_IE_DNCMPB_Msk			(0x01 << PWM_IE_DNCMPB_Pos)

#define PWM_IF_UPOVF_Pos			0
#define PWM_IF_UPOVF_Msk			(0x01 << PWM_IF_UPOVF_Pos)
#define PWM_IF_DNOVF_Pos			1
#define PWM_IF_DNOVF_Msk			(0x01 << PWM_IF_DNOVF_Pos)
#define PWM_IF_UPCMPA_Pos			2
#define PWM_IF_UPCMPA_Msk			(0x01 << PWM_IF_UPCMPA_Pos)
#define PWM_IF_UPCMPB_Pos			3
#define PWM_IF_UPCMPB_Msk			(0x01 << PWM_IF_UPCMPB_Pos)
#define PWM_IF_DNCMPA_Pos			4
#define PWM_IF_DNCMPA_Msk			(0x01 << PWM_IF_DNCMPA_Pos)
#define PWM_IF_DNCMPB_Pos			5
#define PWM_IF_DNCMPB_Msk			(0x01 << PWM_IF_DNCMPB_Pos)

#define PWM_SR_STAT_Pos				0		//0 IDLE   1 ACTIVE   2 PAUSE
#define PWM_SR_STAT_Msk				(0x03 << PWM_SR_STAT_Pos)
#define PWM_SR_DIR_Pos				4		//0 ���ϼ���   1 ���¼���
#define PWM_SR_DIR_Msk				(0x01 << PWM_SR_DIR_Pos)
#define PWM_SR_OUTA_Pos				5
#define PWM_SR_OUTA_Msk				(0x01 << PWM_SR_OUTA_Pos)
#define PWM_SR_OUTB_Pos				6
#define PWM_SR_OUTB_Msk				(0x01 << PWM_SR_OUTB_Pos)
#define PWM_SR_OUTAN_Pos			7
#define PWM_SR_OUTAN_Msk			(0x01 << PWM_SR_OUTAN_Pos)
#define PWM_SR_OUTBN_Pos			8
#define PWM_SR_OUTBN_Msk			(0x01 << PWM_SR_OUTBN_Pos)


typedef struct {
	__IO uint32_t START;
	
	__IO uint32_t SWBRK;					//Software Brake�����ɲ��
    
    __IO uint32_t RESET;
	
	union {
		__IO uint32_t RELOADEN;
		
		__IO uint32_t RESTART;
	};
	
    __IO uint32_t PULSE;
	
    __IO uint32_t FILTER;					//�ⲿ�ź��˲���0 ���˲�   1 4��PCLK����   2 8��PCLK����   3 16��PCLK����
	
    __IO uint32_t BRKPOL;					//ɲ���źż��ԣ�
	
    __IO uint32_t BRKIE;
    
	union {
		__IO uint32_t BRKIF;
		
		__IO uint32_t BRKSR;
	};
	
	__IO uint32_t EVSR;
	
	__IO uint32_t SWEV;
} PWMG_TypeDef;


#define PWMG_START_PWM0_Pos			0
#define PWMG_START_PWM0_Msk			(0x01 << PWMG_START_PWM0_Pos)
#define PWMG_START_PWM1_Pos			1
#define PWMG_START_PWM1_Msk			(0x01 << PWMG_START_PWM1_Pos)

#define PWMG_SWBRK_PWM0A_Pos		0
#define PWMG_SWBRK_PWM0A_Msk		(0x01 << PWMG_SWBRK_PWM0A_Pos)
#define PWMG_SWBRK_PWM1A_Pos		1
#define PWMG_SWBRK_PWM1A_Msk		(0x01 << PWMG_SWBRK_PWM1A_Pos)
#define PWMG_SWBRK_PWM0B_Pos		8
#define PWMG_SWBRK_PWM0B_Msk		(0x01 << PWMG_SWBRK_PWM0B_Pos)
#define PWMG_SWBRK_PWM1B_Pos		9
#define PWMG_SWBRK_PWM1B_Msk		(0x01 << PWMG_SWBRK_PWM1B_Pos)

#define PWMG_RESET_PWM0_Pos			0
#define PWMG_RESET_PWM0_Msk			(0x01 << PWMG_RESET_PWM0_Pos)
#define PWMG_RESET_PWM1_Pos			1
#define PWMG_RESET_PWM1_Msk			(0x01 << PWMG_RESET_PWM1_Pos)

#define PWMG_RELOADEN_PWM0_Pos		0
#define PWMG_RELOADEN_PWM0_Msk		(0x01 << PWMG_RELOADEN_PWM0_Pos)
#define PWMG_RELOADEN_PWM1_Pos		1
#define PWMG_RELOADEN_PWM1_Msk		(0x01 << PWMG_RELOADEN_PWM1_Pos)

#define PWMG_RESTART_PWM0_Pos		8
#define PWMG_RESTART_PWM0_Msk		(0x01 << PWMG_RESTART_PWM0_Pos)
#define PWMG_RESTART_PWM1_Pos		9
#define PWMG_RESTART_PWM1_Msk		(0x01 << PWMG_RESTART_PWM1_Pos)

#define PWMG_PULSE_EDGE0_Pos		0		//PWM_PULSE0 �������أ�0 ������   1 �½���
#define PWMG_PULSE_EDGE0_Msk		(0x01 << PWMG_PULSE_EDGE0_Pos)
#define PWMG_PULSE_EDGE1_Pos		1
#define PWMG_PULSE_EDGE1_Msk		(0x01 << PWMG_PULSE_EDGE1_Pos)

#define PWMG_BRKPOL_BRK0_Pos		0		//PWMG_BRK0 ɲ���źż��ԣ�0 �͵�ƽɲ��   1 �ߵ�ƽɲ��
#define PWMG_BRKPOL_BRK0_Msk		(0x01 << PWMG_BRKPOL_BRK0_Pos)
#define PWMG_BRKPOL_BRK1_Pos		1
#define PWMG_BRKPOL_BRK1_Msk		(0x01 << PWMG_BRKPOL_BRK1_Pos)
#define PWMG_BRKPOL_BRK2_Pos		2
#define PWMG_BRKPOL_BRK2_Msk		(0x01 << PWMG_BRKPOL_BRK2_Pos)

#define PWMG_BRKIE_BRK0_Pos			0
#define PWMG_BRKIE_BRK0_Msk			(0x01 << PWMG_BRKIE_BRK0_Pos)
#define PWMG_BRKIE_BRK1_Pos			1
#define PWMG_BRKIE_BRK1_Msk			(0x01 << PWMG_BRKIE_BRK1_Pos)
#define PWMG_BRKIE_BRK2_Pos			2
#define PWMG_BRKIE_BRK2_Msk			(0x01 << PWMG_BRKIE_BRK2_Pos)

#define PWMG_BRKIF_BRK0_Pos			0
#define PWMG_BRKIF_BRK0_Msk			(0x01 << PWMG_BRKIF_BRK0_Pos)
#define PWMG_BRKIF_BRK1_Pos			1
#define PWMG_BRKIF_BRK1_Msk			(0x01 << PWMG_BRKIF_BRK1_Pos)
#define PWMG_BRKIF_BRK2_Pos			2
#define PWMG_BRKIF_BRK2_Msk			(0x01 << PWMG_BRKIF_BRK2_Pos)

#define PWMG_BRKSR_BRK0_Pos			4		//ɲ�����ŵ�ƽֵ
#define PWMG_BRKSR_BRK0_Msk			(0x01 << PWMG_BRKSR_BRK0_Pos)
#define PWMG_BRKSR_BRK1_Pos			5
#define PWMG_BRKSR_BRK1_Msk			(0x01 << PWMG_BRKSR_BRK1_Pos)
#define PWMG_BRKSR_BRK2_Pos			6
#define PWMG_BRKSR_BRK2_Msk			(0x01 << PWMG_BRKSR_BRK2_Pos)

#define PWMG_EVSR_EV0_Pos			0		//�ⲿ�¼��źŵ�ƽֵ
#define PWMG_EVSR_EV0_Msk			(0x01 << PWMG_EVSR_EV0_Pos)
#define PWMG_EVSR_EV1_Pos			1
#define PWMG_EVSR_EV1_Msk			(0x01 << PWMG_EVSR_EV1_Pos)
#define PWMG_EVSR_EV2_Pos			2
#define PWMG_EVSR_EV2_Msk			(0x01 << PWMG_EVSR_EV2_Pos)
#define PWMG_EVSR_EV3_Pos			3
#define PWMG_EVSR_EV3_Msk			(0x01 << PWMG_EVSR_EV3_Pos)
#define PWMG_EVSR_EV4_Pos			4
#define PWMG_EVSR_EV4_Msk			(0x01 << PWMG_EVSR_EV4_Pos)
#define PWMG_EVSR_EV5_Pos			5
#define PWMG_EVSR_EV5_Msk			(0x01 << PWMG_EVSR_EV5_Pos)
#define PWMG_EVSR_EV6_Pos			6
#define PWMG_EVSR_EV6_Msk			(0x01 << PWMG_EVSR_EV6_Pos)

#define PWMG_SWEV_EV2_Pos			0
#define PWMG_SWEV_EV2_Msk			(0x01 << PWMG_SWEV_EV2_Pos)
#define PWMG_SWEV_EV3_Pos			1
#define PWMG_SWEV_EV3_Msk			(0x01 << PWMG_SWEV_EV3_Pos)
#define PWMG_SWEV_EV4_Pos			2
#define PWMG_SWEV_EV4_Msk			(0x01 << PWMG_SWEV_EV4_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t POSCNT;					//[15:0] λ�ü�����
	__IO uint32_t MAXCNT;					//[15:0] ������ֵ
		
		 uint32_t RESERVED[5];
	
	__IO uint32_t IE;						//Interrupt Enable��Ϊ0ʱIF��Ӧλ����λ
	
	__IO uint32_t IM;						//Interrupt Mask��Ϊ0ʱ��ʹIF��Ӧλ��λҲ������ QEI_IRQn �ж�
	
	__O  uint32_t IC;						//Interrupt Clear
	
	__I  uint32_t IF;						//Interrupt Flag
	
	__IO uint32_t IFOV;						//interrupt Interrupt Overrun
} QEI_TypeDef;


#define QEI_CR_ENA_Pos   			0
#define QEI_CR_ENA_Msk				(0x01 << QEI_CR_ENA_Pos)
#define QEI_CR_ABSWAP_Pos  			4		//1 A��B���Ž���
#define QEI_CR_ABSWAP_Msk			(0x01 << QEI_CR_ABSWAP_Pos)
#define QEI_CR_X2X4_Pos  			5		//0 X2����ģʽ		1 X4����ģʽ
#define QEI_CR_X2X4_Msk				(0x01 << QEI_CR_X2X4_Pos)
#define QEI_CR_RSTSRC_Pos  			6		//Reset Source		0 ����ƥ�临λ		1 �����źŸ�λ
#define QEI_CR_RSTSRC_Msk			(0x01 << QEI_CR_RSTSRC_Pos)
#define QEI_CR_MODE_Pos  			7		//����ģʽѡ��		1 QEIģʽ
#define QEI_CR_MODE_Msk				(0x01 << QEI_CR_MODE_Pos)
#define QEI_CR_INDEX_Pos 			9		//0 ��������Ϊ�͵�ƽ		1 ��������Ϊ�ߵ�ƽ
#define QEI_CR_INDEX_Msk			(0x01 << QEI_CR_INDEX_Pos)
#define QEI_CR_PAUSE_Pos 			10		//1 ����ģʽֹͣλ
#define QEI_CR_PAUSE_Msk			(0x01 << QEI_CR_PAUSE_Pos)

#define QEI_IE_INDEX_Pos 			0		//��⵽Index����
#define QEI_IE_INDEX_Msk			(0x01 << QEI_IE_INDEX_Pos)
#define QEI_IE_MATCH_Pos 			1		//POSCNT��������MAXCNT��ȣ���POSCNT��MAXCNT�ݼ���0
#define QEI_IE_MATCH_Msk			(0x01 << QEI_IE_MATCH_Pos)
#define QEI_IE_CNTOV_Pos 			2		//Counter Overrun�����������
#define QEI_IE_CNTOV_Msk			(0x01 << QEI_IE_CNTOV_Pos)
#define QEI_IE_ERROR_Pos 			3		//����������
#define QEI_IE_ERROR_Msk			(0x01 << QEI_IE_ERROR_Pos)

#define QEI_IM_INDEX_Pos 			0
#define QEI_IM_INDEX_Msk			(0x01 << QEI_IM_INDEX_Pos)
#define QEI_IM_MATCH_Pos 			1
#define QEI_IM_MATCH_Msk			(0x01 << QEI_IM_MATCH_Pos)
#define QEI_IM_CNTOV_Pos 			2
#define QEI_IM_CNTOV_Msk			(0x01 << QEI_IM_CNTOV_Pos)
#define QEI_IM_ERROR_Pos 			3
#define QEI_IM_ERROR_Msk			(0x01 << QEI_IM_ERROR_Pos)

#define QEI_IC_INDEX_Pos 			0
#define QEI_IC_INDEX_Msk			(0x01 << QEI_IC_INDEX_Pos)
#define QEI_IC_MATCH_Pos 			1
#define QEI_IC_MATCH_Msk			(0x01 << QEI_IC_MATCH_Pos)
#define QEI_IC_CNTOV_Pos 			2
#define QEI_IC_CNTOV_Msk			(0x01 << QEI_IC_CNTOV_Pos)
#define QEI_IC_ERROR_Pos 			3
#define QEI_IC_ERROR_Msk			(0x01 << QEI_IC_ERROR_Pos)

#define QEI_IF_INDEX_Pos 			0
#define QEI_IF_INDEX_Msk			(0x01 << QEI_IF_INDEX_Pos)
#define QEI_IF_MATCH_Pos 			1
#define QEI_IF_MATCH_Msk			(0x01 << QEI_IF_MATCH_Pos)
#define QEI_IF_CNTOV_Pos 			2
#define QEI_IF_CNTOV_Msk			(0x01 << QEI_IF_CNTOV_Pos)
#define QEI_IF_ERROR_Pos 			3
#define QEI_IF_ERROR_Msk			(0x01 << QEI_IF_ERROR_Pos)

#define QEI_IFOV_INDEX_Pos 			0
#define QEI_IFOV_INDEX_Msk			(0x01 << QEI_IFOV_INDEX_Pos)
#define QEI_IFOV_MATCH_Pos 			1
#define QEI_IFOV_MATCH_Msk			(0x01 << QEI_IFOV_MATCH_Pos)
#define QEI_IFOV_CNTOV_Pos 			2
#define QEI_IFOV_CNTOV_Msk			(0x01 << QEI_IFOV_CNTOV_Pos)
#define QEI_IFOV_ERROR_Pos 			3
#define QEI_IFOV_ERROR_Msk			(0x01 << QEI_IFOV_ERROR_Pos)




typedef struct {    
	__IO uint32_t IF;						//Interrupt Flag
    
	__IO uint32_t IFC;						//Interrupt Flag Clear
    	
		 uint32_t RESERVED[2];
		
	struct {
		__IO uint32_t MUX;
		
		__IO uint32_t CR;
		
		__IO uint32_t NDT;					//Number of data to transfer
		
		__IO uint32_t PAR;					//Peripheral address register
		
		__IO uint32_t MAR;					//Memory address register
		
			 uint32_t RESERVED[3];
	} CH[2];
} DMA_TypeDef;


#define DMA_IF_GLB0_Pos				0		//Channel 0 global interrupt
#define DMA_IF_GLB0_Msk				(0x01 << DMA_IF_GLB0_Pos)
#define DMA_IF_DONE0_Pos			1		//Channel 0 transfer done
#define DMA_IF_DONE0_Msk			(0x01 << DMA_IF_DONE0_Pos)
#define DMA_IF_HALF0_Pos			2		//Channel 0 half transfer
#define DMA_IF_HALF0_Msk			(0x01 << DMA_IF_HALF0_Pos)
#define DMA_IF_ERR0_Pos				3		//Channel 0 transfer error
#define DMA_IF_ERR0_Msk				(0x01 << DMA_IF_ERR0_Pos)
#define DMA_IF_GLB1_Pos				4
#define DMA_IF_GLB1_Msk				(0x01 << DMA_IF_GLB1_Pos)
#define DMA_IF_DONE1_Pos			5
#define DMA_IF_DONE1_Msk			(0x01 << DMA_IF_DONE1_Pos)
#define DMA_IF_HALF1_Pos			6
#define DMA_IF_HALF1_Msk			(0x01 << DMA_IF_HALF1_Pos)
#define DMA_IF_ERR1_Pos				7
#define DMA_IF_ERR1_Msk				(0x01 << DMA_IF_ERR1_Pos)

#define DMA_IFC_GLB0_Pos			0
#define DMA_IFC_GLB0_Msk			(0x01 << DMA_IFC_GLB0_Pos)
#define DMA_IFC_DONE0_Pos			1
#define DMA_IFC_DONE0_Msk			(0x01 << DMA_IFC_DONE0_Pos)
#define DMA_IFC_HALF0_Pos			2
#define DMA_IFC_HALF0_Msk			(0x01 << DMA_IFC_HALF0_Pos)
#define DMA_IFC_ERR0_Pos			3
#define DMA_IFC_ERR0_Msk			(0x01 << DMA_IFC_ERR0_Pos)
#define DMA_IFC_GLB1_Pos			4
#define DMA_IFC_GLB1_Msk			(0x01 << DMA_IFC_GLB1_Pos)
#define DMA_IFC_DONE1_Pos			5
#define DMA_IFC_DONE1_Msk			(0x01 << DMA_IFC_DONE1_Pos)
#define DMA_IFC_HALF1_Pos			6
#define DMA_IFC_HALF1_Msk			(0x01 << DMA_IFC_HALF1_Pos)
#define DMA_IFC_ERR1_Pos			7
#define DMA_IFC_ERR1_Msk			(0x01 << DMA_IFC_ERR1_Pos)

#define DMA_MUX_MRDHSSIG_Pos		0		//memory read  handshake signal
#define DMA_MUX_MRDHSSIG_Msk		(0x03 << DMA_MUX_MRDHSSIG_Pos)
#define DMA_MUX_MRDHSEN_Pos			3		//memory read  handshake enable
#define DMA_MUX_MRDHSEN_Msk			(0x01 << DMA_MUX_MRDHSEN_Pos)
#define DMA_MUX_MWRHSSIG_Pos		4		//memory write handshake signal
#define DMA_MUX_MWRHSSIG_Msk		(0x03 << DMA_MUX_MWRHSSIG_Pos)
#define DMA_MUX_MWRHSEN_Pos			7		//memory write handshake enable
#define DMA_MUX_MWRHSEN_Msk			(0x01 << DMA_MUX_MWRHSEN_Pos)
#define DMA_MUX_EXTHSSIG_Pos		8		//�ⲿ�����źţ�0 TIMR0   1 TIMR1   2 TIMR2   3 TIMR3   4 TIMR4   5 DMA_TRIG0   6 DMA_TRIG1
#define DMA_MUX_EXTHSSIG_Msk		(0x07 << DMA_MUX_EXTHSSIG_Pos)
#define DMA_MUX_EXTHSEN_Pos			11
#define DMA_MUX_EXTHSEN_Msk			(0x01 << DMA_MUX_EXTHSEN_Pos)

#define DMA_CR_EN_Pos				0
#define DMA_CR_EN_Msk				(0x01 << DMA_CR_EN_Pos)
#define DMA_CR_DONEIE_Pos			1
#define DMA_CR_DONEIE_Msk			(0x01 << DMA_CR_DONEIE_Pos)
#define DMA_CR_HALFIE_Pos			2
#define DMA_CR_HALFIE_Msk			(0x01 << DMA_CR_HALFIE_Pos)
#define DMA_CR_ERRIE_Pos			3
#define DMA_CR_ERRIE_Msk			(0x01 << DMA_CR_ERRIE_Pos)
#define DMA_CR_DIR_Pos				4
#define DMA_CR_DIR_Msk				(0x01 << DMA_CR_DIR_Pos)
#define DMA_CR_CIRC_Pos				5
#define DMA_CR_CIRC_Msk				(0x01 << DMA_CR_CIRC_Pos)
#define DMA_CR_PINC_Pos				6
#define DMA_CR_PINC_Msk				(0x01 << DMA_CR_PINC_Pos)
#define DMA_CR_MINC_Pos				7
#define DMA_CR_MINC_Msk				(0x01 << DMA_CR_MINC_Pos)
#define DMA_CR_PSIZ_Pos				8
#define DMA_CR_PSIZ_Msk				(0x03 << DMA_CR_PSIZ_Pos)
#define DMA_CR_MSIZ_Pos				10
#define DMA_CR_MSIZ_Msk				(0x03 << DMA_CR_MSIZ_Pos)
#define DMA_CR_PL_Pos				12
#define DMA_CR_PL_Msk				(0x0F << DMA_CR_PL_Pos)
#define DMA_CR_MEM2MEM_Pos			16
#define DMA_CR_MEM2MEM_Msk			(0x01 << DMA_CR_MEM2MEM_Pos)

#define DMA_NDT_LEN_Pos				0		//ͨ���ر�ʱ��д��Ҫ��������ݸ�����ͨ��ʹ�ܺ�ָʾʣ��Ĵ�����������Ŀ
#define DMA_NDT_LEN_Msk				(0xFFFF << DMA_NDT_LEN_Pos)
#define DMA_NDT_HALF_Pos			16		//���� HALF ָ���������ݺ���λ DMA->IF.HALF �жϱ�־λ
#define DMA_NDT_HALF_Msk			(0xFFFF << DMA_NDT_HALF_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t DCR;						//Device Configuration Register
	
	__IO uint32_t SR;
	
	__IO uint32_t FCR;						//Flag Clear Register
	
	__IO uint32_t DLR;						//Data Length Register
											//Number of data to be retrieved in indirect and status-polling modes
	__IO uint32_t CCR;						//Communication Configuration Register
	
	__IO uint32_t AR;
	
	__IO uint32_t ABR;						//Alternate Bytes Registers
	
	union {
		__IO uint32_t DRW;
		
		__IO uint16_t DRH;
		
		__IO uint8_t  DRB;
	};
	
	__IO uint32_t PSMSK;					//Polling Status Mask
	
	__IO uint32_t PSMAT;					//Polling Status Match
	
	__IO uint32_t PSITV;					//Polling Status Interval
	
		 uint32_t RESERVED[4];
	
	__IO uint32_t SSHIFT;					//Sample Shift in System clock cycles, ʵ�ʵĲ����ӳ�ʱ���Ǵ˼Ĵ����� CR.SSHIFT �趨�ӳٵ��ۼ�
} QSPI_TypeDef;


#define QSPI_CR_EN_Pos				0
#define QSPI_CR_EN_Msk				(0x01 << QSPI_CR_EN_Pos)
#define QSPI_CR_ABORT_Pos			1
#define QSPI_CR_ABORT_Msk			(0x01 << QSPI_CR_ABORT_Pos)
#define QSPI_CR_DMAEN_Pos			2
#define QSPI_CR_DMAEN_Msk			(0x01 << QSPI_CR_DMAEN_Pos)
#define QSPI_CR_SSHIFT_Pos			4		//Sample Shift in QSPI clock cycle, 0 No shift   1 1/2 cycle shift
#define QSPI_CR_SSHIFT_Msk			(0x01 << QSPI_CR_SSHIFT_Pos)
#define QSPI_CR_BIDI_Pos			5		//����˫��ģʽ��0 IO0�����IO1����    1 IO0�����������
#define QSPI_CR_BIDI_Msk			(0x01 << QSPI_CR_BIDI_Pos)
#define QSPI_CR_FFTHR_Pos			8		//FIFO Threshold��indirect read  ģʽ�£�FIFO �����ݸ��� �� CR.FFTHR+1 ʱ��SR.FFTHR ��λ
											//				  indirect write ģʽ�£�FIFO �п�λ���� �� CR.FFTHR+1 ʱ��SR.FFTHR ��λ
#define QSPI_CR_FFTHR_Msk			(0x0F << QSPI_CR_FFTHR_Pos)
#define QSPI_CR_ERRIE_Pos			16		//Transfer Error Interrupt Enable
#define QSPI_CR_ERRIE_Msk			(0x01 << QSPI_CR_ERRIE_Pos)
#define QSPI_CR_DONEIE_Pos			17		//Transfer Done/Complete Interrupt Enable
#define QSPI_CR_DONEIE_Msk			(0x01 << QSPI_CR_DONEIE_Pos)
#define QSPI_CR_FFTHRIE_Pos			18		//FIFO Threshold Interrupt Enable
#define QSPI_CR_FFTHRIE_Msk			(0x01 << QSPI_CR_FFTHRIE_Pos)
#define QSPI_CR_PSMATIE_Pos			19		//Polling Status Match Interrupt Enable
#define QSPI_CR_PSMATIE_Msk			(0x01 << QSPI_CR_PSMATIE_Pos)
#define QSPI_CR_PSSTPMOD_Pos		22		//Polling Status Stop Mode��0 always polling until abort or QSPI disabled   1 stop polling as soon as match
#define QSPI_CR_PSSTPMOD_Msk		(0x01 << QSPI_CR_PSSTPMOD_Pos)
#define QSPI_CR_PSMATMOD_Pos		23		//Polling Status Match Mode��0 AND��match when all unmasked bits received from Flash match PSMAT register   1 OR
#define QSPI_CR_PSMATMOD_Msk		(0x01 << QSPI_CR_PSMATMOD_Pos)
#define QSPI_CR_CLKDIV_Pos			24		//QSPI_SCLK = HCLK / (CR.CLKDIV + 1)
#define QSPI_CR_CLKDIV_Msk			(0xFFu<< QSPI_CR_CLKDIV_Pos)

#define QSPI_DCR_CLKMOD_Pos			0		//0 Mode 0   1 Mode 3
#define QSPI_DCR_CLKMOD_Msk			(0x01 << QSPI_DCR_CLKMOD_Pos)
#define QSPI_DCR_CSHIGH_Pos			8		//nCS stay high for at least DCR.CSHIGH+1 cycles between Flash memory commands
#define QSPI_DCR_CSHIGH_Msk			(0x07 << QSPI_DCR_CSHIGH_Pos)
#define QSPI_DCR_FLSIZE_Pos			16		//Flash Size = pow(2, DCR.FLSIZE+1)
#define QSPI_DCR_FLSIZE_Msk			(0x1F << QSPI_DCR_FLSIZE_Pos)

#define QSPI_SR_ERR_Pos				0		//Transfer Error Flag
#define QSPI_SR_ERR_Msk				(0x01 << QSPI_SR_ERR_Pos)
#define QSPI_SR_DONE_Pos			1		//Transfer Done/Complete Flag
#define QSPI_SR_DONE_Msk			(0x01 << QSPI_SR_DONE_Pos)
#define QSPI_SR_FFTHR_Pos			2		//FIFO Threshold reached Flag
#define QSPI_SR_FFTHR_Msk			(0x01 << QSPI_SR_FFTHR_Pos)
#define QSPI_SR_PSMAT_Pos			3		//Polling Status Match Flag
#define QSPI_SR_PSMAT_Msk			(0x01 << QSPI_SR_PSMAT_Pos)
#define QSPI_SR_TO_Pos				4		//Time-Out
#define QSPI_SR_TO_Msk				(0x01 << QSPI_SR_TO_Pos)
#define QSPI_SR_BUSY_Pos			5		//Set when operation is on going, Clear when operation done and FIFO emtpy
#define QSPI_SR_BUSY_Msk			(0x01 << QSPI_SR_BUSY_Pos)
#define QSPI_SR_FFLVL_Pos			8		//FIFO Level
#define QSPI_SR_FFLVL_Msk			(0x1F << QSPI_SR_FFLVL_Pos)

#define QSPI_FCR_ERR_Pos			0
#define QSPI_FCR_ERR_Msk			(0x01 << QSPI_FCR_ERR_Pos)
#define QSPI_FCR_DONE_Pos			1
#define QSPI_FCR_DONE_Msk			(0x01 << QSPI_FCR_DONE_Pos)
#define QSPI_FCR_PSMAT_Pos			3
#define QSPI_FCR_PSMAT_Msk			(0x01 << QSPI_FCR_PSMAT_Pos)

#define QSPI_CCR_CODE_Pos			0		//Insruction Code
#define QSPI_CCR_CODE_Msk			(0xFF << QSPI_CCR_CODE_Pos)
#define QSPI_CCR_IMODE_Pos			8		//0 No instruction   1 Instruction on D0   2 on D0-1   3 on D0-3
#define QSPI_CCR_IMODE_Msk			(0x03 << QSPI_CCR_IMODE_Pos)
#define QSPI_CCR_AMODE_Pos			10		//0 No address   1 Address on D0   2 on D0-1   3 on D0-3
#define QSPI_CCR_AMODE_Msk			(0x03 << QSPI_CCR_AMODE_Pos)
#define QSPI_CCR_ASIZE_Pos			12		//Address size, 0 8-bit   1 16-bit   2 24-bit   3 32-bit
#define QSPI_CCR_ASIZE_Msk			(0x03 << QSPI_CCR_ASIZE_Pos)
#define QSPI_CCR_ABMODE_Pos			14		//0 No alternate bytes   1 Alternate bytes on D0   2 on D0-1   3 on D0-3
#define QSPI_CCR_ABMODE_Msk			(0x03 << QSPI_CCR_ABMODE_Pos)
#define QSPI_CCR_ABSIZE_Pos			16		//Alternate bytes size, 0 8-bit   1 16-bit   2 24-bit   3 32-bit
#define QSPI_CCR_ABSIZE_Msk			(0x03 << QSPI_CCR_ABSIZE_Pos)
#define QSPI_CCR_DUMMY_Pos			18		//Number of dummy cycles
#define QSPI_CCR_DUMMY_Msk			(0x1F << QSPI_CCR_DUMMY_Pos)
#define QSPI_CCR_DMODE_Pos			24		//0 No Data   1 Data on D0   2 on D0-1   3 on D0-3
#define QSPI_CCR_DMODE_Msk			(0x03 << QSPI_CCR_DMODE_Pos)
#define QSPI_CCR_MODE_Pos			26		//0 Indirect write mode   1 Indirect read mode   2 Automatic polling mode
#define QSPI_CCR_MODE_Msk			(0x03 << QSPI_CCR_MODE_Pos)
#define QSPI_CCR_SIOO_Pos			28		//Send Instruction Only Once
#define QSPI_CCR_SIOO_Msk			(0x01 << QSPI_CCR_SIOO_Pos)

#define QSPI_SSHIFT_CYCLE_Pos		0		//Sample Shift Cycle Count in System clock
#define QSPI_SSHIFT_CYCLE_Msk		(0x0F << QSPI_SSHIFT_CYCLE_Pos)
#define QSPI_SSHIFT_SPACE_Pos		4		//�� RX FIFO ��ʣ�� SPACE ����λʱ����ǰ��ͣ���գ���ֹ���޷���ʱ��ͣ���µ� FIFO ���
#define QSPI_SSHIFT_SPACE_Msk		(0x0F << QSPI_SSHIFT_SPACE_Pos)




typedef struct {
	__IO uint32_t CR;						//Control Register
	
	__O  uint32_t CMD;						//Command Register
	
	__I  uint32_t SR;						//Status Register
	
	__IO uint32_t IF;						//Interrupt Flag����ȡ����
	
	__IO uint32_t IE;						//Interrupt Enable
	
	__IO uint32_t BT2;
	
	__IO uint32_t BT0;						//Bit Time Register 0
	
	__IO uint32_t BT1;						//Bit Time Register 1
	
	     uint32_t RESERVED;
	
	__IO uint32_t AFM;						//Acceptance Filter Mode
	
	__IO uint32_t AFE;						//Acceptance Filter Enable
	
	__I  uint32_t ALC;						//Arbitration Lost Capture, �ٲö�ʧ��׽
	
	__I  uint32_t ECC;						//Error code capture, ������벶׽
	
	__IO uint32_t EWLIM;					//Error Warning Limit, ���󱨾�����
	
	__IO uint32_t RXERR;					//RX�������
	
	__IO uint32_t TXERR;					//TX�������
	
	struct {
		__IO uint32_t INFO;					//�����ʽ���Buffer��д���ʷ���Buffer
	
		__IO uint32_t DATA[12];
	} FRAME;
	
	__I  uint32_t RMCNT;					//Receive Message Count
	
		 uint32_t RESERVED2[162];
	
	__IO uint32_t ACR[16];					//Acceptance Check Register, ���ռĴ���
	
		 uint32_t RESERVED3[16];
	
	__IO uint32_t AMR[16];					//Acceptance Mask Register, �������μĴ�������Ӧλд0��ID��������ռĴ���ƥ��
} CAN_TypeDef;


#define CAN_CR_RST_Pos				0
#define CAN_CR_RST_Msk				(0x01 << CAN_CR_RST_Pos)
#define CAN_CR_LOM_Pos				1		//Listen Only Mode
#define CAN_CR_LOM_Msk				(0x01 << CAN_CR_LOM_Pos)
#define CAN_CR_STM_Pos				2		//Self Test Mode, ��ģʽ�¼�ʹû��Ӧ��CAN������Ҳ���Գɹ�����
#define CAN_CR_STM_Msk				(0x01 << CAN_CR_STM_Pos)
#define CAN_CR_SLEEP_Pos			4		//д1����˯��ģʽ�������߻���ж�ʱ���Ѳ��Զ������λ
#define CAN_CR_SLEEP_Msk			(0x01 << CAN_CR_SLEEP_Pos)

#define CAN_CMD_TXREQ_Pos			0		//Transmission Request
#define CAN_CMD_TXREQ_Msk			(0x01 << CAN_CMD_TXREQ_Pos)
#define CAN_CMD_ABTTX_Pos			1		//Abort Transmission
#define CAN_CMD_ABTTX_Msk			(0x01 << CAN_CMD_ABTTX_Pos)
#define CAN_CMD_RRB_Pos				2		//Release Receive Buffer
#define CAN_CMD_RRB_Msk				(0x01 << CAN_CMD_RRB_Pos)
#define CAN_CMD_CLROV_Pos			3		//Clear Data Overrun
#define CAN_CMD_CLROV_Msk			(0x01 << CAN_CMD_CLROV_Pos)
#define CAN_CMD_SRR_Pos				4		//Self Reception Request
#define CAN_CMD_SRR_Msk				(0x01 << CAN_CMD_SRR_Pos)

#define CAN_SR_RXDA_Pos				0		//Receive Data Available������FIFO����������Ϣ���Զ�ȡ
#define CAN_SR_RXDA_Msk				(0x01 << CAN_SR_RXDA_Pos)
#define CAN_SR_RXOV_Pos				1		//Receive FIFO Overrun���½��յ���Ϣ���ڽ���FIFO����������
#define CAN_SR_RXOV_Msk				(0x01 << CAN_SR_RXOV_Pos)
#define CAN_SR_TXBR_Pos				2		//Transmit Buffer Release��0 ���ڴ���ǰ��ķ��ͣ����ڲ���д�µ���Ϣ    1 ����д���µ���Ϣ����
#define CAN_SR_TXBR_Msk				(0x01 << CAN_SR_TXBR_Pos)
#define CAN_SR_TXOK_Pos				3		//Transmit OK��successfully completed
#define CAN_SR_TXOK_Msk				(0x01 << CAN_SR_TXOK_Pos)
#define CAN_SR_RXBUSY_Pos			4		//Receive Busy�����ڽ���
#define CAN_SR_RXBUSY_Msk			(0x01 << CAN_SR_RXBUSY_Pos)
#define CAN_SR_TXBUSY_Pos			5		//Transmit Busy�����ڷ���
#define CAN_SR_TXBUSY_Msk			(0x01 << CAN_SR_TXBUSY_Pos)
#define CAN_SR_ERRWARN_Pos			6		//1 ����һ������������ﵽ Warning Limit
#define CAN_SR_ERRWARN_Msk			(0x01 << CAN_SR_ERRWARN_Pos)
#define CAN_SR_BUSOFF_Pos			7		//1 CAN �������������߹ر�״̬��û�в��뵽���߻
#define CAN_SR_BUSOFF_Msk			(0x01 << CAN_SR_BUSOFF_Pos)

#define CAN_IF_RXDA_Pos				0		//IF.RXDA = SR.RXDA & IE.RXDA
#define CAN_IF_RXDA_Msk				(0x01 << CAN_IF_RXDA_Pos)
#define CAN_IF_TXBR_Pos				1		//��IE.TXBR=1ʱ��SR.TXBR��0���1����λ��λ
#define CAN_IF_TXBR_Msk				(0x01 << CAN_IF_TXBR_Pos)
#define CAN_IF_ERRWARN_Pos			2		//��IE.ERRWARN=1ʱ��SR.ERRWARN��SR.BUSOFF 0-to-1 �� 1-to-0����λ��λ
#define CAN_IF_ERRWARN_Msk			(0x01 << CAN_IF_ERRWARN_Pos)
#define CAN_IF_RXOV_Pos				3		//IF.RXOV = SR.RXOV & IE.RXOV
#define CAN_IF_RXOV_Msk				(0x01 << CAN_IF_RXOV_Pos)
#define CAN_IF_WKUP_Pos				4		//��IE.WKUP=1ʱ����˯��ģʽ�µ�CAN��������⵽���߻ʱӲ����λ
#define CAN_IF_WKUP_Msk				(0x01 << CAN_IF_WKUP_Pos)
#define CAN_IF_ERRPASS_Pos			5		//
#define CAN_IF_ERRPASS_Msk			(0x01 << CAN_IF_ERRPASS_Pos)
#define CAN_IF_ARBLOST_Pos			6		//Arbitration Lost����IE.ARBLOST=1ʱ��CAN��������ʧ�ٲñ�ɽ��շ�ʱӲ����λ
#define CAN_IF_ARBLOST_Msk			(0x01 << CAN_IF_ARBLOST_Pos)
#define CAN_IF_BUSERR_Pos			7		//��IE.BUSERR=1ʱ��CAN��������⵽���ߴ���ʱӲ����λ
#define CAN_IF_BUSERR_Msk			(0x01 << CAN_IF_BUSERR_Pos)

#define CAN_IE_RXDA_Pos				0
#define CAN_IE_RXDA_Msk				(0x01 << CAN_IE_RXDA_Pos)
#define CAN_IE_TXBR_Pos				1
#define CAN_IE_TXBR_Msk				(0x01 << CAN_IE_TXBR_Pos)
#define CAN_IE_ERRWARN_Pos			2
#define CAN_IE_ERRWARN_Msk			(0x01 << CAN_IE_ERRWARN_Pos)
#define CAN_IE_RXOV_Pos				3
#define CAN_IE_RXOV_Msk				(0x01 << CAN_IE_RXOV_Pos)
#define CAN_IE_WKUP_Pos				4
#define CAN_IE_WKUP_Msk				(0x01 << CAN_IE_WKUP_Pos)
#define CAN_IE_ERRPASS_Pos			5
#define CAN_IE_ERRPASS_Msk			(0x01 << CAN_IE_ERRPASS_Pos)
#define CAN_IE_ARBLOST_Pos			6
#define CAN_IE_ARBLOST_Msk			(0x01 << CAN_IE_ARBLOST_Pos)
#define CAN_IE_BUSERR_Pos			7
#define CAN_IE_BUSERR_Msk			(0x01 << CAN_IE_BUSERR_Pos)

#define CAN_BT2_BRP_Pos				0
#define CAN_BT2_BRP_Msk				(0x0F << CAN_BT2_BRP_Pos)

#define CAN_BT0_BRP_Pos				0		//Baud Rate Prescaler��CANʱ�䵥λ=2*Tsysclk*((BT2.BRP<<6) + BT0.BRP + 1)
#define CAN_BT0_BRP_Msk				(0x3F << CAN_BT0_BRP_Pos)
#define CAN_BT0_SJW_Pos				6		//Synchronization Jump Width
#define CAN_BT0_SJW_Msk				(0x03 << CAN_BT0_SJW_Pos)

#define CAN_BT1_TSEG1_Pos			0		//t_tseg1 = CANʱ�䵥λ * (TSEG1+1)
#define CAN_BT1_TSEG1_Msk			(0x0F << CAN_BT1_TSEG1_Pos)
#define CAN_BT1_TSEG2_Pos			4		//t_tseg2 = CANʱ�䵥λ * (TSEG2+1)
#define CAN_BT1_TSEG2_Msk			(0x07 << CAN_BT1_TSEG2_Pos)
#define CAN_BT1_SAM_Pos				7		//��������  0: sampled once  1: sampled three times
#define CAN_BT1_SAM_Msk				(0x01 << CAN_BT1_SAM_Pos)

#define CAN_ECC_SEGCODE_Pos			0		//Segment Code
#define CAN_ECC_SEGCODE_Msk			(0x1F << CAN_ECC_SEGCODE_Pos)
#define CAN_ECC_DIR_Pos				5		//0 error occurred during transmission   1 during reception
#define CAN_ECC_DIR_Msk				(0x01 << CAN_ECC_DIR_Pos)
#define CAN_ECC_ERRCODE_Pos			6		//Error Code��0 Bit error   1 Form error   2 Stuff error   3 other error
#define CAN_ECC_ERRCODE_Msk			(0x03 << CAN_ECC_ERRCODE_Pos)

#define CAN_INFO_DLC_Pos			0		//Data Length Control
#define CAN_INFO_DLC_Msk			(0x0F << CAN_INFO_DLC_Pos)
#define CAN_INFO_RTR_Pos			6		//Remote Frame��1 Զ��֡    0 ����֡
#define CAN_INFO_RTR_Msk			(0x01 << CAN_INFO_RTR_Pos)
#define CAN_INFO_FF_Pos				7		//Frame Format��0 ��׼֡��ʽ    1 ��չ֡��ʽ
#define CAN_INFO_FF_Msk				(0x01 << CAN_INFO_FF_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t SR;
	
	__IO uint32_t IE;						//[0] ��������ж�ʹ��
	
	     uint32_t RESERVED;
	
	__IO uint32_t DIVIDEND;					//������
	
	__IO uint32_t DIVISOR;					//����
	
	__IO uint32_t QUO;						//��
	
	__IO uint32_t REMAIN;					//����
	
	__IO uint32_t RADICAND;					//��������
	
	__IO uint32_t ROOT;						//ƽ��������16λΪС�����֣���16λΪ��������
} DIV_TypeDef;


#define DIV_CR_DIVGO_Pos			0		//д1�����������㣬������ɺ��Զ�����
#define DIV_CR_DIVGO_Msk			(0x01 << DIV_CR_DIVGO_Pos)
#define DIV_CR_DIVSIGN_Pos			1		//0 �з��ų���    1 �޷��ų���
#define DIV_CR_DIVSIGN_Msk			(0x01 << DIV_CR_DIVSIGN_Pos)
#define DIV_CR_ROOTGO_Pos			8		//д1������ƽ�������㣬������ɺ��Զ�����
#define DIV_CR_ROOTGO_Msk			(0x01 << DIV_CR_ROOTGO_Pos)
#define DIV_CR_ROOTMOD_Pos			9		//��ƽ����ģʽ��0 ���Ϊ����    1 �������������������С������
#define DIV_CR_ROOTMOD_Msk			(0x01 << DIV_CR_ROOTMOD_Pos)

#define DIV_SR_DIVEND_Pos			0		//����������ɱ�־��д1����
#define DIV_SR_DIVEND_Msk			(0x01 << DIV_SR_DIVEND_Pos)
#define DIV_SR_DIVBUSY_Pos			1		//1 �������������
#define DIV_SR_DIVBUSY_Msk			(0x01 << DIV_SR_DIVBUSY_Pos)
#define DIV_SR_ROOTENDI_Pos			8		//��������������ɱ�־��д1����
#define DIV_SR_ROOTENDI_Msk			(0x01 << DIV_SR_ROOTENDI_Pos)
#define DIV_SR_ROOTENDF_Pos			9		//����С��������ɱ�־��д1����
#define DIV_SR_ROOTENDF_Msk			(0x01 << DIV_SR_ROOTENDF_Pos)
#define DIV_SR_ROOTBUSY_Pos			10		//1 �������������
#define DIV_SR_ROOTBUSY_Msk			(0x01 << DIV_SR_ROOTBUSY_Pos)




typedef struct {
    __IO uint32_t CR;
    
	__O  uint32_t DATAIN;
	
    __IO uint32_t INIVAL;					//CR.ENд1ʱ��INIVAL�е�ֵд��RESULT
    
    __I  uint32_t RESULT;
} CRC_TypeDef;


#define CRC_CR_EN_Pos			    0
#define CRC_CR_EN_Msk			    (0x01 << CRC_CR_EN_Pos)
#define CRC_CR_IREV_Pos				1       //���������Ƿ�ת��0 bit˳�򲻱�    1 bit˳����ȫ��ת   2 bit˳���ֽ��ڷ�ת   3 ���ֽ�˳��ת
#define CRC_CR_IREV_Msk				(0x03 << CRC_CR_IREV_Pos)
#define CRC_CR_INOT_Pos				3       //���������Ƿ�ȡ��
#define CRC_CR_INOT_Msk				(0x01 << CRC_CR_INOT_Pos)
#define CRC_CR_OREV_Pos				4       //�������Ƿ�ת��0 bit˳�򲻱�    1 bit˳����ȫ��ת   2 bit˳���ֽ��ڷ�ת   3 ���ֽ�˳��ת
#define CRC_CR_OREV_Msk				(0x03 << CRC_CR_OREV_Pos)
#define CRC_CR_ONOT_Pos				6       //�������Ƿ�ȡ��
#define CRC_CR_ONOT_Msk				(0x01 << CRC_CR_ONOT_Pos)
#define CRC_CR_POLY_Pos				7       //����ʽѡ��0 x^16+x^12+x^5+1   1 x^8+x^2+x+1   2 x^16+x^15+x^2+1   3 x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1
#define CRC_CR_POLY_Msk				(0x03 << CRC_CR_POLY_Pos)
#define CRC_CR_IBIT_Pos				9       //����������Чλ�� 0 32λ    1 16λ    2 8λ
#define CRC_CR_IBIT_Msk				(0x03 << CRC_CR_IBIT_Pos)




typedef struct {
		 uint32_t RESERVED[2];
	
	__IO uint32_t SR;
	
	__IO uint32_t CR;
	
	union {
		__IO uint8_t  IRB;
		
		__IO uint16_t IRH;
		
		__IO uint32_t RESERVED2;
	};
	
	union {
		__IO uint8_t  DRB;
		
		__IO uint16_t DRH;
		
		__IO uint32_t RESERVED3;
	};
} MPU_TypeDef;


#define MPU_SR_BUSY_Pos				0
#define MPU_SR_BUSY_Msk				(0x01 << MPU_SR_BUSY_Pos)
#define MPU_SR_DMAEN_Pos			1
#define MPU_SR_DMAEN_Msk			(0x01 << MPU_SR_DMAEN_Pos)
#define MPU_SR_ENDIAN_Pos			2		//0 С��   1 ���
#define MPU_SR_ENDIAN_Msk			(0x01 << MPU_SR_ENDIAN_Pos)

#define MPU_CR_RCS1_0_Pos			0		//������ʱ��CS�����ص��½���ʱ������0  1��ʱ������
#define MPU_CR_RCS1_0_Msk			(0x1F << MPU_CR_RCS1_0_Pos)
#define MPU_CR_RDHOLD_Pos			5		//RD�͵�ƽ����ʱ��
#define MPU_CR_RDHOLD_Msk			(0x1F << MPU_CR_RDHOLD_Pos)
#define MPU_CR_WCS1_0_Pos			10		//д����ʱ��CS�����ص��½���ʱ����
#define MPU_CR_WCS1_0_Msk			(0x0F << MPU_CR_WCS1_0_Pos)
#define MPU_CR_WR1CS1_Pos			14		//WR�����ص�CS��������ʱ
#define MPU_CR_WR1CS1_Msk			(0x03 << MPU_CR_WR1CS1_Pos)
#define MPU_CR_WRHOLD_Pos			16		//WR�͵�ƽ����ʱ��
#define MPU_CR_WRHOLD_Msk			(0x0F << MPU_CR_WRHOLD_Pos)
#define MPU_CR_CS0WR0_Pos			20		//CS�½��ص�WR�½�����ʱ
#define MPU_CR_CS0WR0_Msk			(0x03 << MPU_CR_CS0WR0_Pos)




typedef struct {
	__IO uint32_t DATA;
	
	__IO uint32_t ADDR;
	
	__IO uint32_t ERASE;
	
	__IO uint32_t CACHE;
	
	__IO uint32_t CFG0;
	
	__IO uint32_t CFG1;						//����д�� 0x5A5A5A5A��0xA5A5A5A5 ��� readonly��д����������ֵ�ָ� readonly
	
	__IO uint32_t CFG2;
	
	__IO uint32_t CFG3;
	
	__IO uint32_t CFG4;
	
	__IO uint32_t STAT;
		
	__IO uint32_t REMAP;
} FMC_TypeDef;


#define FMC_ERASE_ADDR_Pos			0		//512 Byte per Page
#define FMC_ERASE_ADDR_Msk			(0x1FFFF<< FMC_ERASE_ADDR_Pos)
#define FMC_ERASE_REQ_Pos			24
#define FMC_ERASE_REQ_Msk			(0xFFu<< FMC_ERASE_REQ_Pos)

#define FMC_CACHE_CEN_Pos			0		//Cache Enable
#define FMC_CACHE_CEN_Msk			(0x01 << FMC_CACHE_CEN_Pos)
#define FMC_CACHE_CPEN_Pos			1		//Cache Predict Enable
#define FMC_CACHE_CPEN_Msk			(0x01 << FMC_CACHE_CPEN_Pos)
#define FMC_CACHE_CCLR_Pos			31		//Cache Clear���Զ�����
#define FMC_CACHE_CCLR_Msk			(0x01u<< FMC_CACHE_CCLR_Pos)

#define FMC_CFG0_WREN_Pos			9
#define FMC_CFG0_WREN_Msk			(0x01 << FMC_CFG0_WREN_Pos)

#define FMC_STAT_ERASEBUSY_Pos		0
#define FMC_STAT_ERASEBUSY_Msk		(0x01 << FMC_STAT_ERASEBUSY_Pos)
#define FMC_STAT_PROGBUSY_Pos		1
#define FMC_STAT_PROGBUSY_Msk		(0x01 << FMC_STAT_PROGBUSY_Pos)
#define FMC_STAT_READBUSY_Pos		2
#define FMC_STAT_READBUSY_Msk		(0x01 << FMC_STAT_READBUSY_Pos)
#define FMC_STAT_FIFOEMPTY_Pos		3		//Write FIFO Empty
#define FMC_STAT_FIFOEMPTY_Msk		(0x01 << FMC_STAT_FIFOEMPTY_Pos)
#define FMC_STAT_FIFOFULL_Pos		4		//Write FIFO Full
#define FMC_STAT_FIFOFULL_Msk		(0x01 << FMC_STAT_FIFOFULL_Pos)
#define FMC_STAT_READONLY_Pos		7
#define FMC_STAT_READONLY_Msk		(0x01 << FMC_STAT_READONLY_Pos)
#define FMC_STAT_INITDONE_Pos		30
#define FMC_STAT_INITDONE_Msk		(0x01 << FMC_STAT_INITDONE_Pos)
#define FMC_STAT_IDLE_Pos			31
#define FMC_STAT_IDLE_Msk			(0x01u<< FMC_STAT_IDLE_Pos)

#define FMC_REMAP_ON_Pos			0
#define FMC_REMAP_ON_Msk			(0x01 << FMC_REMAP_ON_Pos)
#define FMC_REMAP_OFFSET_Pos		1		//��0x000-0x800��2K��ַ�ķ���ӳ�䵽2K*OFFSET-2K*(OFFSET+1)��ַ��
#define FMC_REMAP_OFFSET_Msk		(0x3F << FMC_REMAP_OFFSET_Pos)




typedef struct {
	__IO uint32_t RSTVAL;					//��������������ֵʱ������λ
	
	__IO uint32_t INTVAL;					//��������������ֵʱ�����ж�
	
	__IO uint32_t CR;
	
	__IO uint32_t IF;						//[0] �жϱ�־��д1����
	
	__IO uint32_t FEED;						//д0x55ι��
} WDT_TypeDef;


#define WDT_CR_EN_Pos				0
#define WDT_CR_EN_Msk				(0x01 << WDT_CR_EN_Pos)
#define WDT_CR_RSTEN_Pos			1
#define WDT_CR_RSTEN_Msk			(0x01 << WDT_CR_RSTEN_Pos)
#define WDT_CR_INTEN_Pos			2
#define WDT_CR_INTEN_Msk			(0x01 << WDT_CR_INTEN_Pos)
#define WDT_CR_WINEN_Pos			3		//Window function enable
#define WDT_CR_WINEN_Msk			(0x01 << WDT_CR_WINEN_Pos)
#define WDT_CR_CLKDIV_Pos			8		//WDT����ʱ�ӷ�Ƶֵ = pow(2, CLKDIV+1)
#define WDT_CR_CLKDIV_Msk			(0x0F << WDT_CR_CLKDIV_Pos)




/******************************************************************************/
/*						 Peripheral memory map							  */
/******************************************************************************/
#define RAM_BASE		   	0x20000000
#define AHB_BASE			0x40000000
#define APB1_BASE		 	0x40040000
#define APB2_BASE			0x400A0000


/* AHB Peripheral memory map */
#define SYS_BASE			(AHB_BASE + 0x00000)

#define DMA_BASE			(AHB_BASE + 0x00800)

#define QSPI0_BASE			(AHB_BASE + 0x01800)

#define CRC_BASE			(AHB_BASE + 0x02800)

#define DIV_BASE			(AHB_BASE + 0x03000)

#define GPIOA_BASE			(AHB_BASE + 0x03800)
#define GPIOB_BASE			(AHB_BASE + 0x04000)
#define GPIOC_BASE			(AHB_BASE + 0x04800)

#define MPU_BASE			(AHB_BASE + 0x05000)


/* APB1 Peripheral memory map */
#define UART0_BASE			(APB1_BASE + 0x0000)
#define UART1_BASE			(APB1_BASE + 0x0800)

#define QEI_BASE			(APB1_BASE + 0x1000)

#define SPI0_BASE			(APB1_BASE + 0x1800)

#define I2C0_BASE			(APB1_BASE + 0x2000)

#define CAN0_BASE			(APB1_BASE + 0x2800)

#define PWM0_BASE			(APB1_BASE + 0x3000)
#define PWM1_BASE			(APB1_BASE + 0x3080)
#define PWMG_BASE			(APB1_BASE + 0x3400)

#define TIMR0_BASE			(APB1_BASE + 0x3800)
#define TIMR1_BASE			(APB1_BASE + 0x3840)
#define TIMR2_BASE			(APB1_BASE + 0x3880)
#define TIMRG_BASE			(APB1_BASE + 0x3C00)

#define BTIMR0_BASE			(APB1_BASE + 0x4000)
#define BTIMR1_BASE			(APB1_BASE + 0x4040)
#define BTIMR2_BASE			(APB1_BASE + 0x4080)
#define BTIMR3_BASE			(APB1_BASE + 0x40C0)
#define BTIMRG_BASE			(APB1_BASE + 0x4400)

#define ADC0_BASE			(APB1_BASE + 0x4800)
#define ADC1_BASE			(APB1_BASE + 0x4900)

#define FMC_BASE			(APB1_BASE + 0x5000)		//Flash Memory Controller

#define PORTA_BASE			(APB1_BASE + 0x6000)
#define PORTB_BASE			(APB1_BASE + 0x6010)
#define PORTC_BASE			(APB1_BASE + 0x6020)

#define WDT_BASE			(APB1_BASE + 0x6800)

#define USART0_BASE			(APB1_BASE + 0x7000)


/******************************************************************************/
/*						 Peripheral declaration							 */
/******************************************************************************/
#define SYS					((SYS_TypeDef  *) SYS_BASE)

#define PORTA				((PORT_TypeDef *) PORTA_BASE)
#define PORTB				((PORT_TypeDef *) PORTB_BASE)
#define PORTC				((PORT_TypeDef *) PORTC_BASE)

#define GPIOA				((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB				((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC				((GPIO_TypeDef *) GPIOC_BASE)

#define TIMR0				((TIMR_TypeDef *) TIMR0_BASE)
#define TIMR1				((TIMR_TypeDef *) TIMR1_BASE)
#define TIMR2				((TIMR_TypeDef *) TIMR2_BASE)
#define TIMRG				((TIMRG_TypeDef*) TIMRG_BASE)

#define BTIMR0				((TIMR_TypeDef *) BTIMR0_BASE)
#define BTIMR1				((TIMR_TypeDef *) BTIMR1_BASE)
#define BTIMR2				((TIMR_TypeDef *) BTIMR2_BASE)
#define BTIMR3				((TIMR_TypeDef *) BTIMR3_BASE)
#define BTIMRG				((TIMRG_TypeDef*) BTIMRG_BASE)

#define UART0				((UART_TypeDef *) UART0_BASE)
#define UART1				((UART_TypeDef *) UART1_BASE)

#define USART0				((USART_TypeDef *)USART0_BASE)

#define QSPI0				((QSPI_TypeDef *) QSPI0_BASE)

#define SPI0				((SPI_TypeDef  *) SPI0_BASE)

#define I2C0				((I2C_TypeDef  *) I2C0_BASE)

#define CAN0				((CAN_TypeDef  *) CAN0_BASE)

#define ADC0 				((ADC_TypeDef  *) ADC0_BASE)
#define ADC1				((ADC_TypeDef  *) ADC1_BASE)

#define PWM0				((PWM_TypeDef  *) PWM0_BASE)
#define PWM1				((PWM_TypeDef  *) PWM1_BASE)
#define PWMG				((PWMG_TypeDef *) PWMG_BASE)

#define DIV					((DIV_TypeDef  *) DIV_BASE)

#define CRC					((CRC_TypeDef  *) CRC_BASE)

#define DMA					((DMA_TypeDef  *) DMA_BASE)

#define MPU					((MPU_TypeDef  *) MPU_BASE)

#define FMC					((FMC_TypeDef  *) FMC_BASE)

#define WDT					((WDT_TypeDef  *) WDT_BASE)

#define QEI					((QEI_TypeDef  *) QEI_BASE)


#include "SWM221_port.h"
#include "SWM221_gpio.h"
#include "SWM221_exti.h"
#include "SWM221_timr.h"
#include "SWM221_uart.h"
#include "SWM221_spi.h"
#include "SWM221_i2c.h"
#include "SWM221_pwm.h"
#include "SWM221_adc.h"
#include "SWM221_dma.h"
#include "SWM221_mpu.h"
#include "SWM221_can.h"
#include "SWM221_div.h"
#include "SWM221_crc.h"
#include "SWM221_wdt.h"
#include "SWM221_qei.h"
#include "SWM221_qspi.h"
#include "SWM221_usart.h"
#include "SWM221_flash.h"
#include "SWM221_sleep.h"
#include "SWM221_iofilt.h"



#ifdef  SW_LOG_RTT
#define log_printf(...)	 	SEGGER_RTT_printf(0, __VA_ARGS__)
#else
#define log_printf(...)	 	printf(__VA_ARGS__)
#endif


#ifndef SW_LOG_LEVEL
#define SW_LOG_LEVEL        0
#endif

#if (SW_LOG_LEVEL > 0)
#define SW_LOG_ERR(...)    	{						 \
							log_printf("ERROR: ");   \
							log_printf(__VA_ARGS__); \
							log_printf("\n");		 \
							}

#if (SW_LOG_LEVEL > 1)
#define SW_LOG_WARN(...) 	{						 \
							log_printf("WARN : ");   \
							log_printf(__VA_ARGS__); \
							log_printf("\n");		 \
							}

#if (SW_LOG_LEVEL > 2)
#define SW_LOG_INFO(...)   	{						 \
							log_printf("INFO : ");   \
							log_printf(__VA_ARGS__); \
							log_printf("\n");		 \
							}
#else
#define SW_LOG_INFO(...)
#endif

#else
#define SW_LOG_WARN(...)
#define SW_LOG_INFO(...)
#endif

#else
#define SW_LOG_ERR(...)
#define SW_LOG_WARN(...)
#define SW_LOG_INFO(...)
#endif


#endif //__SWM221_H__
