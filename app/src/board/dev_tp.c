/**
 *******************************************************************************************************************************************
 * @file        dev_tp.c
 * @brief       Touch Device
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
#include <string.h>
#include "dev_tp.h"

#include "dev_systick.h"
#include "SWM221.h"

#if 1 /* 调试打印 */
#include <stdio.h>
#define TP_LOG(...)           printf(__VA_ARGS__)
#else
#define TP_LOG(...)           
#endif
#if 1 /* assert */
//#include <assert.h>
# define TP_ASSERT(expr)      if (expr) for(;;) /*__NOP()*/TP_LOG("[%s]\r\n", __FUNCTION__)
#else 
# define TP_ASSERT(expr)      
#endif
/*******************************************************************************************************************************************
 * public prototypes
 *******************************************************************************************************************************************/
extern uint8_t tp_adapter(tp_adapter_t *self, const char *name);
extern void board_tp_init_i2c(tp_desc_t *self);
extern void board_tp_deinit_i2c(tp_desc_t *self);

/*******************************************************************************************************************************************
 * static defines
 *******************************************************************************************************************************************/
#define TP_GPIO_RST         GPIOA
#define TP_PIN_RST          PIN3

#define TP_GPIO_INT         GPIOA
#define TP_PIN_INT          PIN4
#define TP_INT_IRQ          GPIOA_IRQn
#define TP_INT_ISR          GPIOA_Handler

/*******************************************************************************************************************************************
 * static prototypes
 *******************************************************************************************************************************************/
static void board_tp_init_common(tp_desc_t *self);
static void board_tp_deinit_common(tp_desc_t *self);

/*******************************************************************************************************************************************
 * static variables
 *******************************************************************************************************************************************/
static volatile uint8_t TP_INT_ISR_Flag = 0; /* 0: idle   1: busy */

/*******************************************************************************************************************************************
 * public functions
 *******************************************************************************************************************************************/
__WEAK
void tp_int_handler_hook(void)
{
}
/* GPIO_[x] EXTI ISR */
void TP_INT_ISR(void)
{
    if (EXTI_State(TP_GPIO_INT, TP_PIN_INT)) {
        EXTI_Clear(TP_GPIO_INT, TP_PIN_INT);
        //for list exec callback
        TP_INT_ISR_Flag = 1;
        tp_int_handler_hook();
    }
}
static uint8_t tp_int_interrupt_ready_cb(void)
{
    return (0 != TP_INT_ISR_Flag) ? 0 : 1;
}
static void tp_int_interrupt_clear_cb(void)
{
    TP_INT_ISR_Flag = 0;
}

/**
 * @brief  构造实例
 * @param  self : see tp_desc_t
 * @param  cfg  : see tp_cfg_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t tp_init(tp_desc_t *self, tp_cfg_t *cfg)
{
    TP_ASSERT(!self);
    uint8_t res = 0;
    if (cfg) {
        memcpy(&self->cfg, cfg, sizeof(tp_cfg_t));
        res = tp_adapter(&self->adapter, self->cfg.name);
        if (0 != res) {
            return res;
        }
    } 
    board_tp_init_common(self);
    board_tp_init_i2c(self);
    if (self->adapter.init) {
        res = self->adapter.init(self);
        if (0 != res) {
            TP_LOG("[%s]: [%s] adapter.init return error code = [%d]!\r\n", __FUNCTION__, self->cfg.name, res);
            return res;
        }
        /* add observer to list */
        tp_probe_int_interrupt_ready_cb(self, tp_int_interrupt_ready_cb); //默认示例是单例, 预先为用户强制注册好, 可通过外部调用覆盖内部的实现
        tp_probe_int_interrupt_clear_cb(self, tp_int_interrupt_clear_cb);
    }
    return 0;
}

/**
 * @brief  析构实例
 * @param  self : see tp_desc_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t tp_device_depose(tp_desc_t *self)
{
    TP_ASSERT(!self);
    board_tp_deinit_common(self);
    board_tp_deinit_i2c(self);
    memset(self, 0, sizeof(tp_desc_t));
    //free(self); //unused(self)
    return 0;
}

/**
 * @brief  读触摸点
 * @param  self    : see tp_desc_t
 * @param  tp_data : 返回触摸点数据的数组
 * @param  points  : 期望读取多少个触摸点
 * @retval 实际读取到的触摸点个数
 */
uint8_t tp_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points)
{
    TP_ASSERT(!(self && tp_data && points));
    if (self->cfg.work_mode == TP_MODE_INT 
        && self->board.hw_int_gpio_interrupt_ready 
        && self->board.hw_int_gpio_interrupt_ready()
        )
    {
        //TP_LOG("[%s]: work_mode[%d]\r\n", self->cfg.name, self->cfg.work_mode);
        return 0;
    }
    if (self->board.hw_int_gpio_interrupt_clear) {
        self->board.hw_int_gpio_interrupt_clear(); // clearing
    }
    if (self->adapter.read_points) {
        return self->adapter.read_points(self, tp_data, points);
    }
    return 0;
}

/**
 * @brief  低功耗待机
 * @param  self : see tp_desc_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t tp_standby(tp_desc_t *self)
{
    TP_ASSERT(!self);
    if (self->adapter.standby) {
        self->adapter.standby(self);
    }
    board_tp_deinit_common(self);
    //board_tp_deinit_i2c(self);
    return 0;
}

/*******************************************************************************************************************************************
 * static functions
 *******************************************************************************************************************************************/
static void tp_rst_gpio_set(uint8_t level);
static void tp_int_gpio_set_input(uint8_t input_mode);
static void tp_int_gpio_interrupt_init(uint8_t exti_mode);
static void tp_int_gpio_interrupt_deinit(void);

static void board_tp_init_common(tp_desc_t *self)
{
    /* board common */
    GPIO_INIT(TP_GPIO_RST, TP_PIN_RST, GPIO_OUTPUT);
    self->board.hw_rst_gpio_set = tp_rst_gpio_set;

    GPIO_INIT(TP_GPIO_INT, TP_PIN_INT, GPIO_INPUT);
    self->board.hw_int_gpio_set_input = tp_int_gpio_set_input;
    self->board.hw_int_gpio_interrupt_init = tp_int_gpio_interrupt_init;
    self->board.hw_int_gpio_interrupt_deinit = tp_int_gpio_interrupt_deinit;

    self->board.mdelay = systick_delay_ms;
}

static void board_tp_deinit_common(tp_desc_t *self)
{
    if (self->cfg.work_mode == TP_MODE_INT) {
        if (self->board.hw_int_gpio_interrupt_deinit) {
            self->board.hw_int_gpio_interrupt_deinit();
        }
    }
}

static void tp_rst_gpio_set(uint8_t level)
{
    if (0 == level) {
        GPIO_AtomicClrBit(TP_GPIO_RST, TP_PIN_RST);
    }
    else {
        GPIO_AtomicSetBit(TP_GPIO_RST, TP_PIN_RST);
    }
}

static void tp_int_gpio_set_input(uint8_t input_mode)
{
    if (0 == input_mode) {
        GPIO_INIT(TP_GPIO_INT, TP_PIN_INT, GPIO_INPUT_PullDown);
    }
    else /*if (1 == input_mode)*/ {
        GPIO_INIT(TP_GPIO_INT, TP_PIN_INT, GPIO_INPUT_PullUp);
    }
}

static void tp_int_gpio_interrupt_init(uint8_t exti_mode)
{
    GPIO_INIT(TP_GPIO_INT, TP_PIN_INT, GPIO_INPUT);
    if (0 == exti_mode) {
        EXTI_Init(TP_GPIO_INT, TP_PIN_INT, EXTI_FALL_EDGE); //下降沿触发
    }
    else /*if (1 == exti_mode)*/ {
        EXTI_Init(TP_GPIO_INT, TP_PIN_INT, EXTI_RISE_EDGE); //上降沿触发
    }
    //EXTI_Init(TP_GPIO_INT, TP_PIN_INT, EXTI_BOTH_EDGE); //双边沿触发
    NVIC_EnableIRQ(TP_INT_IRQ);
    EXTI_Open(TP_GPIO_INT, TP_PIN_INT);
}

static void tp_int_gpio_interrupt_deinit(void)
{
    GPIO_INIT(TP_GPIO_INT, TP_PIN_INT, GPIO_INPUT);
    NVIC_DisableIRQ(TP_INT_IRQ);
    EXTI_Close(TP_GPIO_INT, TP_PIN_INT);
    NVIC_ClearPendingIRQ(TP_INT_IRQ);
}
