/**
 *******************************************************************************************************************************************
 * @file        dev_sleep_stop.c
 * @brief       浅/深睡眠
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * @remark      如想测量最低功耗电流值, 需要对 Demo 板进行硬件处理, 去掉磁珠 FB3 (3V3 -> V33), 以电流表安倍档串联观测示数.
 * @test        基于 221 系列型号 Demo 板实测, MCU 进入浅睡眠下 约为 ? uA 左右, 个体差异会有些许浮动.
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
#include "SWM221.h"
#include "dev_sleep_stop.h"

/* 复用 Touch_INT 端口, 应用 TP 触摸时 INT 端口会产生脉冲信号的特性 */

/* Sleep Wake Up (IO) */
/* P (X) WKEN : PORT (x) 唤醒使能控制寄存器 */
/* P (X) WKSR : PORT (x) 唤醒状态寄存器 */
#define SLEEP_GPIO         GPIOA
#define SLEEP_PORT         PORTA
#define SLEEP_PIN          PIN4
#define SLEEP_PX_WKEN      PAWKEN
#define SLEEP_PX_WKSR      PAWKSR

static void sleep_io_all(void);

__attribute__((used, section(".SRAM")))
static void enter_sleep_mode(void)
{
    __NOP();__NOP();__NOP();
    SYS->SLEEP |= SYS_SLEEP_SLEEP_Msk;
}

/**
 * @brief 进入浅睡眠模式, 被唤醒后会退出该函数恢复程序继续执行, 外设工作状态以及 RAM 数据将会保持.
 * @note  芯片的 ISP、SWD 引脚默认开启了上拉电阻, 会增加休眠功耗, 若想获得最低休眠功耗, 休眠前请关闭所有引脚的上拉/下拉电阻以及数字输入.
 * 故在退出浅睡眠后需要重新配置对应端口以恢复休眠前的工作状态.
 */
void sleep_init(void)
{
    /* 关闭所有引脚的上拉和下拉电阻以及数字输入, 将功耗降至最低 */
    sleep_io_all();

    __disable_irq();
    /* 休眠前确保内部 32KHz 低频振荡器处于开启 */
    
    SYS->RCCR |= SYS_RCCR_LON_Msk; //休眠模式使用低频时钟
    
    /* 休眠前必须切换到内部高频 */
    switchToHRC();
    /* 若有使用 外部晶振 或 PLL, 要达成最低功耗必须关掉 */
    if (SYS->PLLCR) {
        SYS->PLLCR &= ~(1 << SYS_PLLCR_OUTEN_Pos); /* 关闭 PLL */
    }
    if (SYS->XTALCR) {
        SYS->XTALCR = 0; /* 关闭外部时钟 */
    }
    SystemCoreClockUpdate();
    __enable_irq();
    
    /* 可使用唤醒方式: 指定任意一个或多个 IO 下降沿唤醒 */
    GPIO_INIT(SLEEP_GPIO, SLEEP_PIN, GPIO_INPUT_PullUp); /* 输入, 上拉使能, 接 KEY */
    SYS->SLEEP_PX_WKSR |= (1 << SLEEP_PIN);              /* 清除对应引脚唤醒标志 */
    SYS->SLEEP_PX_WKEN |= (1 << SLEEP_PIN);              /* 使能对应引脚下降沿唤醒(对应引脚在休眠期间须保持高电平) */

    /* 进入 Sleep 模式 */
    enter_sleep_mode();

    while ((0 == (SYS->SLEEP_PX_WKSR & (1 << SLEEP_PIN))))
    {
        __NOP(); /* 浅睡眠下直至检测到指定 IO 下降沿唤醒 */
    }

    SYS->SLEEP_PX_WKSR |= (1 << SLEEP_PIN);  /* 清除对应引脚唤醒标志 */
    SYS->SLEEP_PX_WKEN &= ~(1 << SLEEP_PIN); /* 失能对应引脚下降沿唤醒 */

    /* 唤醒后恢复为上电默认的系统时钟配置 */
    SystemInit();
    
    /* 总是保持 SWD 有效(如有 SWD 特殊复用需求请屏蔽此处) */
    *((volatile uint32_t *)0x40000190) = 1;
    PORT_Init(PORTC, PIN1, PORTC_PIN1_SWDIO, 1);
    PORT_Init(PORTC, PIN0, PORTC_PIN0_SWCLK, 1);
}

/**
 * @brief 休眠全部端口
 */
static void sleep_io_all(void)
{
/* 关上拉电阻/下拉电阻/数字输入, 配置为普通 GPIO 功能
 * note: SWD 端口功能切换未在固件库中开放给用户, 避免用户配置端口错误而导致无法下载, 重新上电后, SWD 端口总会自动恢复为 SWD 功能.
 * 1、SWD 端口切换为 GPIO 或 其它非 SWD 功能前必须加上 *((volatile uint32_t *)0x40000190) = 0; 
 * 2、SWD 端口恢复为 SWD 功能前必须加上 *((volatile uint32_t *)0x40000190) = 1; 
 */
#define IO_SLEEP(port, pin)             \
    do {                                \
        if ( (port) == PORTC && ( (pin) == PIN0 || (pin) == PIN1 ) ) { \
            *((volatile uint32_t *)0x40000190) = 0;                    \
        }                                                              \
        (port)->PULLU &= ~(1 << (pin)); \
        (port)->PULLD &= ~(1 << (pin)); \
        PORT_Init((port), (pin), 0, 0); \
    } while (0)

#define GET_ARRAY_NUMS(array)           (sizeof(array) / sizeof((array)[0]))

    PORT_TypeDef *port_table[] = {
        PORTA, PORTB, PORTC, 
    };
    uint8_t pin_template[] = {
        PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15,
    };
    uint8_t pin_B[] = {
        PIN0, PIN1, /*PIN2, PIN3,*/ PIN4, PIN5, PIN6, PIN7, /*PIN8,*/ PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, /*PIN15,*/
    };
    uint8_t pin_C[] = {
        PIN0, PIN1, /*PIN2,*/ PIN3, PIN4, /*PIN5, PIN6,*/ PIN7, PIN8, PIN9, PIN10, /*PIN11, PIN12, PIN13, PIN14, PIN15,*/
    };
    struct {
        uint8_t *pin_list;
        uint32_t pin_nums;
    } pin_table[GET_ARRAY_NUMS(port_table)] = {
        /* 如果每个端口组的 pin 数量不一致, 则在初始化时显式声明指定 */
        {&pin_template[0], GET_ARRAY_NUMS(pin_template)}, // GPIOA pin[0 ~ 15]
        {&pin_B[0], GET_ARRAY_NUMS(pin_B)},               // GPIOB pin[0 ~ 15](not pin 2/3/8/15)
        {&pin_C[0], GET_ARRAY_NUMS(pin_C)},               // GPIOM pin[0\1\3\4, 7 ~ 10]
    };
    /* 如果每个端口组的 pin 数量一致, 则通过迭代器初始化
    for (uint32_t i = 0; i < GET_ARRAY_NUMS(pin_table); ++i) {
        pin_table[i].pin_list = &pin_template[0];
        pin_table[i].pin_nums = GET_ARRAY_NUMS(pin_template);
    }*/
    /* 休眠端口 */
    for (uint32_t i = 0; i < GET_ARRAY_NUMS(port_table); ++i) {
        for (uint32_t pin_i = 0; pin_i < pin_table[i].pin_nums; ++pin_i) {
            IO_SLEEP(port_table[i], pin_table[i].pin_list[pin_i]);
        }
    }
}
