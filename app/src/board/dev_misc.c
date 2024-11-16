/**
 *******************************************************************************************************************************************
 * @file        dev_misc.c
 * @brief       低电压 / SWD 端口复用 / 外部晶振停振 检测 / boot 跳转 app
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-04   lzh          the first version
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include <stdio.h>
#include "SWM221.h"
#include "dev_misc.h"

/**
 * @brief  欠压检测配置
 * @note   该功能默认为常开状态, 上电后默认配置 1.65V 产生复位, 失能中断.
 * @param  rst_mv : 低电压复位阈值
 * @param  int_mv : 低电压触发阈值
 * @param  int_en : 0-失能低电压触发中断    1-使能低电压触发中断
 * @retval /
 */
void bod_config(uint8_t rst_mv, uint8_t int_mv, uint8_t int_en)
{
	SYS->PVDCR = (1 << SYS_PVDCR_EN_Pos) |
				 (1 << SYS_PVDCR_IE_Pos) |
				 (int_mv << SYS_PVDCR_LVL_Pos);
	
	SYS->LVRCR = (1 << SYS_LVRCR_EN_Pos)  |
				 (rst_mv << SYS_LVRCR_LVL_Pos) |
				 (1 << SYS_LVRCR_WEN_Pos);
    if (int_en)
    {
        SYS->PVDSR |= (1 << SYS_PVDSR_IF_Pos);
        NVIC_EnableIRQ(GPIOA3_GPIOC3_PVD_IRQn);
    }
}

//__WEAK 
void GPIOA3_GPIOC3_PVD_Handler(void)
{
    SYS->PVDSR = (1 << SYS_PVDSR_IF_Pos); //清除中断标志
    
    while (1) __NOP(); //eg: WDT RST
}

/**
 * @brief  配置 SWD 端口的功能
 * @note   SWD 端口功能切换未在固件库中开放给用户, 避免用户配置端口错误而导致无法下载, 重新上电后, SWD 端口总会自动恢复为 SWD 功能.
 * 1、SWD 端口切换为 GPIO 或 其它非 SWD 功能前必须加上 *((volatile uint32_t *)0x40000190) = 0; 
 * 2、SWD 端口恢复为 SWD 功能前必须加上 *((volatile uint32_t *)0x40000190) = 1; 
 * @param  fun_mux: 0- SWD 功能      1-除 SWD 外的功能
 * @return /
 */
void swd_port_config(uint8_t fun_mux)
{
    if (0 == fun_mux) {
        *((volatile uint32_t *)0x40000190) = 1;
        PORT_Init(PORTC, PIN1, PORTC_PIN1_SWDIO, 1);
        PORT_Init(PORTC, PIN0, PORTC_PIN0_SWCLK, 1);
    } else {
        *((volatile uint32_t *)0x40000190) = 0;
        /* 可外部覆盖 */
        PORT_Init(PORTC, PIN1, PORTC_PIN1_GPIO, 1);
        PORT_Init(PORTC, PIN0, PORTC_PIN0_GPIO, 1);
    }
}

/**
 * @brief 外部晶振停振检测初始化
 */
void xtal_stop_check_init(void)
{
    // 等待晶振稳定，防止上电时误识别晶振停振
    //for (uint32_t i = 0; i < SystemCoreClock / 4; i++) __NOP();
    SYS->XTALSR = SYS_XTALSR_STOP_Msk; // 清除标志
    NVIC_ClearPendingIRQ(XTALSTOP_IRQn);
    NVIC_EnableIRQ(XTALSTOP_IRQn);
}

//__WEAK 
void XTALSTOP_Handler(void)
{
    /* 若不执行 switchTo8MHz(), 晶振恢复振荡时系统时钟会自动切换回外部晶振, 
     * 若外部晶振不稳定, 在振荡和不振荡间来回切换, 则系统时钟也会在内部时钟和外部时钟之间来回切换.
     */
    switchToHRC();
    SystemCoreClockUpdate();

    SYS->XTALCR = 0; // 外部晶振工作不正常, 关闭
    NVIC_DisableIRQ(XTALSTOP_IRQn);
    SYS->XTALSR = SYS_XTALSR_STOP_Msk; // 清除标志
    NVIC_ClearPendingIRQ(XTALSTOP_IRQn);

#if 1 // 系统主频在程序运行时动态发生变化, 查看时钟树中除了拥有独立时钟源的外设, 其他外设均需重新配置一遍以更新设定工作频率.
    UART_Close(UART0);
    UART_SetBaudrate(UART0, 57600);
    UART_Open(UART0);
    printf("[%s]: XTAL Stop Detected!\r\n", __FUNCTION__);
#endif
}

__NO_RETURN void jump_to_app(uint32_t addr)
{
	/* 跳转到APP前需要将UserBoot使用的外设关掉（复位）*/
	__disable_irq();
	
	SYS->PRSTEN = 0x55;
	SYS->PRSTR0 = 0xFFFFFFFF & (~SYS_PRSTR0_ANAC_Msk);
	for(uint32_t i = 0; i < CyclesPerUs; ++i) __NOP();
	SYS->PRSTR0 = 0;
	SYS->PRSTEN = 0;
    
    /* Disable SysTick IRQ and SysTick Timer */
    SysTick->CTRL = 0;
    /* pending SysTick exception */
    if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)
    {
        SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk; /* clear pending bit */
    }
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

	NVIC->ICER[0] = 0xFFFFFFFF;
    

	static volatile uint32_t sp;
	static volatile uint32_t pc;
    
    sp = *((volatile uint32_t *)(addr));
    pc = *((volatile uint32_t *)(addr + 4));
	
	__set_MSP(sp);
	
    typedef void (*ResetHandler)(void);
	((ResetHandler)pc)();

    for(;;) /* wait until reset, no return */
    {
        __NOP();
    }
}

__attribute__((used, section(".SRAM")))
void Flash_remap(uint32_t addr)
{
	/* 只能在APP中REMAP，在UserBoot中REMAP可能会导致访问UserBoot的代码被重定向到APP的代码 */
	FMC->REMAP = (1 << FMC_REMAP_ON_Pos) | ((addr / 2048) << FMC_REMAP_OFFSET_Pos);
	FMC->CACHE |= FMC_CACHE_CCLR_Msk;
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__enable_irq();
}
