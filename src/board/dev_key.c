/**
 *******************************************************************************************************************************************
 * @file        dev_key.c
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
#include "SWM221.h"
#include "dev_key.h"

/* Btn pressed GPIO level */
#define PRESSED_LEVEL      ( 0 )

typedef struct gpio_descriptor_t
{
    GPIO_TypeDef *port;
    uint8_t pin;
} gpio_descriptor_t;

static const gpio_descriptor_t Btn_GPIO[_USER_BTN_ID_MAX_] = {
    {GPIOA, PIN4}, /* btn0 */
};
static struct Button Btn[_USER_BTN_ID_MAX_];

static uint8_t read_button_GPIO(uint8_t button_id)
{
    if (button_id >= _USER_BTN_ID_MAX_)
        return 0;
    return GPIO_GetBit(Btn_GPIO[button_id].port, Btn_GPIO[button_id].pin);
}

#if 0 /* CallBack */
static void BTN_PRESS_DOWN_Handler(void* btn)
{
    struct Button* handle = btn;
    if (handle->button_id >= _USER_BTN_ID_MAX_)
        return ;
}

static void BTN_PRESS_UP_Handler(void* btn)
{
    struct Button* handle = btn;
    if (handle->button_id >= _USER_BTN_ID_MAX_)
        return ;
}
#endif

/**
 * @brief 按键初始化
 */
void user_key_init(void)
{
    for (uint8_t button_id = 0; button_id < _USER_BTN_ID_MAX_; ++button_id)
    {
        GPIO_INIT(Btn_GPIO[button_id].port, Btn_GPIO[button_id].pin, GPIO_INPUT_PullUp);

        button_init(&Btn[button_id], read_button_GPIO, PRESSED_LEVEL, button_id);

        #if 0 /* CallBack */
        button_attach(&Btn[button_id], PRESS_DOWN,       BTN_PRESS_DOWN_Handler);
        button_attach(&Btn[button_id], PRESS_UP,         BTN_PRESS_UP_Handler);
        button_attach(&Btn[button_id], PRESS_REPEAT,     BTN_PRESS_REPEAT_Handler);
        button_attach(&Btn[button_id], SINGLE_CLICK,     BTN_SINGLE_Click_Handler);
        button_attach(&Btn[button_id], DOUBLE_CLICK,     BTN_DOUBLE_Click_Handler);
        button_attach(&Btn[button_id], LONG_PRESS_START, BTN_LONG_PRESS_START_Handler);
        button_attach(&Btn[button_id], LONG_PRESS_HOLD,  BTN_LONG_PRESS_HOLD_Handler);
        #endif

        button_start(&Btn[button_id]);
    }
    //make the timer invoking the button_ticks() interval 5ms.
    //This function is implemented by yourself.
    //__timer_start(button_ticks, 0, 5);
}

/**
 * @brief  查询按键状态
 * @note   轮询调用
 * @param  button_id: 按键标识符
 * @retval enum PressEvent see "multi_button.h"
 */
uint8_t user_key_handler(uint8_t button_id)
{
    return get_button_event(&Btn[button_id]); /* polling */
}
