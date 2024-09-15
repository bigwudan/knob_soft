/******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file ev_hc32f460_lqfp100_v2.c
 **
 ** A detailed description is available at
 ** @link EV_HC32F460_LQFP100_V2 EV_HC32F460_LQFP100_V2 description @endlink
 **
 **   - 2020-02-22  CDT  First version for Device Driver Library of BSP.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ev_hc32f460_lqfp100_v2.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @defgroup EV_HC32F460_LQFP100_V2 EV_HC32F460_LQFP100_V2
 * @{
 */

#if (BSP_EV_HC32F460_LQFP100_V2 == BSP_EV_HC32F460)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief BSP Port and pin configuration structure
 ******************************************************************************/
typedef struct
{
    en_port_t port;
    en_pin_t  pin;
} BSP_Port_Pin;

/**
 *******************************************************************************
 ** \brief BSP key in configuration structure
 ******************************************************************************/
typedef struct
{
    en_port_t    port;
    en_pin_t     pin;
    en_exti_ch_t ch;
    en_int_src_t int_src;
    IRQn_Type    irq;
    func_ptr_t   callback;
} BSP_KeyIn_Config;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static const BSP_Port_Pin BSP_LED_PORT_PIN[BSP_LED_NUM] = {
    {BSP_LED_RED_PORT,      BSP_LED_RED_PIN},
    {BSP_LED_GREEN_PORT,    BSP_LED_GREEN_PIN},
    {BSP_LED_BLUE_PORT,     BSP_LED_BLUE_PIN}};

uint8_t u8GlobalKey;		
		
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F460_LQFP100_V2_Global_Functions EV_HC32F460_LQFP100_V2 Global Functions
 * @{
 */

/**
 * @brief  BSP clock initialize.
 *         Set board system clock to MPLL@200MHz
 * @param  None
 * @retval None
 */
void BSP_CLK_Init(void)
{
    stc_clk_sysclk_cfg_t    stcSysClkCfg;
    stc_clk_xtal_cfg_t      stcXtalCfg;
    stc_clk_mpll_cfg_t      stcMpllCfg;
    stc_sram_config_t       stcSramConfig;

    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);
    MEM_ZERO_STRUCT(stcSramConfig);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv  = ClkSysclkDiv1;
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Config Xtal and Enable Xtal */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* sram init include read/write wait cycle setting */
    stcSramConfig.u8SramIdx = Sram12Idx | Sram3Idx | SramHsIdx | SramRetIdx;
    stcSramConfig.enSramRC = SramCycle2;
    stcSramConfig.enSramWC = SramCycle2;
    SRAM_Init(&stcSramConfig);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_5);
    EFM_Lock();

    /* MPLL config (XTAL / pllmDiv * plln / PllpDiv = 200M). */
    stcMpllCfg.pllmDiv = 1ul;
    stcMpllCfg.plln    = 50ul;
    stcMpllCfg.PllpDiv = 2ul;
    stcMpllCfg.PllqDiv = 2ul;
    stcMpllCfg.PllrDiv = 2ul;
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);
    /* Wait MPLL ready. */
    while(Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
        ;
    }
    /* Switch driver ability */
    PWC_HS2HP();
    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);
}

/**
 *******************************************************************************
 ** \brief ExtInt13 as key callback function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Key_Callback(void)
{
    if (Set == EXINT_IrqFlgGet(ExtiCh13))
    {
//				BSP_LED_Toggle(LED_GREEN);
			
        while (Reset == PORT_GetBit(KEYIN_PORT, KEYIN0_PIN))
        {
                u8GlobalKey = 1;
        }			
//				u8GlobalKey = 0;
        /* clear int request flag */
        EXINT_IrqFlgClr(ExtiCh13);
    }
}


/**
 *******************************************************************************
 ** \brief BSP KEY init function
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void BSP_KEY_Init(void)
{
    stc_exint_config_t stcExtiConfig;
    stc_irq_regi_conf_t stcIrqRegiConf;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcExtiConfig);
    MEM_ZERO_STRUCT(stcIrqRegiConf);
    MEM_ZERO_STRUCT(stcPortInit);

    /**************************************************************************/
    /* External Int Ch.13 for keyin1                                          */
    /**************************************************************************/
    stcExtiConfig.enExitCh = ExtiCh13;

    /* Filter setting */
    stcExtiConfig.enFilterEn = Enable;
    stcExtiConfig.enFltClk = Pclk3Div8;
    /* falling edge for keyscan function */
    stcExtiConfig.enExtiLvl = ExIntFallingEdge;
    EXINT_Init(&stcExtiConfig);

    /* Set PC13 as External Int Ch.13 input */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enExInt = Enable;
    stcPortInit.enPullUp = Enable;
    PORT_Init(KEYIN_PORT, KEYIN0_PIN, &stcPortInit);

    /* Select External Int Ch.13 */
    stcIrqRegiConf.enIntSrc = INT_PORT_EIRQ13;

    /* Register External Int to Vect.No.000 */
    stcIrqRegiConf.enIRQn = Int000_IRQn;

    /* Callback function */
    stcIrqRegiConf.pfnCallback = &Key_Callback;

    /* Registration IRQ */
    enIrqRegistration(&stcIrqRegiConf);

    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);

    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);

    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
		
    KEYSCAN_Start();		
}

/**
 * @brief  LED initialize.
 * @param  None
 * @retval none
 */
void BSP_LED_Init(void)
{
    uint8_t i;
    stc_port_init_t stcPortInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = Pin_Mode_Out;
    /* Initialize LED pin */
    for (i = 0U; i < BSP_LED_NUM; i++)
    {
        PORT_Init(BSP_LED_PORT_PIN[i].port, BSP_LED_PORT_PIN[i].pin, &stcPortInit);
    }
		BSP_LED_Off(LED_RED);
		BSP_LED_Off(LED_GREEN);
		BSP_LED_Off(LED_BLUE);		
}

/**
 * @brief  Turn on LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_GREEN
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval none
 */
void BSP_LED_On(uint8_t u8Led)
{
    uint8_t i;

    for (i = 0U; i < BSP_LED_NUM; i++)
    {
        if (0U != ((u8Led >> i) & 1U))
        {
            PORT_ResetBits(BSP_LED_PORT_PIN[i].port, BSP_LED_PORT_PIN[i].pin);
        }
    }
}

/**
 * @brief  Turn off LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_GREEN
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval none
 */
void BSP_LED_Off(uint8_t u8Led)
{
    uint8_t i;

    for (i = 0U; i < BSP_LED_NUM; i++)
    {
        if (0U != ((u8Led >> i) & 1U))
        {
            PORT_SetBits(BSP_LED_PORT_PIN[i].port, BSP_LED_PORT_PIN[i].pin);
        }
    }
}

/**
 * @brief  Toggle LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_GREEN
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval none
 */
void BSP_LED_Toggle(uint8_t u8Led)
{
    uint8_t i;

    for (i = 0U; i < BSP_LED_NUM; i++)
    {
        if (0U != ((u8Led >> i) & 1U))
        {
            PORT_Toggle(BSP_LED_PORT_PIN[i].port, BSP_LED_PORT_PIN[i].pin);
        }
    }
}

/**
 * @brief  BSP printf port initialize
 * @param  None
 * @retval None
 */
void BSP_PRINTF_PortInit(void)
{
    PORT_SetFunc(BSP_PRINTF_PORT, BSP_PRINTF_PIN, BSP_PRINTF_PORT_FUNC, Disable);
}

/**
 * @}
 */


/**
 * @}
 */

#endif /* BSP_EV_HC32F460_LQFP100_V2 */

/**
 * @}
 */

/**
 * @}
 */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
