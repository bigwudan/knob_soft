/**
 *******************************************************************************************************************************************
 * @file        dev_lcd_mpu.h
 * @brief       LCD MPU display
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * 2024-08-18   lzh          add QSPI interface, refactor [lcd_mpu_board_t]
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __DEV_LCD_MPU_H__
#define __DEV_LCD_MPU_H__

#include <stdint.h>

//Declaration LCD var / function
#define __DECL_LCD_NAME(__base_name, __type_name)      __base_name##_##__type_name

typedef struct lcd_mpu_cfg_t
{
    const char *name;
    uint16_t hres, vres;
    uint8_t interface; // see [app_cfg.h] enum interface
} lcd_mpu_cfg_t;

// platform provide, driver used
typedef struct lcd_mpu_driver_t
{
    void (*wr_cmd)(uint32_t cmd, void *seq); //parm: i8080 / SPI used [cmd], QSPI used [seq]
    void (*wr_data)(uint8_t *data, uint32_t count);
    void (*rd_data)(uint8_t *data, uint32_t count);

    void (*mdelay)(uint32_t ms);
    void (*hw_rst_gpio_set)(uint8_t level);
} lcd_mpu_driver_t;

// platform provide
typedef struct lcd_mpu_board_t lcd_mpu_board_t;
struct lcd_mpu_board_t
{
    lcd_mpu_driver_t driver; //driver used
    
    void (*peripheral_init)(void);                       /**< 外设初始化 */
    void (*peripheral_deinit)(void);                     /**< 外设反初始化 */
    void (*port_init)(void);                             /**< 通讯端口初始化 */
    void (*port_deinit)(void);                           /**< 通讯端口反初始化 */
    void (*draw_point)(uint32_t color);                  /**< 画点(slow) */
    void (*fill_color)(uint32_t color, uint32_t pixels); /**< 填充色块 */
    void (*flush_bitmap)(void *data, uint32_t pixels);   /**< 填充位图 */
    
    // optional: Hardware Acceleration
    /* return => 0: async vaild   other: async invaild, please use sync(note: sync always available) */
    uint8_t (*flush_bitmap_async)(void *data, uint32_t pixels);
    /* it should be inserted into the ISR Handler, return => 0: transfer completed   other: transfer busy */
    uint8_t (*flush_bitmap_async_done)(void);
    
    // optional
    uint32_t (*set_backlight)(uint32_t val); /**< 多档调光 */
    void (*set_power)(uint8_t mode);         /**< 设置屏幕电源(使用 GPIO 来控制屏幕电源通断) */
};

// driver provide
typedef struct lcd_mpu_adapter_t
{
    void (*timseq_init)(lcd_mpu_driver_t *self, lcd_mpu_cfg_t *cfg);                                   /**< 上电初始化时序以配置屏幕参数 */
    void (*set_disp_area)(lcd_mpu_driver_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye); /**< 开窗(以 0 为起点) */
    // optional
    void (*set_rotate)(lcd_mpu_driver_t *self, uint8_t dir);         /**< 设置屏幕旋转(旋转方向传参暂由具体驱动自定义实现) */
    uint32_t (*set_backlight)(lcd_mpu_driver_t *self, uint32_t val); /**< 设置背光亮度(以设置指令的方式调节屏幕亮度) */
    void (*set_power)(lcd_mpu_driver_t *self, uint8_t mode);         /**< 设置屏幕电源(以设置指令的方式进入休眠, 电流不会比直接关断屏幕供电更低) */
    int16_t offset_x, offset_y; // reserve: offset_pixels
} lcd_mpu_adapter_t;

typedef struct lcd_mpu_desc_t lcd_mpu_desc_t;
struct lcd_mpu_desc_t
{
    lcd_mpu_cfg_t cfg;
    lcd_mpu_board_t board;
    lcd_mpu_adapter_t adapter;
};

/**
 * @brief  构造实例
 * @param  self : see lcd_mpu_desc_t
 * @param  cfg  : see lcd_mpu_cfg_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t lcd_mpu_init(lcd_mpu_desc_t *self, const lcd_mpu_cfg_t *cfg);

/**
 * @brief  析构实例
 * @param  self : see lcd_mpu_desc_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t lcd_mpu_depose(lcd_mpu_desc_t *self);

/**
 * @brief  开窗
 * @param  self : see lcd_mpu_desc_t
 * @param  xs\xe\ys\ye : 窗口区域坐标(以 0 为起点)
 * @retval \
 */
static inline 
void lcd_mpu_set_disp_area(lcd_mpu_desc_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    if (self && self->adapter.set_disp_area) {
        /* += signed */
        xs += self->adapter.offset_x;
        xe += self->adapter.offset_x;
        ys += self->adapter.offset_y;
        ye += self->adapter.offset_y;
extern void st77916_set_disp_area(lcd_mpu_driver_t *self, uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
				st77916_set_disp_area(&self->board.driver, xs, xe, ys, ye);
//        self->adapter.set_disp_area(&self->board.driver, xs, xe, ys, ye);
    }
}

/**
 * @brief  异步绘图
 * @param  self : see lcd_mpu_desc_t
 * @param  data   : 数据源地址
 * @param  pixels : 像素个数
 * @retval 0     : async vaild
 * @retval other : async invaild, please use sync(note: sync always available)
 */
static inline
uint8_t lcd_mpu_flush_bitmap_async(lcd_mpu_desc_t *self, void *data, uint32_t pixels)
{
    if (self && self->board.flush_bitmap_async && self->board.flush_bitmap_async_done) {
        if (0 == self->board.flush_bitmap_async(data, pixels)) {
            return 0;
        }
        return 1;
    }
    return 2;
}

/**
 * @brief  异步绘图完成回调, 应插入到 ISR 中被调用
 * @param  self : see lcd_mpu_desc_t
 * @retval 0     : transfer completed
 * @retval other : transfer busy
 */
static inline
uint8_t lcd_mpu_flush_bitmap_done(lcd_mpu_desc_t *self)
{
    if (self && self->board.flush_bitmap_async_done) {
        return self->board.flush_bitmap_async_done();
    }
    return 1;
}

/**
 * @brief  等待异步信号就绪
 * @param  self : see lcd_mpu_desc_t
 * @param  signal_busy: 0 - ok     other - busy
 * @retval \
 */
static inline
void lcd_mpu_flush_bitmap_wait(lcd_mpu_desc_t *self, volatile uint8_t *signal_busy)
{
    if (self && self->board.flush_bitmap_async && self->board.flush_bitmap_async_done) {
        while (signal_busy && *(volatile uint8_t *)signal_busy) ;
    }
}

/* Macro Function(Not safe, it don't check the parameters type) */

//see [lcd_mpu_driver_t]
#define lcd_mpu_driver_mdelay(driver, ms)             (driver)->mdelay(ms)
#define lcd_mpu_driver_wr_cmd(driver, cmd, seq)       (driver)->wr_cmd(cmd, seq)
#define lcd_mpu_driver_wr_data(driver, data, cnt)     (driver)->wr_data(data, cnt)
#define lcd_mpu_driver_rd_data(driver, data, cnt)     (driver)->rd_data(data, cnt)

// init / deinit
#define lcd_mpu_peripheral_init(lcd_desc)             (lcd_desc)->board.peripheral_init()
#define lcd_mpu_peripheral_deinit(lcd_desc)           (lcd_desc)->board.peripheral_deinit()
#define lcd_mpu_port_init(lcd_desc)                   (lcd_desc)->board.port_init()
#define lcd_mpu_port_deinit(lcd_desc)                 (lcd_desc)->board.port_deinit()
#define lcd_mpu_timseq_init(lcd_desc)                 (lcd_desc)->adapter.timseq_init(&((lcd_desc)->board.driver), &((lcd_desc)->cfg))

// draw
#define lcd_mpu_draw_point(lcd_desc, color)           (lcd_desc)->board.draw_point(color)
#define lcd_mpu_fill_color(lcd_desc, color, pixels)   (lcd_desc)->board.fill_color(color, pixels)
#define lcd_mpu_flush_bitmap(lcd_desc, data, pixels)  (lcd_desc)->board.flush_bitmap(data, pixels)

// misc
#define lcd_mpu_set_rotate(lcd_desc, dir)             (lcd_desc)->adapter.set_rotate(&((lcd_desc)->board.driver), dir)

#define lcd_mpu_set_backlight_board(lcd_desc, val)    (lcd_desc)->board.set_backlight(val)
#define lcd_mpu_set_backlight_driver(lcd_desc, val)   (lcd_desc)->adapter.set_backlight(&((lcd_desc)->board.driver), val)
#define lcd_mpu_set_power_board(lcd_desc, mode)       (lcd_desc)->board.power(mode)
#define lcd_mpu_set_power_driver(lcd_desc, mode)      (lcd_desc)->adapter.power(&((lcd_desc)->board.driver), mode)

//自定义选择一个实现
#define lcd_mpu_set_backlight(lcd_desc, val)          lcd_mpu_set_backlight_board(lcd_desc, val)
#define lcd_mpu_set_power(lcd_desc, mode)             lcd_mpu_set_power_driver(lcd_desc, mode)

/*!
 * \note do NOT use this macro directly
 */
#ifndef __ARM_VA_NUM_ARGS_IMPL
#define __ARM_VA_NUM_ARGS_IMPL( _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,      \
                                _13,_14,_15,_16,__N,...)      __N
#endif

/*!
 * \brief A macro to count the number of parameters
 * 
 * \note if GNU extension is not supported or enabled, the following express will
 *       be false:  (__ARM_VA_NUM_ARGS() != 0)
 *       This might cause problems when in this library.
 */
#ifndef __ARM_VA_NUM_ARGS
#define __ARM_VA_NUM_ARGS(...)                                                  \
            __ARM_VA_NUM_ARGS_IMPL( 0,##__VA_ARGS__,16,15,14,13,12,11,10,9,     \
                                      8,7,6,5,4,3,2,1,0)
#endif

/*! 
 * \brief detect whether GNU extension is enabled in compilation or not
 */
#if __ARM_VA_NUM_ARGS() != 0
#   warning Please enable GNU extensions, it is required by the [__ARM_VA_NUM_ARGS].
#endif

#endif //__DEV_LCD_MPU_H__
