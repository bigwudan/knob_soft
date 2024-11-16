/**
 *******************************************************************************************************************************************
 * @file        dev_tp_i2c.c
 * @brief       Touch Device ops[I2C]
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
#include "dev_tp.h"
#include "SWM221.h"

/*******************************************************************************************************************************************
 * static defines
 *******************************************************************************************************************************************/
//#define TP_SW_I2C /* 当该板的 TP 对接端口无法映射为硬件 I2C 时可转用软件模拟. */

#ifdef TP_SW_I2C
    #define TP_GPIO_SCL         GPIOA
    #define TP_PIN_SCL          PIN6

    #define TP_GPIO_SDA         GPIOA
    #define TP_PIN_SDA          PIN7
#else /* HW_I2C */
    #define TP_PORT_SCL         PORTA
    #define TP_PIN_SCL          PIN6
    #define TP_PIN_SCL_FUN      PORTA_PIN6_I2C0_SCL

    #define TP_PORT_SDA         PORTA
    #define TP_PIN_SDA          PIN7
    #define TP_PIN_SDA_FUN      PORTA_PIN7_I2C0_SDA

    #define TP_I2C_X            I2C0
#endif
/*******************************************************************************************************************************************
 * static prototypes
 *******************************************************************************************************************************************/
#define __DECL_TP_I2C(__base_name, __type_name)      __base_name##_##__type_name
#define IMPORT_TP_I2C(__type_name)                                                    \
    static uint8_t __DECL_TP_I2C(i2c_init, __type_name)(uint32_t);                    \
    static uint8_t __DECL_TP_I2C(i2c_start, __type_name)(uint16_t, uint8_t, uint8_t); \
    static uint8_t __DECL_TP_I2C(i2c_stop, __type_name)(uint8_t);                     \
    static uint8_t __DECL_TP_I2C(i2c_read, __type_name)(uint8_t, uint8_t);            \
    static uint8_t __DECL_TP_I2C(i2c_write, __type_name)(uint8_t, uint8_t)

#ifdef TP_SW_I2C
IMPORT_TP_I2C(sw);
#else /* HW_I2C */
IMPORT_TP_I2C(hw);
#endif
/*******************************************************************************************************************************************
 * public functions
 *******************************************************************************************************************************************/
void board_tp_init_i2c(tp_desc_t *self)
{
#ifdef TP_SW_I2C
    self->board.i2c_ops.init = __DECL_TP_I2C(i2c_init, sw);
    self->board.i2c_ops.start = __DECL_TP_I2C(i2c_start, sw);
    self->board.i2c_ops.stop = __DECL_TP_I2C(i2c_stop, sw);
    self->board.i2c_ops.read = __DECL_TP_I2C(i2c_read, sw);
    self->board.i2c_ops.write = __DECL_TP_I2C(i2c_write, sw);
#else /* HW_I2C */
    self->board.i2c_ops.init = __DECL_TP_I2C(i2c_init, hw);
    self->board.i2c_ops.start = __DECL_TP_I2C(i2c_start, hw);
    self->board.i2c_ops.stop = __DECL_TP_I2C(i2c_stop, hw);
    self->board.i2c_ops.read = __DECL_TP_I2C(i2c_read, hw);
    self->board.i2c_ops.write = __DECL_TP_I2C(i2c_write, hw);
#endif
}

/**
 * @brief 关闭可能影响设备进入低功耗的端口
 * @note  Touch IC 通常支持以发送指令的方式以进入低功耗/休眠模式, 详见具体型号 TP_IC 数据手册/驱动实现.
 */
void board_tp_deinit_i2c(tp_desc_t *self)
{
#ifdef TP_SW_I2C
    GPIO_INIT(TP_GPIO_SCL, TP_PIN_SCL, GPIO_INPUT_PullUp);
    GPIO_INIT(TP_GPIO_SDA, TP_PIN_SDA, GPIO_INPUT_PullUp);
#else
    I2C_Close(TP_I2C_X);
#endif
}

/*******************************************************************************************************************************************
 * static functions
 *******************************************************************************************************************************************/
#ifdef TP_SW_I2C
#include "dev_sw_i2c.h"
static sw_i2c_desc_t SW_I2C_Obj;
#define SW_I2C_This (&SW_I2C_Obj)

static uint8_t __DECL_TP_I2C(i2c_init, sw)(uint32_t master_clk)
{
    sw_i2c_cfg_t sw_i2c_cfg = {
        .obj = {
            .scl = {TP_GPIO_SCL, TP_PIN_SCL},
            .sda = {TP_GPIO_SDA, TP_PIN_SDA},
            .delay_tick = 0, // master_clk
            .delay = NULL,   // sw_i2c_delay,
            .retry_times = 3,
            .addr_10b = 0,
        },
    };
    sw_i2c_cfg.obj.delay_tick = I2C_CLK_TO_TICK_DEFAULT(master_clk);
    return (SW_I2C_RET_OK == sw_i2c_init(SW_I2C_This, &sw_i2c_cfg)) ? 0 : 1;
}

static uint8_t __DECL_TP_I2C(i2c_start, sw)(uint16_t addr, uint8_t restart, uint8_t timeout)
{
    for (uint32_t i = 0; (timeout) ? (i < timeout) : 1; ++i)
    {
        uint8_t res = sw_i2c_start(SW_I2C_This, addr, restart);
        if (SW_I2C_RET_OK == res) {
            return 0;
        }
    }
    return 1;
}

static uint8_t __DECL_TP_I2C(i2c_stop, sw)(uint8_t timeout)
{
    for (uint32_t i = 0; (timeout) ? (i < timeout) : 1; ++i)
    {
        if (SW_I2C_RET_OK == sw_i2c_stop(SW_I2C_This)) {
            return 0;
        }
    }
    return 1;
}

static uint8_t __DECL_TP_I2C(i2c_read, sw)(uint8_t ack, uint8_t timeout)
{
    for (uint32_t i = 0; (timeout) ? (i < timeout) : 1; ++i)
    {
        uint8_t data = 0;
        if (SW_I2C_RET_OK == sw_i2c_read(SW_I2C_This, &data, ack)) {
            return data;
        }
    }
    return 0;
}

static uint8_t __DECL_TP_I2C(i2c_write, sw)(uint8_t data, uint8_t timeout)
{
    for (uint32_t i = 0; (timeout) ? (i < timeout) : 1; ++i)
    {
        uint8_t res = sw_i2c_write(SW_I2C_This, data);
        if (SW_I2C_RET_OK == res) {
            return 0;
        }
        else if (SW_I2C_RET_NO_ACK == res) {
            return 1;
        }
    }
    return 2;
}

#else /* HW_I2C */
static uint8_t __DECL_TP_I2C(i2c_init, hw)(uint32_t master_clk)
{
    PORT_Init(TP_PORT_SCL, TP_PIN_SCL, TP_PIN_SCL_FUN, 1);
    PORT_Init(TP_PORT_SDA, TP_PIN_SDA, TP_PIN_SDA_FUN, 1);
    // 必须使能上拉，用于模拟开漏
    TP_PORT_SCL->PULLU |= (1 << TP_PIN_SCL); 
    TP_PORT_SDA->PULLU |= (1 << TP_PIN_SDA);
    // 使能开漏
    TP_PORT_SCL->OPEND |= (1 << TP_PIN_SCL);
    TP_PORT_SDA->OPEND |= (1 << TP_PIN_SDA);

    I2C_InitStructure I2C_initStruct;
    I2C_initStruct.Master = 1;
    I2C_initStruct.Addr10b = 0;
    I2C_initStruct.MstClk = master_clk;
    I2C_initStruct.TXEmptyIEn = 0;
    I2C_initStruct.RXNotEmptyIEn = 0;
    I2C_Init(TP_I2C_X, &I2C_initStruct);
    I2C_Open(TP_I2C_X);
    return 0;
}

#define HW_I2C_RETRY_TIMES_DEFAULT       (SystemCoreClock / 1000)

static uint8_t __DECL_TP_I2C(i2c_start, hw)(uint16_t addr, uint8_t restart, uint8_t timeout)
{
    (void)restart; // hw auto
#if 1
    if (timeout > 0)
    {
        I2C_Start(TP_I2C_X, addr, 0);
        for (uint32_t i = 0; i < HW_I2C_RETRY_TIMES_DEFAULT; ++i) {
            if (0 == I2C_StartDone(TP_I2C_X)) {
                break;
            }
        }
        return (I2C_IsAck(TP_I2C_X)) ? 0 : 1;
    }
    return (I2C_Start(TP_I2C_X, addr, 1) ? 0 : 1);
#else
    return (I2C_Start(TP_I2C_X, addr) ? 0 : 1);
#endif
}

static uint8_t __DECL_TP_I2C(i2c_stop, hw)(uint8_t timeout)
{
#if 1
    if (timeout > 0)
    {
        I2C_Stop(TP_I2C_X, 0);
        for (uint32_t i = 0; i < HW_I2C_RETRY_TIMES_DEFAULT; ++i) {
            if (0 == I2C_StopDone(TP_I2C_X)) {
                return 0;
            }
        }
        return 1;
    }
    I2C_Stop(TP_I2C_X, 1);
#else
    I2C_Stop(TP_I2C_X);
#endif
    return 0;
}

static uint8_t __DECL_TP_I2C(i2c_read, hw)(uint8_t ack, uint8_t timeout)
{
#if 1
    if (timeout > 0)
    {
        I2C_Read(TP_I2C_X, ack, 0);
        for (uint32_t i = 0; i < HW_I2C_RETRY_TIMES_DEFAULT; ++i) {
            if (0 == I2C_ReadDone(TP_I2C_X)) {
                break;
            }
        }
        return TP_I2C_X->RXDATA;
    }
    return I2C_Read(TP_I2C_X, ack, 1);
#else
    return I2C_Read(TP_I2C_X, ack);
#endif
}

static uint8_t __DECL_TP_I2C(i2c_write, hw)(uint8_t data, uint8_t timeout)
{
#if 1
    if (timeout > 0)
    {
        I2C_Write(TP_I2C_X, data, 0);
        for (uint32_t i = 0; i < HW_I2C_RETRY_TIMES_DEFAULT; ++i) {
            if (0 == I2C_WriteDone(TP_I2C_X)) {
                break;
            }
        }
        return (I2C_IsAck(TP_I2C_X)) ? 0 : 1;
    }
    return (I2C_Write(TP_I2C_X, data, 1) ? 0 : 1);
#else
    return (I2C_Write(TP_I2C_X, data) ? 0 : 1);
#endif
}
#endif
