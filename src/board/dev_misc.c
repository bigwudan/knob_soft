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
	SYS->BODCR = (1 << SYS_BODCR_EN_Pos) |
				 (1 << SYS_BODCR_IE_Pos) |
				 (int_mv << SYS_BODCR_LVL_Pos);
	
	SYS->LVRCR = (1 << SYS_LVRCR_EN_Pos)  |
				 (rst_mv << SYS_LVRCR_LVL_Pos) |
				 (1 << SYS_LVRCR_WEN_Pos);
    if (int_en)
    {
        SYS->BODSR |= (1 << SYS_BODSR_IF_Pos);
        NVIC_EnableIRQ(GPIOA3_GPIOC3_BOD_IRQn);
    }
}

//__WEAK 
void GPIOA3_GPIOC3_BOD_Handler(void)
{
    SYS->BODSR = (1 << SYS_BODSR_IF_Pos); //清除中断标志
    
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

/* 注意事项：jumpApp.c 需要定位到 Data RAM, 方法如下：(两种方法任取其一)
 * 1、在 jumpApp.c 上右键选 “Options for File 'jumpApp.c'”, 在“Properties”选项页中“Code/Const”后下拉框选“IRAM1 [0x20000000-0x20000FFF]”, 点“OK”确定.
 * 2、修改 scatter file 设定 jump_to_app 至 RAM section 内.
 * ARM cortex-M0 没有 SCB->VTOR 寄存器.
 */
__attribute__((section(".SRAM")))
__NO_RETURN void jump_to_app(void)
{
    //本函数必须在RAM中执行，因为其中会REMAP操作，代码所在Flash可能会被重映射到其他地址
    
    #define EEPROM_ADDR      (0x800 * 4)
	/* 将 EEPROM_ADDR 处的 Flash 映射到 0 地址, 后从 0 地址读取数据 */
	FMC->REMAP = (1 << FMC_REMAP_ON_Pos) | ((EEPROM_ADDR / 0x800) << FMC_REMAP_OFFSET_Pos);
	FMC->CACHE |= (1 << FMC_CACHE_CCLR_Pos);

//	for (uint32_t i = 0; i < 20; i++) RdBuff[i] = ((volatile uint32_t *)0x00)[i];
	FMC->REMAP = (0 << FMC_REMAP_ON_Pos);
	FMC->CACHE |= FMC_CACHE_CCLR_Msk;

//	for (uint32_t i = 0; i < 20; i++) printf("0x%08X, ", RdBuff[i]);

#if 1
    NVIC_SystemReset();
#else
    __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */
    SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                 SCB_AIRCR_SYSRESETREQ_Msk);
    __DSB();                                                          /* Ensure completion of memory access */
#endif

    for(;;) /* wait until reset, no return */
    {
        __NOP();
    }
}

