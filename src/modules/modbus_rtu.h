/**
 *******************************************************************************************************************************************
 * @file        modbus_rtu.h
 * @brief       Modbus RTU Server
 * @since       Change Logs:
 * Date         Author       Notes
 * 2022-11-06   xjh          the first version
 * 2024-04-23   lzh          optimize code
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__

#include <stdint.h>

#define MB_PDU_MAX_SIZE      ( 253 )

/* RS232/RS485 ADU: 253 bytes + Server address(1 byte) + CRC (2 bytes) = 256 bytes
 * TCP MODBUS ADU: 253 bytes + MBAP (7 bytes) = 260 bytes
 */
#define MB_ADU_MAX_SIZE      ( 256 )

/** enum Modbus session state */
enum
{
    MB_STA_OK = 0,           /**< normal return */
    MB_STA_BUSY,             /**< busy, pkt_data no ready */
    MB_STA_NO_DATA,          /**< no data */
    MB_STA_TIMEOUT,          /**< communication timeout */
    MB_STA_INVALID_DATA,     /**< invalid data */
    MB_STA_ERROR_ADDR,       /**< addr error */
    MB_STA_ERROR_CRC16,      /**< CRC16 error */
    MB_STA_ERROR_RETRANS,    /**< retrans over max-times */
    MB_STA_ERROR_NO_SUPPORT, /**< no support */
    MB_STA_ERROR_UNKNOWN,    /**< unknown error */
    __MB_STA_MAX__ = 0xFF,   /**< modbus sta max type */
};
typedef uint8_t modbus_sta_t;

/** Modbus operations */
typedef struct modbus_cfg_t modbus_cfg_t;
struct modbus_cfg_t
{
    /**
     * @brief   接收数据
     * @param   buff: 消息缓冲区
     * @param   len: 缓冲区长度上限 / Bytes
     * @retval  实际接收到的数据长度(不会超过缓冲区长度上限)
     */
    uint32_t (*rx_data)(uint8_t *buff, uint32_t len);

    /**
     * @brief   发送数据
     * @param   buff: 消息缓冲区
     * @param   len: 数据长度 / Bytes
     * @retval  将发送的数据长度, 如果传入 len != 0 却返回 0, 代表发生硬件超时错误
     */
    uint32_t (*tx_data)(const uint8_t *buff, uint32_t len);

    /**
     * @brief   接收数据是否超时
     * @param   \
     * @retval  MB_STA_OK: timeout && data ready    MB_STA_BUSY: rx data busy    MB_STA_NO_DATA: timeout && no data
     */
    uint8_t (*rx_timeout)(void);

    /**
     * @brief   发送数据是否完成(optional)
     * @param   \
     * @retval  0: success  other: error code
     */
    uint8_t (*tx_done)(void);
};

/** Modbus extra expansion of synwit */
typedef struct modbus_extra_synwit_t modbus_extra_synwit_t;
struct modbus_extra_synwit_t
{
    uint32_t pkt_id;
    uint32_t pkt_unit;
};

/** Modbus session control struct */
typedef struct modbus_session
{
    modbus_cfg_t cfg;

    uint8_t addr;
    uint8_t fun_code;
    uint8_t *data;
    uint8_t data_len;

    modbus_extra_synwit_t extra_synwit;
} modbus_session_t;

/**
 * @brief Modbus 会话初始化
 */
modbus_sta_t modbus_session_init(modbus_session_t *session, modbus_cfg_t *cfg);

/**
 * @brief Modbus 消息处理
 */
modbus_sta_t modbus_handler(modbus_session_t *session, uint8_t *buff, uint8_t *size);

#endif // __MODBUS_RTU_H__
