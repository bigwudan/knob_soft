/**
 *******************************************************************************************************************************************
 * @file        dev_lcd_mpu_qspi.h
 * @brief       LCD MPU QSPI expand
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-08-18   lzh          the first version
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __DEV_LCD_MPU_QSPI_H__
#define __DEV_LCD_MPU_QSPI_H__

#include <stdint.h>

// LCD QSPI expand
typedef struct lcd_qspi_seq_t
{
    uint8_t instruct;      // 指令码
    uint8_t instruct_line; // 可取值: 0 / 1 / 2 / 4 line

    uint8_t addr_line; // 可取值: 0 / 1 / 2 / 4 line
    uint8_t addr_bits; // 可取值: 8 / 16 / 24 / 32 bit
    uint32_t addr;     // 地址码

    uint32_t alternate_bytes;    // 交替字节码
    uint8_t alternate_byte_line; // 可取值: 0 / 1 / 2 / 4 line
    uint8_t alternate_byte_bits; // 可取值: 8 / 16 / 24 / 32 bit
    
    uint8_t dummy_cycles;        // dummy_cycles 可取值: 0 -- 31

    uint8_t data_line;   // 可取值: 0 / 1 / 2 / 4 line
    uint32_t data_count; // 读写数据的字节个数

    uint8_t rw_mode; // [0: Write      1: Read      2: AutoPolling]

    uint8_t send_ins_only_once; // reserve: Send Instruction Only Once
} lcd_qspi_seq_t;

static inline
void __LCD_QSPI_SEQ_SET(lcd_qspi_seq_t *seq, 
                        uint8_t ins, uint8_t ins_line, 
                        uint32_t addrs, uint8_t addrline, uint8_t addrbits, 
                        uint32_t alt, uint8_t alt_line, uint8_t alt_bits, 
                        uint32_t dummycycles, 
                        uint8_t dataline, uint32_t datacount, uint8_t rw)
{
    seq->instruct = ins;
    seq->instruct_line = ins_line;
    seq->addr_line = addrline;
    seq->addr_bits = addrbits;
    seq->addr = addrs;
    seq->alternate_bytes = alt;
    seq->alternate_byte_line = alt_line;
    seq->alternate_byte_bits = alt_bits;
    seq->dummy_cycles = dummycycles;
    seq->data_line = dataline;
    seq->data_count = datacount;
    seq->rw_mode = rw;
}

#define LCD_QSPI_SEQ_SET_WR(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount) \
    __LCD_QSPI_SEQ_SET(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount, 0)

#define LCD_QSPI_SEQ_SET_RD(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount) \
    __LCD_QSPI_SEQ_SET(seq, ins, ins_line, addrs, addrline, addrbits, alt, alt_line, alt_bits, dummycycles, dataline, datacount, 1)

#endif //__DEV_LCD_MPU_QSPI_H__
