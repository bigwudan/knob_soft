/**
 *******************************************************************************************************************************************
 * @file        dev_tp.h
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
#ifndef __DEV_TP_H__
#define __DEV_TP_H__

#include <stdint.h>

#define TP_MAX_POINTS      (5) /**< 一般电容屏同时支持的最大点数为 5 点 */

/** 触摸状态 */
enum
{
    TP_STA_RELEASED = 0,
    TP_STA_PRESSED,
};

/** 触摸数据结构体 */
typedef struct tp_data_t
{
    uint16_t x, y; /**< 触摸点物理坐标 */
    uint8_t sta;   /**< 触摸点状态(抬起/按下/移动等) */
//    uint16_t width;    /**< 触摸点的宽度大小(Reserve) */
//    uint16_t track_id; /**< 每个触摸点的触摸轨迹(Reserve) */
} tp_data_t;

/** TP 触摸坐标校准(即使是同一驱动型号的芯片, 由于模组厂商贴玻璃时的基准选取不同, 故需要设置校准参数) */
typedef struct tp_align_pos_t // default: (0, 0, 0, 0, 0, lcd_hres, lcd_vres)
{
    uint8_t mirror_x : 1;       /**< X 镜像 */
    uint8_t mirror_y : 1;       /**< Y 镜像 */
    uint8_t swap_xy  : 1;       /**< X/Y 坐标对调(即X轴变成Y轴, Y轴变成X轴) */
    int16_t offset_x, offset_y; /**< X/Y 坐标偏移 */
    uint16_t max_x, max_y;      /**< X/Y 坐标上限 */
} tp_align_pos_t;

enum
{
    TP_MODE_POLL = 0, /**< 只要调用 tp_read_point 就会发起 I2C 通讯 */
    TP_MODE_INT,      /**< 仅接收 INT 脉冲有效下调用 tp_read_point 才会发起 I2C 通讯 */
};

typedef struct tp_cfg_t
{
    const char *name;
    uint8_t work_mode;
    tp_align_pos_t pos;
} tp_cfg_t;

typedef struct i2c_desc_ops_t
{
    //return => 0: success   other: error code
    /* timeout:  
     * 0: 无超时, 阻塞死循环查询
     * >0: retry_times, 超时则抛出
     */
    uint8_t (*init)(uint32_t master_clk);
    uint8_t (*start)(uint16_t addr, uint8_t restart, uint8_t timeout);
    uint8_t (*stop)(uint8_t timeout);
    uint8_t (*read)(uint8_t ack, uint8_t timeout);
    uint8_t (*write)(uint8_t data, uint8_t timeout);
} i2c_desc_ops_t;

typedef struct tp_board_t
{
    i2c_desc_ops_t i2c_ops;
    void (*mdelay)(uint32_t ms);
    void (*hw_rst_gpio_set)(uint8_t level);
    // void (*hw_int_gpio_set)(uint8_t level); //reserve: 通常 TP_INT 不需要做输出
    void (*hw_int_gpio_set_input)(uint8_t input_mode);     // 0: GPIO_INPUT_PullDown     1: GPIO_INPUT_PullUp
    void (*hw_int_gpio_interrupt_init)(uint8_t exti_mode); // 0: EXTI_FALL_EDGE          1: EXTI_RISE_EDGE
    void (*hw_int_gpio_interrupt_deinit)(void);
    uint8_t (*hw_int_gpio_interrupt_ready)(void); //return => 0: ready   other: no ready
    void (*hw_int_gpio_interrupt_clear)(void);
} tp_board_t;

typedef struct tp_desc_t tp_desc_t;

typedef struct tp_adapter_t tp_adapter_t;
struct tp_adapter_t
{
    uint8_t (*init)(tp_desc_t *self);
    uint8_t (*read_points)(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points);
    uint8_t (*standby)(tp_desc_t *self); //optional: 需 TP Driver IC 支持以指令的形式进入低功耗
};

struct tp_desc_t
{
    tp_cfg_t cfg;
    tp_board_t board;
    tp_adapter_t adapter;
};

extern //__WEAK
void tp_int_handler_hook(void);

/**
 * @brief 注册 tp 相关中断信号相关回调(就绪 / 清除)
 */
static inline
void tp_probe_int_interrupt_ready_cb(tp_desc_t *self, uint8_t (*hw_int_gpio_interrupt_ready_cb)(void))
{
    if (self) {
        self->board.hw_int_gpio_interrupt_ready = hw_int_gpio_interrupt_ready_cb;
    }
}
static inline 
void tp_probe_int_interrupt_clear_cb(tp_desc_t *self, void (*hw_int_gpio_interrupt_clear_cb)(void))
{
    if (self) {
        self->board.hw_int_gpio_interrupt_clear = hw_int_gpio_interrupt_clear_cb;
    }
}

/**
 * @brief  构造实例
 * @param  self : see tp_desc_t
 * @param  cfg  : see tp_cfg_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t tp_init(tp_desc_t *self, tp_cfg_t *cfg);

/**
 * @brief  析构实例
 * @param  self : see tp_desc_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t tp_device_depose(tp_desc_t *self);

/**
 * @brief  读触摸点
 * @param  self    : see tp_desc_t
 * @param  tp_data : 返回触摸点数据的数组
 * @param  points  : 期望读取多少个触摸点
 * @retval 实际读取到的触摸点个数
 */
uint8_t tp_read_points(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points);

/**
 * @brief  低功耗待机
 * @param  self : see tp_desc_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t tp_standby(tp_desc_t *self);

#endif //__DEV_TP_H__
