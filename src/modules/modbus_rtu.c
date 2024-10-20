/**
 *******************************************************************************************************************************************
 * @file        modbus_rtu.c
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
#include <string.h>
#include "modbus_rtu.h"

/*******************************************************************************************************************************************
 * modbus address
 *******************************************************************************************************************************************/
#define MB_ADDR_BROADCAST    ( 0 )   /**< Modbus broadcast address. */
#define MB_ADDR_MIN          ( 1 )   /**< Smallest possible slave address. */
#define MB_ADDR_MAX          ( 247 ) /**< Biggest possible slave address. */

/*******************************************************************************************************************************************
 * modbus function code
 *******************************************************************************************************************************************/
#define MB_FUNC_NONE                          (  0 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )
#define MB_FUNC_DIAG_READ_EXCEPTION           (  7 )
#define MB_FUNC_DIAG_DIAGNOSTIC               (  8 )
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        ( 11 )
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        ( 12 )
#define MB_FUNC_OTHER_REPORT_SLAVEID          ( 17 )
#define MB_FUNC_ERROR                         ( 128 )

/*******************************************************************************************************************************************
 * static prototypes
 *******************************************************************************************************************************************/
static uint8_t mb_slave_parser_adu(modbus_session_t *session, uint8_t *buff, uint32_t len);
static uint32_t mb_slave_reply_adu(modbus_session_t *session, uint8_t *buff);
static uint16_t mb_crc16(uint8_t *data, uint32_t len);

/*******************************************************************************************************************************************
 * public function
 *******************************************************************************************************************************************/
/**
 * @brief Modbus 会话初始化
 */
modbus_sta_t modbus_session_init(modbus_session_t *session, modbus_cfg_t *cfg)
{
    //MB_ASSERT(!session);
    if (cfg)
    {
        memcpy(&session->cfg, cfg, sizeof(modbus_cfg_t));
    }
    if (!(session->cfg.rx_data && session->cfg.tx_data && session->cfg.rx_timeout))
    {
        return MB_STA_INVALID_DATA;
    }
    return MB_STA_OK;
}

/**
 * @brief Modbus 消息处理
 */
modbus_sta_t modbus_handler(modbus_session_t *session, uint8_t *buff, uint8_t *size)
{
    //MB_ASSERT(!(session && buff && len));
    uint8_t adu[MB_ADU_MAX_SIZE] = {0};
    uint32_t len = 0;

    *size = 0;

    /* RX-Timeout */
    uint8_t rx_timeout_flag = session->cfg.rx_timeout();
    if (0 != rx_timeout_flag)
    {
        return rx_timeout_flag; //MB_STA_BUSY  MB_STA_NO_DATA
    }
    /* Read RX-FIFO */
    len = session->cfg.rx_data(adu, sizeof(adu));
    if (len <= 4) {
        //addr + funcode + user_data + crc16 > 4Bytes
        return MB_STA_INVALID_DATA;
    }
    /* CRC16 check */
    if (0 != mb_slave_parser_adu(session, adu, len))
    {
        session->extra_synwit.pkt_id += 1;
        return MB_STA_ERROR_CRC16;
    }
    /* 示例地址配合 PC 的 Synwit 上位机使用, 如仅演示 Demo 请不要改动地址 */
    #define MODBUS_SERVER_ADDR       ( 0x55 )
    /* Addr check */
    if ((MODBUS_SERVER_ADDR != session->addr) || (session->addr > MB_ADDR_MAX))
    {
        return MB_STA_ERROR_ADDR;
    }

    /* 上位机自定义通讯帧解析 */
    if (0x9000 == ((adu[2] << 8) | adu[3])) /* 上位机自定义的指令, 代表这是一个固件更新包 */
    {
        uint32_t offset = 7; /* 上位机自定义的数据偏移 */
        uint16_t type = (adu[offset] << 8) | adu[offset + 1];
        offset += 2;
        if (1 == type) /* 上位机自定义的指令, 代表这是程序内容 */
        {
            /* 上位机自定义每次传输一包数据实际有效 128 字节 */
            session->extra_synwit.pkt_id = (adu[offset] << 24) | (adu[offset + 1] << 16) | (adu[offset + 2] << 8) | adu[offset + 3];
            session->extra_synwit.pkt_unit = 128;
            offset += 4;

            *size = session->extra_synwit.pkt_unit;
            memcpy(buff, &adu[offset], *size);
        }
        /* slave_respone test */
        session->fun_code = 0x10; // 强制回复 0x10 测试 (MB_FUNC_WRITE_MULTIPLE_REGISTERS)
        session->data_len = 4; // 只回复前 4 个数据
        
        uint32_t len = mb_slave_reply_adu(session, adu);
        uint32_t retry = 0;
        for (uint32_t i = 0; i < len && retry < 3; ++retry)
        {
            i += session->cfg.tx_data(&adu[i], len - i);
        }
        /* wait for TX-FIFO to complete sending */
        if (session->cfg.tx_done && 0 != session->cfg.tx_done()) {
            return MB_STA_TIMEOUT;
        }
        return MB_STA_OK;
    }
    return MB_STA_INVALID_DATA;
}

/*******************************************************************************************************************************************
 * static function
 *******************************************************************************************************************************************/
/* MODBUS 解析 ADU */
static uint8_t mb_slave_parser_adu(modbus_session_t *session, uint8_t *buff, uint32_t len)
{
    /* len > 4 */
    if (((buff[len - 1] << 8) | buff[len - 2]) != mb_crc16(buff, len - 2))
    {
        return 1;
    }
    session->addr = buff[0];
    session->fun_code = buff[1];
    session->data = &buff[2];
    session->data_len = len - 2;
    return 0;
}

/* MODBUS 回应 ADU */
static uint32_t mb_slave_reply_adu(modbus_session_t *session, uint8_t *buff)
{
    uint8_t adu[MB_ADU_MAX_SIZE] = {0};

    uint32_t len = 2;

    switch (session->fun_code)
    {
    case (MB_FUNC_READ_COILS):            /* 读线圈状态 - bit */
    case (MB_FUNC_READ_DISCRETE_INPUTS):  /* 读输入状态 - bit */
    case (MB_FUNC_READ_HOLDING_REGISTER): /* 读保持寄存器 */
    case (MB_FUNC_READ_INPUT_REGISTER):   /* 读输入寄存器 */
        adu[len++] = session->data_len;
    break;

    case (MB_FUNC_WRITE_SINGLE_COIL):        /* 写单个线圈 - bit */
    case (MB_FUNC_WRITE_REGISTER):           /* 写单个保持寄存器 */
    case (MB_FUNC_WRITE_MULTIPLE_COILS):     /* 写多个线圈 - bit */
    case (MB_FUNC_WRITE_MULTIPLE_REGISTERS): /* 写多个保持寄存器 */
        /* 回复与主机下发数据相同 */
    break;

    default: return 0; //other ignore
    }
    /* header */
    adu[0] = session->addr;
    adu[1] = session->fun_code;

    /* data */
    memcpy(&adu[len], session->data, session->data_len);
    len += session->data_len;

    /* CRC16 */
    uint16_t val_crc16 = mb_crc16(adu, len);
    adu[len++] = val_crc16 & 0xFF;
    adu[len++] = (val_crc16 >> 8) & 0xFF;

    memcpy(buff, adu, len);
    return len;
}

#if 1 /* 分治 */
//use __builtin_arm ? __REV(n)  __REVSH(n)  __RBIT(n)
static uint32_t reverse_bits(uint32_t n, uint8_t bit_len)
{
    switch (bit_len)
    {
        case (32):
            n = (((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1));
            n = (((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2));
            n = (((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4));
            n = (((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8));
            n = ((n >> 16) | (n << 16));
        break;

        case (16):
            n = ((n << 4) & 0xF0FF) | ((n >> 4) & 0xFF0F);
            n = ((n << 2) & 0xCCCC) | ((n >> 2) & 0x3333);
            n = ((n << 1) & 0xAAAA) | ((n >> 1) & 0x5555);
            n = (n << 8) | (n >> 8);
        break;

        case (8):
            n = ((n << 2) & 0xcc) | ((n >> 2) & 0x33);
            n = ((n << 1) & 0xaa) | ((n >> 1) & 0x55);
            n = (n << 4) | (n >> 4);
        break;

        default : break;
    }
    return n;
}
#else /* 轮询 */
static uint32_t reverse_bits(uint32_t n, uint8_t bit_len)
{
    uint32_t rev = 0;
    for (uint8_t i = 0; i < bit_len && n > 0; ++i)
    {
        rev |= (n & 1) << (bit_len - 1 - i);
        n >>= 1;
    }
    return rev;
}
#endif

/* 交换比特序 */
#define SWAP_BIT(VAL, BIT_LEN)    reverse_bits(VAL, BIT_LEN)

static uint16_t mb_crc16(uint8_t *data, uint32_t len)
{
#define INIT      (0xFFFF)
#define POLY      (0x8005)
#define REF_IN    (1)
#define REF_OUT   (1)
#define XOR_OUT   (0)

    uint16_t val = INIT;
    uint8_t c = 0;
    for (uint32_t i = 0; i < len; ++i)
    {
#if (REF_IN)
        c = SWAP_BIT(data[i], 8);
#else
        c = data[i];
#endif
        val ^= (c << 8);
        for (uint8_t bit = 0; bit < 8; ++bit)
        {
            if (val & 0x8000)
            {
                val = (val << 1) ^ POLY;
            }
            else
            {
                val <<= 1;
            }
        }
    }
#if (REF_OUT)
    val = SWAP_BIT(val, 16);
#endif
#if (XOR_OUT)
    //TODO
#endif
    return val;
}
