/**
 *******************************************************************************************************************************************
 * @file        dev_lcd_mpu.c
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
#include <string.h>
#include "dev_lcd_mpu.h"
#include "dev_systick.h"
#include "app_cfg.h"

#include "SWM221.h"

#if 1 /* 调试打印 */
#include <stdio.h>
#define LCD_LOG(...)           printf(__VA_ARGS__)
#else
#define LCD_LOG(...)           
#endif
#if 1 /* assert */
// #include <assert.h>
#define LCD_ASSERT(expr)       if (expr) for (;;) /*__NOP()*/LCD_LOG("[%s]\r\n", __FUNCTION__)
#else
#define LCD_ASSERT(expr)
#endif
/*******************************************************************************************************************************************
 * public prototypes
 *******************************************************************************************************************************************/
extern uint8_t lcd_mpu_adapter(lcd_mpu_adapter_t *adapter, uint16_t hres, uint16_t vres, const char *name);

/*******************************************************************************************************************************************
 * static prototypes
 *******************************************************************************************************************************************/
static uint8_t lcd_mpu_board_init(lcd_mpu_desc_t *self);
static uint8_t lcd_mpu_board_deinit(lcd_mpu_desc_t *self);

/*******************************************************************************************************************************************
 * static defines
 *******************************************************************************************************************************************/
#define LCD_GPIO_RST       GPIOA
#define LCD_PIN_RST        PIN2

#define LCD_PWM_HW_EN // 硬件 PWM 调光可选

#define LCD_GPIO_BL        GPIOB
#define LCD_PIN_BL         PIN13

#if defined(LCD_PWM_HW_EN) 
#define LCD_PORT_BL        PORTB
#define LCD_PORT_FUNMUX_BL PORTB_PIN13_PWM0A

#define LCD_PWM_BL         PWM0
#define LCD_PWM_CHN_BL     PWM_CH_A
#define LCD_PWM_MSK_BL     PWM0_MSK
#endif

/*******************************************************************************************************************************************
 * public functions
 *******************************************************************************************************************************************/
/**
 * @brief  构造实例
 * @param  self : see lcd_mpu_desc_t
 * @param  cfg  : see lcd_mpu_cfg_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t lcd_mpu_init(lcd_mpu_desc_t *self, const lcd_mpu_cfg_t *cfg)
{
    LCD_ASSERT(!self);
    uint8_t res = 0;
    if (cfg) {
        memcpy(&self->cfg, cfg, sizeof(lcd_mpu_cfg_t));

        memset(&self->adapter, 0, sizeof(lcd_mpu_adapter_t));
        res = lcd_mpu_adapter(&self->adapter, self->cfg.hres, self->cfg.vres, self->cfg.name);
        if (0 != res) {
            LCD_LOG("[%s]: [lcd_mpu_adapter] return error code = [%d]!\r\n", __FUNCTION__, res);
            return res;
        }
    }
    memset(&self->board, 0, sizeof(struct lcd_mpu_board_t));
    res = lcd_mpu_board_init(self);
    if (0 != res) {
        LCD_LOG("[%s]: [lcd_mpu_board_init] return error code = [%d]!\r\n", __FUNCTION__, res);
        return res;
    }
    /* init method */
    lcd_mpu_port_init(self);
    lcd_mpu_peripheral_init(self);
    lcd_mpu_timseq_init(self);
    return 0;
}

/**
 * @brief  析构实例
 * @param  self : see lcd_mpu_desc_t
 * @retval 0     : success
 * @retval other : error code
 */
uint8_t lcd_mpu_depose(lcd_mpu_desc_t *self)
{
    LCD_ASSERT(!self);
    uint8_t res = 0;
    res = lcd_mpu_board_deinit(self);
    if (0 != res) {
        LCD_LOG("[%s]: [lcd_mpu_board_init] return error code = [%d]!\r\n", __FUNCTION__, res);
        return res;
    }
    memset(self, 0, sizeof(lcd_mpu_desc_t));
    // free(self); //unused(self)
    return 0;
}

/*******************************************************************************************************************************************
 * static functions
 *******************************************************************************************************************************************/
#define IMPORT_LCD_DRIVER(__interface_name)                                                \
    extern void __DECL_LCD_NAME(wr_cmd, __interface_name)(uint32_t cmd, void *seq);        \
    extern void __DECL_LCD_NAME(wr_data, __interface_name)(uint8_t *data, uint32_t count); \
    extern void __DECL_LCD_NAME(rd_data, __interface_name)(uint8_t *data, uint32_t count)

#define IMPORT_LCD_BOARD(__interface_name)                                                      \
    extern void __DECL_LCD_NAME(peripheral_init, __interface_name)(void);                       \
    extern void __DECL_LCD_NAME(peripheral_deinit, __interface_name)(void);                     \
    extern void __DECL_LCD_NAME(port_init, __interface_name)(void);                             \
    extern void __DECL_LCD_NAME(port_deinit, __interface_name)(void);                           \
    extern void __DECL_LCD_NAME(draw_point, __interface_name)(uint32_t color);                  \
    extern void __DECL_LCD_NAME(fill_color, __interface_name)(uint32_t color, uint32_t pixels); \
    extern void __DECL_LCD_NAME(flush_bitmap, __interface_name)(void *data, uint32_t pixels)

#define IMPORT_LCD_HW_ASYNC(__interface_name)                                                          \
    extern uint8_t __DECL_LCD_NAME(flush_bitmap_async, __interface_name)(void *data, uint32_t pixels); \
    extern uint8_t __DECL_LCD_NAME(flush_bitmap_async_done, __interface_name)(void)

#define IMPORT_LCD_MISC(__interface_name)                                           \
    extern uint32_t __DECL_LCD_NAME(set_backlight, __interface_name)(uint32_t val); \
    extern void __DECL_LCD_NAME(mdelay, __interface_name)(uint32_t ms);             \
    extern void __DECL_LCD_NAME(hw_rst_gpio_set, __interface_name)(uint8_t level)

IMPORT_LCD_DRIVER(qspi);
IMPORT_LCD_DRIVER(i80_8);
IMPORT_LCD_DRIVER(spi_8);
IMPORT_LCD_DRIVER(spi_9);

IMPORT_LCD_BOARD(qspi);
IMPORT_LCD_BOARD(i80_8);
IMPORT_LCD_BOARD(spi_8);
IMPORT_LCD_BOARD(spi_9);

IMPORT_LCD_HW_ASYNC(qspi);
IMPORT_LCD_HW_ASYNC(i80_8);
IMPORT_LCD_HW_ASYNC(spi_8);

#define LINK_LCD_DRIVER(__lcd_mpu_desc, __interface_name)                                \
    (__lcd_mpu_desc)->board.driver.wr_cmd = __DECL_LCD_NAME(wr_cmd, __interface_name);   \
    (__lcd_mpu_desc)->board.driver.wr_data = __DECL_LCD_NAME(wr_data, __interface_name); \
    (__lcd_mpu_desc)->board.driver.rd_data = __DECL_LCD_NAME(rd_data, __interface_name)

#define LINK_LCD_HW_ASYNC(__lcd_mpu_desc, __interface_name)                                             \
    (__lcd_mpu_desc)->board.flush_bitmap_async = __DECL_LCD_NAME(flush_bitmap_async, __interface_name); \
    (__lcd_mpu_desc)->board.flush_bitmap_async_done = __DECL_LCD_NAME(flush_bitmap_async_done, __interface_name)

#define LINK_LCD_BOARD(__lcd_mpu_desc, __interface_name)                                              \
    (__lcd_mpu_desc)->board.peripheral_init = __DECL_LCD_NAME(peripheral_init, __interface_name);     \
    (__lcd_mpu_desc)->board.peripheral_deinit = __DECL_LCD_NAME(peripheral_deinit, __interface_name); \
    (__lcd_mpu_desc)->board.port_init = __DECL_LCD_NAME(port_init, __interface_name);                 \
    (__lcd_mpu_desc)->board.port_deinit = __DECL_LCD_NAME(port_deinit, __interface_name);             \
    (__lcd_mpu_desc)->board.draw_point = __DECL_LCD_NAME(draw_point, __interface_name);               \
    (__lcd_mpu_desc)->board.fill_color = __DECL_LCD_NAME(fill_color, __interface_name);               \
    (__lcd_mpu_desc)->board.flush_bitmap = __DECL_LCD_NAME(flush_bitmap, __interface_name)
    
static void lcd_rst_gpio_set(uint8_t level);
static void lcd_port_init_backlight(void);
static uint32_t lcd_set_backlight(uint32_t val);

static uint8_t lcd_mpu_board_init(lcd_mpu_desc_t *self)
{
    switch (self->cfg.interface)
    {
#if CFG_LCD == CFG_LCD_IF_QSPI
    case (CFG_LCD_IF_QSPI):
    {
        LINK_LCD_DRIVER(self, qspi);
        LINK_LCD_BOARD(self, qspi);
        /* optional: Hardware Acceleration 
        LINK_LCD_HW_ASYNC(self, qspi);
        */
        break;
    }
#elif CFG_LCD == CFG_LCD_IF_I8080_8
    case (CFG_LCD_IF_I8080_8):
    {
        LINK_LCD_DRIVER(self, i80_8);
        LINK_LCD_BOARD(self, i80_8);
        /* optional: Hardware Acceleration */
        LINK_LCD_HW_ASYNC(self, i80_8);
        break;
    }
#elif CFG_LCD == CFG_LCD_IF_SPI_8
    case (CFG_LCD_IF_SPI_8):
    {
        LINK_LCD_DRIVER(self, spi_8);
        LINK_LCD_BOARD(self, spi_8);
        /* optional: Hardware Acceleration */
        LINK_LCD_HW_ASYNC(self, spi_8);
        break;
    }
#elif CFG_LCD == CFG_LCD_IF_SPI_9
    case (CFG_LCD_IF_SPI_9):
    {
        LINK_LCD_DRIVER(self, spi_9);
        LINK_LCD_BOARD(self, spi_9);
        break;
    }
#endif
    default:
    {
        LCD_LOG("[%s]: [self->cfg.interface] error = [%d]!\r\n", __FUNCTION__, self->cfg.interface);
        return 1;
    }
    }
    
    /* board & driver misc */
    lcd_port_init_backlight();
    self->board.set_backlight = lcd_set_backlight;

    GPIO_INIT(LCD_GPIO_RST, LCD_PIN_RST, GPIO_OUTPUT);
    self->board.driver.hw_rst_gpio_set = lcd_rst_gpio_set;
    
    self->board.driver.mdelay = systick_delay_ms;
    return 0;
}

static uint8_t lcd_mpu_board_deinit(lcd_mpu_desc_t *self)
{
    GPIO_INIT(LCD_GPIO_RST, LCD_PIN_RST, GPIO_INPUT);

    GPIO_INIT(LCD_GPIO_BL, LCD_PIN_BL, GPIO_INPUT);
    if (self && self->board.port_deinit) {
        self->board.port_deinit();
    }
    return 0;
}

static void lcd_rst_gpio_set(uint8_t level)
{
    if (0 == level) {
        GPIO_AtomicClrBit(LCD_GPIO_RST, LCD_PIN_RST);
    } else {
        GPIO_AtomicSetBit(LCD_GPIO_RST, LCD_PIN_RST);
    }
}

static void lcd_port_init_backlight(void)
{
#if defined(LCD_PWM_HW_EN)
    PORT_Init(LCD_PORT_BL, LCD_PIN_BL, LCD_PORT_FUNMUX_BL, 0);
    
    PWM_InitStructure  PWM_initStruct;
	PWM_initStruct.Mode = PWM_EDGE_ALIGNED;
	PWM_initStruct.Clkdiv = 6;    //F_PWM = 72M/6 = 12M
	PWM_initStruct.Period = 1000; //12M/1000 = 12KHz
	PWM_initStruct.HdutyA = 0;
	PWM_initStruct.DeadzoneA = 0;
	PWM_initStruct.IdleLevelA = 0;
	PWM_initStruct.IdleLevelAN= 0;
	PWM_initStruct.OutputInvA = 0;
	PWM_initStruct.OutputInvAN= 0;
	PWM_initStruct.HdutyB = 0;
	PWM_initStruct.DeadzoneB = 0;
	PWM_initStruct.IdleLevelB = 0;
	PWM_initStruct.IdleLevelBN= 0;
	PWM_initStruct.OutputInvB = 0;
	PWM_initStruct.OutputInvBN= 0;
	PWM_initStruct.UpOvfIE    = 0;
	PWM_initStruct.DownOvfIE  = 0;
	PWM_initStruct.UpCmpAIE   = 0;
	PWM_initStruct.DownCmpAIE = 0;
	PWM_initStruct.UpCmpBIE   = 0;
	PWM_initStruct.DownCmpBIE = 0;
	PWM_Init(LCD_PWM_BL, &PWM_initStruct);
	PWM_Start(LCD_PWM_MSK_BL);
#else
    GPIO_INIT(LCD_GPIO_BL, LCD_PIN_BL, GPIO_OUTPUT);
    GPIO_AtomicClrBit(LCD_GPIO_BL, LCD_PIN_BL);
#endif
}

/**
 * @brief  LCD 背光亮度调节
 * @note   单位亮度可自定义
 * @param  val : 期望设置亮度(0:暗       0~100:自定义亮度     100:亮)
 * @retval 实际设置亮度
 */
static uint32_t lcd_set_backlight(uint32_t val)
{
#if defined(LCD_PWM_HW_EN)
    const uint32_t bg_stride = 100;
    
    PWM_ReloadDis(LCD_PWM_MSK_BL);
    PWM_SetHDuty(LCD_PWM_BL, LCD_PWM_CHN_BL, PWM_GetPeriod(LCD_PWM_BL) / bg_stride * val);
    PWM_ReloadEn(LCD_PWM_MSK_BL);
    return val;
#else
    if (val > 0)
    {
        GPIO_AtomicSetBit(LCD_GPIO_BL, LCD_PIN_BL); /* 点亮背光 */
        return 1;
    }
    // else //if (val == 0)
    {
        GPIO_AtomicClrBit(LCD_GPIO_BL, LCD_PIN_BL); /* 熄灭背光 */
        return 0;
    }
#endif
}