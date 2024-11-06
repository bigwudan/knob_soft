/**
 *******************************************************************************************************************************************
 * @file        dev_lcd_mpu_spi.c
 * @brief       LCD MPU display [QSPI]
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
#include "dev_lcd_mpu.h"
#include "dev_lcd_mpu_qspi.h"
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
#define LCD_QSPI_X             QSPI0

/** LCD_QSPI TX DMA */
#define LCD_QSPI_TX_DMACHN     DMA_CH0
#define LCD_QSPI_DMA_TX_TRIG   DMA_CH0##_##QSPI0##TX
/* 221 的 DMA 单次传输最大支持 65535 个 Unit */
#define DMA_SINGLE_CNT_MAX    ( 0xFFFF )

#define qspi_dma_xfer_color(data, pixels)            qspi_dma_xfer(data, pixels, 0, 0)
#define qspi_dma_xfer_bitmap(data, pixels)           qspi_dma_xfer(data, pixels, 0, 1)
/* 由于 QSPI 只有一组, 须分时复用 QSPI 屏驱与 QSPI 读 Flash, 不推荐用异步 */
#define qspi_dma_xfer_bitmap_async(data, pixels)     qspi_dma_xfer(data, pixels, 1, 1)

static uint8_t qspi_dma_xfer(void *data, uint32_t pixels, uint8_t async, uint8_t src_inc)
{
    //Reverse byte order [GB RG -> RG GB]
    uint16_t *p = data;
    for (uint32_t i = 0; i < ( (src_inc) ? pixels : 1 ); ++i) {
        p[i] = __REVSH(p[i]);
    }
    
    uint8_t unit_step = 1; //1: half_word   2: word

    DMA_InitStructure DMA_initStruct;
    DMA_initStruct.Mode = DMA_MODE_SINGLE;
    DMA_initStruct.Unit = DMA_UNIT_HALFWORD; /* RGB565 */
    DMA_initStruct.Count = DMA_SINGLE_CNT_MAX;
    DMA_initStruct.MemoryAddr = (uint32_t)data;
    DMA_initStruct.MemoryAddrInc = (src_inc) ? 1 : 0;
    DMA_initStruct.PeripheralAddr = (uint32_t)&LCD_QSPI_X->DRH;
    DMA_initStruct.PeripheralAddrInc = 0;
    DMA_initStruct.Handshake = LCD_QSPI_DMA_TX_TRIG;
    DMA_initStruct.Priority = DMA_PRI_LOW;
    DMA_initStruct.INTEn = 0; /* 失能中断 */
    
    /* 对齐时可加速 */
    if (0 == ((uint32_t)data & (4 - 1)) && 0 == (pixels & (2 - 1))) {
        /* 纯色时特殊处理 */
        if (0 == src_inc) {
            *(uint32_t *)data |= ((*(uint16_t *)data) << 16); //传入参数须为 uint32_t 
        }
        DMA_initStruct.Unit = DMA_UNIT_WORD; /* Word */
        //DMA_initStruct.Count = pixels >> 1;
        DMA_initStruct.PeripheralAddr = (uint32_t)&LCD_QSPI_X->DRW;
        
        pixels >>= 1;
        unit_step = 2;
    }

    DMA_CH_Init(LCD_QSPI_TX_DMACHN, &DMA_initStruct);

    //QSPI_DMAEnable(LCD_QSPI_X, QSPI_Mode_IndirectWrite);
    LCD_QSPI_X->CR |= QSPI_CR_DMAEN_Msk;
    
    /* DMA Sync */
    uint32_t data_addr = (uint32_t)data;
    while (pixels > DMA_SINGLE_CNT_MAX)
    {
        uint32_t single_count = (pixels > DMA_SINGLE_CNT_MAX) ? DMA_SINGLE_CNT_MAX : pixels;

        DMA_CH_SetAddrAndCount(LCD_QSPI_TX_DMACHN, data_addr, single_count);

        DMA_CH_Open(LCD_QSPI_TX_DMACHN);

        while (0 == DMA_CH_INTStat(LCD_QSPI_TX_DMACHN, DMA_IT_DONE)) __NOP();
        DMA_CH_INTClr(LCD_QSPI_TX_DMACHN, DMA_IT_DONE);

        pixels -= single_count;
        data_addr += (src_inc) ? (single_count << unit_step) : 0;
    }
    /* DMA last xfer */
    DMA_CH_SetAddrAndCount(LCD_QSPI_TX_DMACHN, data_addr, pixels);
    if (0 != async) {
        NVIC_EnableIRQ(GPIOB1_GPIOA9_DMA_IRQn); /* DMA_CH_INTEn() 内部实现未操作 NVIC, 须搭配 NVIC_EnableIRQ 使用 */
        DMA_CH_INTEn(LCD_QSPI_TX_DMACHN, DMA_IT_DONE);
    }
    DMA_CH_Open(LCD_QSPI_TX_DMACHN);
    
    if (0 == async) {
        while (0 == DMA_CH_INTStat(LCD_QSPI_TX_DMACHN, DMA_IT_DONE)) __NOP();
        DMA_CH_INTClr(LCD_QSPI_TX_DMACHN, DMA_IT_DONE);
        
        //在 QSPI busy 时，写 QSPI->CR 寄存器无效
        while (QSPI_Busy(LCD_QSPI_X)) __NOP();
        
        QSPI_DMADisable(LCD_QSPI_X);
    }
    return 0;
}
/*******************************************************************************************************************************************
 * public functions
 *******************************************************************************************************************************************/
void __DECL_LCD_NAME(peripheral_init, qspi)(void)
{
    QSPI_InitStructure QSPI_initStruct;
	QSPI_initStruct.Size = QSPI_Size_16MB;
	QSPI_initStruct.ClkDiv = 2;
	QSPI_initStruct.ClkMode = QSPI_ClkMode_3;
	QSPI_initStruct.SampleShift = (2 == QSPI_initStruct.ClkDiv) ? QSPI_SampleShift_1_SYSCLK : QSPI_SampleShift_NONE;
	QSPI_initStruct.IntEn = 0;
	QSPI_Init(LCD_QSPI_X, &QSPI_initStruct);
	QSPI_Open(LCD_QSPI_X);
}

void __DECL_LCD_NAME(peripheral_deinit, qspi)(void)
{
	QSPI_Close(LCD_QSPI_X);
}

void __DECL_LCD_NAME(port_init, qspi)(void)
{
    PORT_Init(PORTA, PIN11,  PORTA_PIN11_QSPI0_SSEL,  0);

	PORT_Init(PORTC, PIN3,  PORTC_PIN3_QSPI0_SCLK,  0);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_QSPI0_MOSI, 1);
	PORT_Init(PORTB, PIN0,  PORTB_PIN0_QSPI0_MISO,  1);
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_QSPI0_D2,    1);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_QSPI0_D3,    1);
}

void __DECL_LCD_NAME(port_deinit, qspi)(void)
{
    /* 将已使用的端口映射为普通 GPIO 功能, 配置为输入 & 无上拉 & 无下拉, 并关闭数字输入使能. */
    GPIO_INIT(GPIOA, PIN11, GPIO_OUTPUT); // 片选必须拉高
    GPIO_AtomicSetBit(GPIOA, PIN11);

    /* 出于辐射考量，可以在此将以下端口设为普通 GPIO 输入 & 无上/下拉电阻 
    GPIO_INIT(GPIOC, PIN3, GPIO_INPUT);
    GPIO_INIT(GPIOA, PIN15, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN0, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN1, GPIO_INPUT);
    GPIO_INIT(GPIOB, PIN2, GPIO_INPUT);*/
}

/**
 * @brief  画点
 * @param  color : 色彩(only support RGB565)
 * @retval \
 */
void __DECL_LCD_NAME(draw_point, qspi)(uint32_t color)
{
    while (QSPI_FIFOSpace(LCD_QSPI_X) < 2) __NOP();
    
    LCD_QSPI_X->DRH = __REVSH(color);
	
	while (QSPI_Busy(LCD_QSPI_X)) __NOP();
}

/**
 * @brief  填充纯色
 * @param  color  : 色彩(only support RGB565)
 * @param  pixels : 像素个数
 * @retval \
 */
void __DECL_LCD_NAME(fill_color, qspi)(uint32_t color, uint32_t pixels)
{
    if (0 == pixels) {
        LCD_LOG("[%s] param [%d].\r\n", __FUNCTION__, pixels);
        return;
    }
    
    if (0 != qspi_dma_xfer_color(&color, pixels))
    {
        for (uint32_t i = 0; i < pixels; ++i)
        {
            while (QSPI_FIFOSpace(LCD_QSPI_X) < 2) __NOP();
            
            LCD_QSPI_X->DRH = __REVSH(color);
        }
        while (QSPI_Busy(LCD_QSPI_X)) __NOP();
    }
}

/**
 * @brief  绘图
 * @param  data   : 图像数据源(only support RGB565, 16bit alignment)
 * @param  pixels : 像素个数
 * @retval \
 */
void __DECL_LCD_NAME(flush_bitmap, qspi)(void *data, uint32_t pixels)
{
    if (!(data && 0 == ((uint32_t)data & 0x01) && pixels)) {
        LCD_LOG("[%s] param [%p][%d].\r\n", __FUNCTION__, data, pixels);
        return;
    }
    
    if (0 != qspi_dma_xfer_bitmap(data, pixels))
    {
        uint16_t *p = (uint16_t *)data;
        
        for (uint32_t i = 0; i < pixels; ++i)
        {
            while (QSPI_FIFOSpace(LCD_QSPI_X) < 2) __NOP();
            
            LCD_QSPI_X->DRH = __REVSH(p[i]);
        }
        while (QSPI_Busy(LCD_QSPI_X)) __NOP();
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
uint8_t __DECL_LCD_NAME(flush_bitmap_async, qspi)(void *data, uint32_t pixels)
{
    if (!(data && 0 == ((uint32_t)data & 0x01) && pixels)) {
        LCD_LOG("[%s] param [%p][%d].\r\n", __FUNCTION__, data, pixels);
        return 1;
    }
    if (0 != qspi_dma_xfer_bitmap_async(data, pixels)) 
    {
        return 2;
    }
    return 0; //不推荐异步, 由于 QSPI 只有一组, 须分时复用 QSPI 屏驱与 QSPI 读 Flash
}

/**
 * @brief  异步画图完成就绪回调
 * @param  \
 * @retval 0: transfer completed   other: transfer busy
 */
uint8_t __DECL_LCD_NAME(flush_bitmap_async_done, qspi)(void)
{
    if (DMA_CH_INTStat(LCD_QSPI_TX_DMACHN, DMA_IT_DONE))
    {
        DMA_CH_INTClr(LCD_QSPI_TX_DMACHN, DMA_IT_DONE);
        
        while (QSPI_Busy(LCD_QSPI_X)) __NOP();
        
        QSPI_DMADisable(LCD_QSPI_X);
        return 0;
    }
    return 1;
}

/*******************************************************************************************************************************************
 * public base functions
 *******************************************************************************************************************************************/
static void qspi_conver_std_peripheral(QSPI_CmdStructure *cmdStruct, lcd_qspi_seq_t *seq);

void __DECL_LCD_NAME(wr_cmd, qspi)(uint32_t cmd, void *seq)
{
    QSPI_CmdStructure cmdStruct;
    QSPI_CmdStructClear(&cmdStruct);
    
    lcd_qspi_seq_t *qspi_seq = seq;
    qspi_conver_std_peripheral(&cmdStruct, qspi_seq);

    uint8_t cmdMode = (0 == qspi_seq->rw_mode) ? QSPI_Mode_IndirectWrite : 
                      ((1 == qspi_seq->rw_mode) ? QSPI_Mode_IndirectRead : QSPI_Mode_AutoPolling);
    
    // 有数据阶段
    if (QSPI_PhaseMode_None != cmdStruct.DataMode) {
        if (QSPI_Mode_IndirectRead == cmdMode) {
            // 单线双向工模式：0 IO0输出，IO1输入    1 IO0负责输入输出
            LCD_QSPI_X->CR &= ~QSPI_CR_BIDI_Msk;
        }
    }
    
    QSPI_Command(LCD_QSPI_X, cmdMode, &cmdStruct);
    //printf("\r\n ins: 0x%x, addr: 0x%x, data: [%d] Bytes\r\n", cmdStruct.Instruction, cmdStruct.Address, cmdStruct.DataCount);
    
    // 无数据阶段
    if (QSPI_PhaseMode_None == cmdStruct.DataMode) {
        while (QSPI_Busy(LCD_QSPI_X)) __NOP();
    }
}

void __DECL_LCD_NAME(wr_data, qspi)(uint8_t *data, uint32_t count)
{
    for (uint32_t i = 0; i < (data ? count : 0); ++i)
    {
        while (QSPI_FIFOSpace(LCD_QSPI_X) < 1) __NOP();

        LCD_QSPI_X->DRB = data[i];
        //printf("[0x%x], ", data[i]);
    }
    while (QSPI_Busy(LCD_QSPI_X)) __NOP();
}

void __DECL_LCD_NAME(rd_data, qspi)(uint8_t *data, uint32_t count)
{
    for (uint32_t i = 0; i < (data ? count : 0); ++i)
    {
        while (QSPI_FIFOCount(LCD_QSPI_X) < 1) __NOP();

        data[i] = LCD_QSPI_X->DRB;
    }
    while (QSPI_Busy(LCD_QSPI_X)) __NOP();
    
    // 单线双向工模式：0 IO0输出，IO1输入    1 IO0负责输入输出
    LCD_QSPI_X->CR |= QSPI_CR_BIDI_Msk;
}

static void qspi_conver_std_peripheral(QSPI_CmdStructure *cmdStruct, lcd_qspi_seq_t *seq)
{
    cmdStruct->InstructionMode = (4 == seq->instruct_line) ? QSPI_PhaseMode_4bit : 
                                ((2 == seq->instruct_line) ? QSPI_PhaseMode_2bit : 
                                ((1 == seq->instruct_line) ? QSPI_PhaseMode_1bit : QSPI_PhaseMode_None));
    cmdStruct->Instruction = seq->instruct;
    
    cmdStruct->AddressMode = (4 == seq->addr_line) ? QSPI_PhaseMode_4bit : 
                            ((2 == seq->addr_line) ? QSPI_PhaseMode_2bit : 
                            ((1 == seq->addr_line) ? QSPI_PhaseMode_1bit : QSPI_PhaseMode_None));
    cmdStruct->AddressSize = (32 == seq->addr_bits) ? QSPI_PhaseSize_32bit : 
                            ((24 == seq->addr_bits) ? QSPI_PhaseSize_24bit : 
                            ((16 == seq->addr_bits) ? QSPI_PhaseSize_16bit : QSPI_PhaseSize_8bit));
    cmdStruct->Address = seq->addr;

    cmdStruct->DummyCycles = seq->dummy_cycles;

    cmdStruct->DataMode = (4 == seq->data_line) ? QSPI_PhaseMode_4bit : 
                        ((2 == seq->data_line) ? QSPI_PhaseMode_2bit : 
                        ((1 == seq->data_line) ? QSPI_PhaseMode_1bit : QSPI_PhaseMode_None));
    cmdStruct->DataCount = seq->data_count;

    cmdStruct->AlternateBytesMode = (4 == seq->alternate_byte_line) ? QSPI_PhaseMode_4bit : 
                                    ((2 == seq->alternate_byte_line) ? QSPI_PhaseMode_2bit : 
                                    ((1 == seq->alternate_byte_line) ? QSPI_PhaseMode_1bit : QSPI_PhaseMode_None));
    cmdStruct->AlternateBytesSize = (32 == seq->alternate_byte_bits) ? QSPI_PhaseSize_32bit : 
                                    ((24 == seq->alternate_byte_bits) ? QSPI_PhaseSize_24bit : 
                                    ((16 == seq->alternate_byte_bits) ? QSPI_PhaseSize_16bit : QSPI_PhaseSize_8bit));
    cmdStruct->AlternateBytes = seq->alternate_bytes;

    //cmdStruct->SendInstOnlyOnce = seq->send_ins_only_once;

    // data_count 为 0 则无数据阶段
    if (0 == seq->data_count) {
        cmdStruct->DataMode = QSPI_PhaseMode_None;
    }
}
