/**
 *******************************************************************************************************************************************
 * @file        dev_tp_private.c
 * @brief       实现电容触摸驱动所需的私有函数
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
#ifndef __DEV_TP_PRIVATE_H__
#define __DEV_TP_PRIVATE_H__

#include <stdint.h>
#include <cmsis_compiler.h>
#include "dev_tp.h"

#define TP_I2C_TIMEOUT_DEFAULT       (0) /* 0: forever      >0: retry_times */

#define TP_PRIVATE_WR_REG_32(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)      static_tp_write_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 32)
#define TP_PRIVATE_RD_REG_32(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)      static_tp_read_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 32)
#define TP_PRIVATE_WR_REG_24(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)      static_tp_write_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 24)
#define TP_PRIVATE_RD_REG_24(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)      static_tp_read_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 24)
#define TP_PRIVATE_WR_REG_16(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)      static_tp_write_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 16)
#define TP_PRIVATE_RD_REG_16(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)      static_tp_read_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 16)
#define TP_PRIVATE_WR_REG_8(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)       static_tp_write_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 8)
#define TP_PRIVATE_RD_REG_8(reg, buf, len, tp_desc, i2c_addr, i2c_timeout)       static_tp_read_regs_xbit(reg, buf, len, tp_desc, i2c_addr, i2c_timeout, 8)

static void static_tp_i2c_delay(void)
{
    extern uint32_t SystemCoreClock;
    for (uint32_t i = 0; i < SystemCoreClock / (1 * 1000 * 1000); ++i) __NOP(); // 1us
}

static uint8_t static_tp_write_regs_xbit(uint32_t reg, uint8_t *buf, uint32_t len,
                                const tp_desc_t *self, uint16_t i2c_addr, uint16_t i2c_timeout, uint8_t bit)
{
    uint32_t i = 0;
    uint8_t ack = 0;
    ack = self->board.i2c_ops.start(i2c_addr | 0, 0, i2c_timeout);
    if (0 != ack) {
        goto wr_end;
    }

    if (bit >= 32) {
        ack = self->board.i2c_ops.write((reg >> 24) & 0xFF, i2c_timeout);
        if (0 != ack) {
            goto wr_end;
        }
    }
    if (bit >= 24) {
        ack = self->board.i2c_ops.write((reg >> 16) & 0xFF, i2c_timeout);
        if (0 != ack) {
            goto wr_end;
        }
    }
    if (bit >= 16) {
        ack = self->board.i2c_ops.write((reg >> 8) & 0xFF, i2c_timeout);
        if (0 != ack) {
            goto wr_end;
        }
    }
    ack = self->board.i2c_ops.write((reg >> 0) & 0xFF, i2c_timeout);
    if (0 != ack) {
        goto wr_end;
    }

    for (i = 0; i < len; ++i)
    {
        ack = self->board.i2c_ops.write(buf[i], i2c_timeout);
        if (0 != ack) {
            goto wr_end;
        }
    }

wr_end:
    ack = self->board.i2c_ops.stop(i2c_timeout);
    if (ack != 0) {
        // log_error
    }
    static_tp_i2c_delay();
    return ack;
}

static uint8_t static_tp_read_regs_xbit(uint32_t reg, uint8_t *buf, uint32_t len,
                                const tp_desc_t *self, uint16_t i2c_addr, uint16_t i2c_timeout, uint8_t bit)
{
    uint32_t i = 0;
    uint8_t ack = 0;
    ack = self->board.i2c_ops.start(i2c_addr | 0, 0, i2c_timeout);
    if (0 != ack) {
        goto rd_end;
    }

    if (bit >= 32) {
        ack = self->board.i2c_ops.write((reg >> 24) & 0xFF, i2c_timeout);
        if (0 != ack) {
            goto rd_end;
        }
    }
    if (bit >= 24) {
        ack = self->board.i2c_ops.write((reg >> 16) & 0xFF, i2c_timeout);
        if (0 != ack) {
            goto rd_end;
        }
    }
    if (bit >= 16) {
        ack = self->board.i2c_ops.write((reg >> 8) & 0xFF, i2c_timeout);
        if (0 != ack) {
            goto rd_end;
        }
    }
    ack = self->board.i2c_ops.write((reg >> 0) & 0xFF, i2c_timeout);
    if (0 != ack) {
        goto rd_end;
    }

    static_tp_i2c_delay(); // ReStart
    
    ack = self->board.i2c_ops.start(i2c_addr | 1, 1, i2c_timeout);
    if (0 != ack){
        goto rd_end;
    }

    for (i = 0; i < len - 1; ++i)
    {
        buf[i] = self->board.i2c_ops.read(1, i2c_timeout);
    }
    buf[i] = self->board.i2c_ops.read(0, i2c_timeout);

rd_end:
    ack = self->board.i2c_ops.stop(i2c_timeout);
    if (ack != 0) {
        // log_error
    }
    static_tp_i2c_delay();
    return ack;
}

#endif //__DEV_TP_PRIVATE_H__
