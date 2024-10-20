/**
 *******************************************************************************************************************************************
 * @file        dev_tp_port.c
 * @brief       Touch Device [port]
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * 2024-08-18   lzh          modify the import method of driver 
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include "dev_tp.h"

#define __DECL_TP_NAME(__tp_name, __method_name)     __tp_name##_##__method_name

#define IMPORT_TP_DRIVER(__tp_name)                                                                                   \
    extern uint8_t __DECL_TP_NAME(__tp_name, init)(tp_desc_t *self);                                                  \
    extern uint8_t __DECL_TP_NAME(__tp_name, read_points)(const tp_desc_t *self, tp_data_t *tp_data, uint8_t points); \
    extern uint8_t __DECL_TP_NAME(__tp_name, standby)(tp_desc_t *self)

/* Touch Driver Prototype */
IMPORT_TP_DRIVER(gt9xx);
IMPORT_TP_DRIVER(ftxx);
IMPORT_TP_DRIVER(cst8xx);
IMPORT_TP_DRIVER(cst3xx);
IMPORT_TP_DRIVER(ili2117a);
IMPORT_TP_DRIVER(tango_c32);
IMPORT_TP_DRIVER(chsc6413);
IMPORT_TP_DRIVER(chsc6540);

#define DECL_TP_DRIVER_BASE(__tp_name)          \
        __DECL_TP_NAME(__tp_name, init),        \
        __DECL_TP_NAME(__tp_name, read_points), \
        NULL

#define DECL_TP_DRIVER_ALL(__tp_name)           \
        __DECL_TP_NAME(__tp_name, init),        \
        __DECL_TP_NAME(__tp_name, read_points), \
        __DECL_TP_NAME(__tp_name, standby)

static const struct {
    const char *name;
    tp_adapter_t adapter;
} TP_Table[] = {
//    {"CHSC6413", DECL_TP_DRIVER_BASE(chsc6413)},
//    {"CHSC6540", DECL_TP_DRIVER_BASE(chsc6540)},
    
    {"GT911", DECL_TP_DRIVER_BASE(gt9xx)},
//    {"GT9147", DECL_TP_DRIVER_BASE(gt9xx)},
    
//    {"CST328", DECL_TP_DRIVER_BASE(cst3xx)},
//    {"CST3240", DECL_TP_DRIVER_BASE(cst3xx)},
    
//    {"CST826", DECL_TP_DRIVER_BASE(cst8xx)},
    {"CST816D", DECL_TP_DRIVER_BASE(cst8xx)},
    
//    {"FT6336", DECL_TP_DRIVER_BASE(ftxx)},
//    {"FT5206", DECL_TP_DRIVER_BASE(ftxx)},
//    {"FT5426", DECL_TP_DRIVER_BASE(ftxx)},
    
//    {"ILI2117A", DECL_TP_DRIVER_BASE(ili2117a)},
//    {"Tango_C32", DECL_TP_DRIVER_BASE(tango_c32)},
};

/**
 * @brief   TP 驱动适配器
 * @param   self : 实例
 * @param   name : TP 模组型号名
 * @retval  0     : success
 * @retval  other : error code
 */
uint8_t tp_adapter(tp_adapter_t *self, const char *name)
{
    uint16_t i = 0;
    for (i = 0; i < sizeof(TP_Table) / sizeof(TP_Table[0]); ++i) {
        if (0 == strcmp(name, TP_Table[i].name)) {
            break;
        }
    }
    if (i >= sizeof(TP_Table) / sizeof(TP_Table[0])) {
        printf("[%s]: No find [%s]TP Driver of correct, You must provide your customized TP Driver!\r\n", __FUNCTION__, name);
        return 1;
    }
    if (!(self && TP_Table[i].adapter.init && TP_Table[i].adapter.read_points)) {
        printf("[%s]: tp_desc or TP_Table[%d] callback is NULL, You must provide [%s]TP Driver callback!\r\n", __FUNCTION__, i, name);
        return 2;
    }
    memcpy(self, &TP_Table[i].adapter, sizeof(tp_adapter_t));
    return 0;
}
