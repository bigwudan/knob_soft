/*****************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file ev_hc32f460_lqfp100_v2.h
 **
 ** A detailed description is available at
 ** @link EV_HC32F460_LQFP100_V2 EV_HC32F460_LQFP100_V2 description @endlink
 **
 **   - 2020-02-22  CDT  First version for Device Driver Library of BSP.
 **
 ******************************************************************************/
#ifndef __EV_HC32F460_LQFP100_V2_H__
#define __EV_HC32F460_LQFP100_V2_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "hc32f460_utility.h"
#include "hc32f460_clk.h"
#include "hc32f460_efm.h"
#include "hc32f460_gpio.h"
#include "hc32f460_exint_nmi_swi.h"
#include "hc32f460_interrupts.h"
#include "hc32f460_pwc.h"
#include "hc32f460_sram.h"
#include "hc32f460_keyscan.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F460_LQFP100_V2
 * @{
 */

#if (BSP_EV_HC32F460_LQFP100_V2 == BSP_EV_HC32F460)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/** 
 * @defgroup EV_HC32F460_LQFP100_V2_LED_Sel EV_HC32F460_LQFP100_V2 LED definition
 * @{
 */
#define LED_RED                 (0x01U)
#define LED_GREEN               (0x02U)
//#define LED_YELLOW              (0x04U)
#define LED_BLUE                (0x04U)
/**
 * @}
 */

/* KEYIN port, pin definition */
#define  KEYIN_PORT         (PortC)

#define  KEYIN0_PIN         (Pin13)


/**
 * @}
 */

/**
 * @defgroup EV_HC32F460_LQFP100_V2_Local_Macros EV_HC32F460_LQFP100_V2 Local Macros
 * @{
 */

/**
 * @defgroup EV_HC32F460_LQFP100_V2_LED_Number EV_HC32F460_LQFP100_V2 LED Number
 * @{
 */
#define BSP_LED_NUM             (3U)
/**
 * @}
 */

/** @defgroup EV_HC32F460_LQFP100_V2_LED_PortPin_Sel EV_HC32F460_LQFP100_V2 LED port/pin definition
 * @{
 */
#define BSP_LED_RED_PORT        (PortA)
#define BSP_LED_RED_PIN         (Pin00)
#define BSP_LED_GREEN_PORT      (PortA)
#define BSP_LED_GREEN_PIN       (Pin01)
#define BSP_LED_BLUE_PORT       (PortA)
#define BSP_LED_BLUE_PIN        (Pin02)
/**
 * @}
 */

/** @defgroup EV_HC32F460_LQFP100_V2_PRINT_CONFIG EV_HC32F460_LQFP100_V2 PRINT Configure definition
 * @{
 */
#define BSP_PRINTF_DEVICE       (M4_USART2)

#define BSP_PRINTF_BAUDRATE     (115200)

#define BSP_PRINTF_PORT         (PortA)
#define BSP_PRINTF_PIN          (Pin15)
#define BSP_PRINTF_PORT_FUNC    (Func_Usart2_Tx)
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup EV_HC32F460_LQFP100_V2_Global_Functions
 * @{
 */

extern uint8_t u8GlobalKey;	

void BSP_CLK_Init(void);

void BSP_KEY_Init(void);

void BSP_LED_Init(void);
void BSP_LED_On(uint8_t u8Led);
void BSP_LED_Off(uint8_t u8Led);
void BSP_LED_Toggle(uint8_t u8Led);

void BSP_PRINTF_PortInit(void);
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

#ifdef __cplusplus
}
#endif

#endif /* __EV_HC32F460_LQFP100_V2_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
