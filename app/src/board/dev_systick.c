/**
 *******************************************************************************************************************************************
 * @file        dev_systick.c
 * @brief       内核滴答定时器
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
#include "dev_systick.h"

/** SysTick ticks */
static volatile uint32_t Systick_Tick = 0;

__WEAK void systick_handler_hook(void)
{
}

//__WEAK 
void SysTick_Handler(void)
{
    systick_inc_tick();
    systick_handler_hook();
}

void systick_init(void)
{
    Systick_Tick = 0;
    /* SystemFrequency / 1000  =>  1ms中断一次 */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1) __NOP();
    }
#if defined(__PERFORMANCE_COUNTER_H__)
    init_cycle_counter(true);
#endif
}

void systick_stop(void)
{
#if defined(__PERFORMANCE_COUNTER_H__)
    before_cycle_counter_reconfiguration();
#else
    /* Disable SysTick IRQ and SysTick Timer */
    SysTick->CTRL = 0;
    /* pending SysTick exception */
    if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)
    {
        SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk; /* clear pending bit */
    }
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
#endif
}

void systick_inc_tick(void)
{
#if defined(__PERFORMANCE_COUNTER_H__)
    perfc_port_insert_to_system_timer_insert_ovf_handler();
#else
    ++Systick_Tick;
#endif
}

uint32_t systick_get_tick(void)
{
#if defined(__PERFORMANCE_COUNTER_H__)
    return get_system_ms(); //get_system_ticks();
#else 
    return Systick_Tick;
#endif
}

void systick_delay_ms(const uint32_t ms)
{
#if defined(__PERFORMANCE_COUNTER_H__)
    delay_ms(ms);
#else 
    uint32_t tickstart = 0;
    for (tickstart = Systick_Tick; /* 补码回环 */(uint32_t)(Systick_Tick - tickstart) < ms;) {}
#endif
}

void systick_delay_us(const uint32_t us)
{
#if defined(__PERFORMANCE_COUNTER_H__)
    delay_us(us);
#else 
    /* 不超过 ms */
    uint32_t reload = SysTick->LOAD;
    uint32_t ticks = 0;
    uint32_t told = 0, tnow = 0, tcnt = 0;
    /* 获得延时经过的 tick 数 */
    ticks = us * reload / 1000;
    /* 获得当前时间 */
    told = SysTick->VAL;
    for (;;)
    {
        /* 循环获得当前时间, 直到达到指定的时间后退出循环 */
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            tcnt += ((tnow < told) ? (told - tnow) : (reload - tnow + told));
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
#endif
}
