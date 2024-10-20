/**
 *******************************************************************************************************************************************
 * @file        dev_key.h
 * @brief       Key Device
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __DEV_KEY_H__
#define __DEV_KEY_H__

#include <stdint.h>
#include "multi_button.h"

enum //Button_IDs
{
    USER_BTN_ID_0 = 0,
    _USER_BTN_ID_MAX_,
};

/**
 * @brief 按键初始化
 */
void user_key_init(void);

/**
 * @brief  查询按键状态
 * @note   轮询调用
 * @param  button_id: 按键标识符
 * @retval enum PressEvent see "multi_button.h"
 */
uint8_t user_key_handler(uint8_t button_id);

#endif /* __DEV_KEY_H__ */
