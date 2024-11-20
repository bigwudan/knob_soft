/**
 *******************************************************************************************************************************************
 * @file        dev_qspi_flash.h
 * @brief       Key Device
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-08-18   lzh          the first version
 * 2024-10-10   lzh          fix QSPI_line-4 DMA read
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __DEV_QSPI_FLASH_H__
#define __DEV_QSPI_FLASH_H__

#include <stdint.h>

void flash_qspi_port_init(void);
void flash_qspi_port_deinit(void);
void qspi_flash_init(void);
void qspi_dma_read(uint32_t addr, void *data, uint32_t cnt, uint8_t addr_width, uint8_t data_width);

#endif /* __DEV_QSPI_FLASH_H__ */
