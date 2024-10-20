/**
 *******************************************************************************************************************************************
 * @file        dev_lcd_mpu_i80.c
 * @brief       LCD MPU display [i8080]
 * @since       Change Logs:
 * Date         Author       Notes
 * 2024-02-18   lzh          the first version
 * 2024-08-18   lzh          refactor [wr_cmd / wr_data / rd_data]
 *******************************************************************************************************************************************
 * @attention   THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
 * REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
 * FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
 * -ECTION WITH THEIR PRODUCTS.
 * @copyright   2012 Synwit Technology
 *******************************************************************************************************************************************
 */
#include "dev_lcd_mpu.h"
#include "SWM221.h"

#if 1 /* 调试打印 */
#include <stdio.h>
#define LCD_LOG(...)             printf(__VA_ARGS__)
#else
#define LCD_LOG(...)
#endif
/*******************************************************************************************************************************************
 * static defines
 *******************************************************************************************************************************************/
#define lcd_dma_xfer_color_16(data, pixels)             lcd_dma_xfer_i80(data, pixels, 0, 0, 16)
#define lcd_dma_xfer_bitmap_16(data, pixels)            lcd_dma_xfer_i80(data, pixels, 0, 1, 16)
#define lcd_dma_xfer_bitmap_async_16(data, pixels)      lcd_dma_xfer_i80(data, pixels, 1, 1, 16)

/* 221 的 DMA 单次传输最大支持 65535 个 Unit */
#define DMA_SINGLE_CNT_MAX    ( 0xFFFF )

static uint8_t lcd_dma_xfer_i80(void *data, uint32_t pixels, uint8_t async, uint8_t src_inc, uint8_t unit_bit)
{
    if (0 != src_inc && 16 == unit_bit && ((uint32_t)data & (2 - 1))) {
        return 1;
    }
    DMA_InitStructure DMA_initStruct;
    DMA_initStruct.Mode = DMA_MODE_SINGLE;
    DMA_initStruct.Unit = (16 == unit_bit) ? DMA_UNIT_HALFWORD : DMA_UNIT_BYTE;
    DMA_initStruct.Count = DMA_SINGLE_CNT_MAX;
    DMA_initStruct.MemoryAddr = (uint32_t)data;
    DMA_initStruct.MemoryAddrInc = (src_inc) ? 1 : 0;
    /* 221 MPU 模块可以接收 8 / 16bit 输入, 以 8bit 模式输出, 字节序以 MPU 初始化 ByteOrder 成员为准 */
    DMA_initStruct.PeripheralAddr = (16 == unit_bit) ? ((uint32_t)&MPU->DRH) : ((uint32_t)&MPU->DRB);
    DMA_initStruct.PeripheralAddrInc = 0;
    DMA_initStruct.Handshake = DMA_CH1_MPUTX;
    DMA_initStruct.Priority = DMA_PRI_LOW;
    DMA_initStruct.INTEn = 0; /* 失能中断 */
    DMA_CH_Init(DMA_CH1, &DMA_initStruct);
    
    MPU->SR |= MPU_SR_DMAEN_Msk;
    MPU->IRB = 0x2C; // write_pixels_cmd
    
    /* DMA Sync */
    uint32_t data_addr = (uint32_t)data;
    while (pixels > DMA_SINGLE_CNT_MAX)
    {
        uint32_t single_count = (pixels > DMA_SINGLE_CNT_MAX) ? DMA_SINGLE_CNT_MAX : pixels;

        DMA_CH_SetAddrAndCount(DMA_CH1, data_addr, single_count);
        DMA_CH_Open(DMA_CH1);

        while (0 == DMA_CH_INTStat(DMA_CH1, DMA_IT_DONE)) __NOP();
        DMA_CH_INTClr(DMA_CH1, DMA_IT_DONE);

        pixels -= single_count;
        data_addr += (src_inc) ? (single_count << ((16 == unit_bit) ? 1 : 0)) : 0;
    }
    /* DMA last xfer */
    DMA_CH_SetAddrAndCount(DMA_CH1, data_addr, pixels);
    if (0 != async) {
        NVIC_EnableIRQ(GPIOB1_GPIOA9_DMA_IRQn); /* DMA_CH_INTEn() 内部实现未操作 NVIC, 须搭配 NVIC_EnableIRQ 使用 */
        DMA_CH_INTEn(DMA_CH1, DMA_IT_DONE);
    }
    DMA_CH_Open(DMA_CH1);

    if (0 == async) {
        while (0 == DMA_CH_INTStat(DMA_CH1, DMA_IT_DONE)) __NOP();
        DMA_CH_INTClr(DMA_CH1, DMA_IT_DONE);

        MPU->SR &= ~MPU_SR_DMAEN_Msk; // 完成 DMA 操作后要清除 DMA 使能
    }
    return 0;
}
/*******************************************************************************************************************************************
 * public functions
 *******************************************************************************************************************************************/
void __DECL_LCD_NAME(peripheral_init, i80_8)(void)
{
    MPU_InitStructure MPU_initStruct;
#if 0 //屏参极限值
    MPU_initStruct.RDHoldTime = 32;
    MPU_initStruct.WRHoldTime = 16;
    MPU_initStruct.CSFall_WRFall = 4;
    MPU_initStruct.WRRise_CSRise = 4;
    MPU_initStruct.RDCSRise_Fall = 32;
    MPU_initStruct.WRCSRise_Fall = 16;
#endif
    // 延时越小, 传输速率越高, 请根据屏幕规格书容忍范围酌情设定
	MPU_initStruct.RDHoldTime = 1;
	MPU_initStruct.WRHoldTime = 1;
	MPU_initStruct.CSFall_WRFall = 1;
	MPU_initStruct.WRRise_CSRise = 1;
	MPU_initStruct.RDCSRise_Fall = 1;
	MPU_initStruct.WRCSRise_Fall = 1;

    MPU_initStruct.ByteOrder = MPU_BIG_ENDIAN;
    MPU_Init(MPU, &MPU_initStruct);
}

void __DECL_LCD_NAME(peripheral_deinit, i80_8)(void)
{
//    SYS->CLKEN0 &= ~(0x01 << SYS_CLKEN0_MPU_Pos);
//    __NOP();__NOP();__NOP();
}

void __DECL_LCD_NAME(port_init, i80_8)(void)
{
    /* 221 MPU 端口是固定的, 不可变更 */
	PORT_Init(PORTA, PIN8,  PORTA_PIN8_MPU_D0,  1);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_MPU_D1,  1);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_MPU_D2, 1);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_MPU_D3, 1);
	PORT_Init(PORTA, PIN12, PORTA_PIN12_MPU_D4, 1);
	PORT_Init(PORTA, PIN13, PORTA_PIN13_MPU_D5, 1);
	PORT_Init(PORTA, PIN14, PORTA_PIN14_MPU_D6, 1);
	PORT_Init(PORTA, PIN0,  PORTA_PIN0_MPU_D7,  1);

	PORT_Init(PORTB, PIN4,  PORTB_PIN4_MPU_CS,  0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_MPU_RS,  0);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_MPU_WR,  0);
    /* 没有用到"读"指令/数据时, RD 保持一直为高(可在电路上直连 VCC 省一个IO) */
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_MPU_RD,  0);
}

void __DECL_LCD_NAME(port_deinit, i80_8)(void)
{
    /* 将已使用的端口映射为普通 GPIO 功能, 配置为输入 & 无上拉 & 无下拉, 并关闭数字输入使能. */
    GPIO_INIT(GPIOB, PIN4, GPIO_OUTPUT); // 片选必须拉高
    GPIO_AtomicSetBit(GPIOB, PIN4);
    
    GPIO_INIT(GPIOA, PIN8, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN9, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN10, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN11, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN12, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN13, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN14, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN0, GPIO_INPUT);

    GPIO_INIT(GPIOB, PIN5, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN6, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN9, GPIO_INPUT);
}

/**
 * @brief  画点
 * @param  color : 色彩(only support RGB565)
 * @retval \
 */
void __DECL_LCD_NAME(draw_point, i80_8)(uint32_t color)
{
    MPU_WR_DATA16(MPU, color & 0xFFFF);
}

/**
 * @brief  填充纯色
 * @param  color  : 色彩(only support RGB565)
 * @param  pixels : 像素个数
 * @retval \
 */
void __DECL_LCD_NAME(fill_color, i80_8)(uint32_t color, uint32_t pixels)
{
    if (0 != lcd_dma_xfer_color_16(&color, pixels))
    {
        for (uint32_t i = 0; i < pixels; ++i)
        {
            MPU_WR_DATA16(MPU, color & 0xFFFF);
        }
    }
}

/**
 * @brief  绘图
 * @param  data   : 图像数据源(only support RGB565, 16bit alignment)
 * @param  pixels : 像素个数
 * @retval \
 */
void __DECL_LCD_NAME(flush_bitmap, i80_8)(void *data, uint32_t pixels)
{
    if (!(data && 0 == ((uint32_t)data & 0x01) && pixels))
    {
        LCD_LOG("[%s] param [%p][%d].\r\n", __FUNCTION__, data, pixels);
        return;
    }
    if (0 != lcd_dma_xfer_bitmap_16(data, pixels))
    {
        uint16_t *p = (uint16_t *)data; /* support RGB565 */
        for (uint32_t i = 0; i < pixels; ++i)
        {
            MPU_WR_DATA16(MPU, p[i]);
        }
    }
}

/*******************************************************************************************************************************************
 * public extra functions
 *******************************************************************************************************************************************/
/**
 * @brief  异步绘图
 * @param  data : 数据源(only support RGB565, 16bit alignment)
 * @param  pixels  : 像素个数
 * @retval 0: async vaild   other: async invaild
 */
uint8_t __DECL_LCD_NAME(flush_bitmap_async, i80_8)(void *data, uint32_t pixels)
{
    if (!(data && 0 == ((uint32_t)data & 0x01) && pixels))
    {
        LCD_LOG("[%s] param [%p][%d].\r\n", __FUNCTION__, data, pixels);
        return 1;
    }
    return lcd_dma_xfer_bitmap_async_16(data, pixels);
}

/**
 * @brief  异步画图完成就绪回调
 * @param  \
 * @retval 0: transfer completed   other: transfer busy
 */
uint8_t __DECL_LCD_NAME(flush_bitmap_async_done, i80_8)(void)
{
    if (DMA_CH_INTStat(DMA_CH1, DMA_IT_DONE))
    {
        DMA_CH_INTClr(DMA_CH1, DMA_IT_DONE);

        MPU->SR &= ~MPU_SR_DMAEN_Msk; // 完成 DMA 操作后要清除 DMA 使能

        return 0;
    }
    return 1;
}

/*******************************************************************************************************************************************
 * public base functions
 *******************************************************************************************************************************************/
void __DECL_LCD_NAME(wr_cmd, i80_8)(uint32_t cmd, void *seq)
{
    MPU_WR_REG8(MPU, cmd & 0xFF);
}

void __DECL_LCD_NAME(wr_data, i80_8)(uint8_t *data, uint32_t count)
{
    for (uint32_t i = 0; i < (data ? count : 0); ++i)
    {
        MPU_WR_DATA8(MPU, data[i]);
    }
}

void __DECL_LCD_NAME(rd_data, i80_8)(uint8_t *data, uint32_t count)
{
    for (uint32_t i = 0; i < (data ? count : 0); ++i)
    {
        while (MPU->SR & MPU_SR_BUSY_Msk) __NOP();
        data[i] = MPU->DRB;
    }
}