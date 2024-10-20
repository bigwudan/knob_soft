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
#ifndef __DEV_SLEEP_STOP_H__
#define __DEV_SLEEP_STOP_H__

#include <stdint.h>
//#include <cmsis_compiler.h>

/**
 * @brief 进入浅睡眠模式, 被唤醒后会退出该函数恢复程序继续执行, 外设工作状态以及 RAM 数据将会保持.
 * @note  芯片的 ISP、SWD 引脚默认开启了上拉电阻, 会增加休眠功耗, 若想获得最低休眠功耗, 休眠前请关闭所有引脚的上拉/下拉电阻以及数字输入.
 * 故在退出浅睡眠后需要重新配置对应端口以恢复休眠前的工作状态.
 */
void sleep_init(void);

#endif //__DEV_SLEEP_STOP_H__
