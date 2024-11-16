/**
 *******************************************************************************************************************************************
 * @file        board.c
 * @brief       板级硬件外设相关
 * @since       Change Logs:
 * Date         Author       Notes
 * 2022-12-08   lzh          the first version
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include "board.h"

static void nvic_config(void);

/**
 * @brief 初始化所有的硬件设备, 该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量(只需要调用一次)
 */
void board_init(void)
{
    SystemInit();
    nvic_config();
    uart_debug_init();
    systick_init();
}

/**
 * @brief 对用户使用的可编程硬件中断设置优先级及分组
 */
static void nvic_config(void)
{
    /* 分组默认为 0 (注意这里和 ST 的 NVIC_PriorityGroup_0 宏不是一个意思,请勿混淆), 设为 0 表示全部为抢占优先级, 无子优先级
     * SWM221 使用 __NVIC_PRIO_BITS == 2 bit, 支持 2 种优先级分组(0 ~ 1), 4级优先级(0 ~ 3)
     * 如无必要, 请勿调整优先级分组, 默认分组即可应用 RTOS.
     */

    //NVIC_SetPriorityGrouping(0); //优先级分组设定, 只需要被调用一次，一旦分组确定就最好不要更改，否则容易造成程序中断分组混乱
    //NVIC_SetPriority(IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), PreemptPriority, SubPriority)); //优先级设定(抢占 && 子优先级)
}
