
#include <string.h>
#include "ugui/ugui.h"
#include "synwit_ui_framework/synwit_ui.h"
#include "synwit_ui_framework/synwit_ui_internal.h"

#include "board.h"
#include "app_cfg.h"


#include "SWM221_qspi.h"
#include "dev_lcd_mpu_qspi_private.h"
#define LCD_QSPI_X             QSPI0

#define INSTR_VAL 0xDE
#define INSTR_RD_VAL 0xDD
/* QSPI extra expand */
#define ST77903_WR_CMD_SEQ(cmd, seq, bytes)          do {                                             \
                                                        ST77903_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes); \
                                                        _cmd_qspi(cmd, seq);                     \
                                                    } while (0)

#define ST77903_WR_DATA_VA(...)                      do {                                    \
                                                        uint8_t temp[] = {__VA_ARGS__};     \
                                                        _wr_data(temp, sizeof(temp)); \
                                                    } while (0)																										

#define ST77903_RD_CMD_SEQ(cmd, seq, bytes)          do {                                             \
                                                        ST77903_QSPI_SEQ_SET_RD(seq, cmd, bytes); \
                                                        _cmd_qspi(cmd, seq);                     \
                                                    } while (0)
																										
																										
#define ST77903_QSPI_SEQ_SET_WR_L11(seq, cmd, bytes)          __LCD_QSPI_SEQ_SET_WR(seq, INSTR_VAL, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 1, bytes)		
																										

#define ST77903_QSPI_SEQ_SET_RD(seq, cmd, bytes)              __LCD_QSPI_SEQ_SET_RD(seq, INSTR_RD_VAL, 1, (cmd << 8) & 0xFF00, 1, 24, 0, 0, 0, 0, 1, bytes)
																										

#define ST77903_WD_DATA(cmd, seq, arr, count) do{                                       \
																									QSPI_Clr;                             \
																									ST77903_WR_CMD_SEQ(cmd, seq, count); \
																									if(count>0) _wr_data(arr, count);	                \
																									QSPI_Set;                             \
																								}while(0)
																										
/* 使能 TEST_CASE_EN 宏将执行单元测试用例: LED / KEY / TP / UART-Modbus / screen_demo, 
 * 反之若屏蔽该宏, 则仅保留 LCD 驱动显示.
 */
//#define TEST_CASE_EN 
#ifdef TEST_CASE_EN
#define __IMPORT_TEST_CASE(__case_name)     extern void test_case##_##__case_name(void); test_case##_##__case_name()
#else 
#define __IMPORT_TEST_CASE(__case_name)     
#endif

#define DISP_WIDTH     CFG_LCD_HDOT
#define DISP_HEIGHT    CFG_LCD_VDOT

/* 分配给显示缓存的像素个数(必须大于或等于屏幕宽度) */
#define DISP_PFB_PIXELS    (DISP_WIDTH * 1)
__USED static MEM_UNIT platform_heap[HEAP_SIZE / sizeof(MEM_UNIT)];

/* driver */
static void driver_init(void);
/*static*/ lcd_mpu_desc_t LCD_Obj;
#define This_LCD        (&LCD_Obj)
static volatile uint8_t ISR_Flag_LCD_DMA = 0; /* 0: idle  1: busy */

void GPIOB1_GPIOA9_DMA_Handler(void)
{
    if (0 == lcd_mpu_flush_bitmap_done(This_LCD)) {
        ISR_Flag_LCD_DMA = 0; // report platform(DMA support SPI 8bit / i8080 8bit / QSPI)
    }
}

static uint8_t QSPI_Multiplex_Flag = 0;//0: Init    1: LCD    2: Flash

void qspi_multiplex_lcd(void)
{
#if CFG_LCD_IF == CFG_LCD_IF_QSPI
    if (1 != QSPI_Multiplex_Flag) {
        QSPI_Multiplex_Flag = 1;
        flash_qspi_port_deinit();
        lcd_mpu_port_init(This_LCD);
    }
#endif
}
void qspi_multiplex_flash(void)
{
#if CFG_LCD_IF == CFG_LCD_IF_QSPI
    if (2 != QSPI_Multiplex_Flag) {
        QSPI_Multiplex_Flag = 2;
        lcd_mpu_port_deinit(This_LCD);
        flash_qspi_port_init();
    }
#endif
}

static void area_set(UG_S16 xs, UG_S16 ys, UG_S16 w, UG_S16 h)
{
//    printf("%s: xs, ys, w, h[%d, %d, %d, %d]\r\n", __FUNCTION__, xs, ys, w, h);
    lcd_mpu_flush_bitmap_wait(This_LCD, &ISR_Flag_LCD_DMA); // async wait

    qspi_multiplex_lcd(); // QSPI 分时复用 LCD 
    
    lcd_mpu_set_disp_area(This_LCD, xs, xs + w - 1, ys, ys + h - 1);
}

static void flush_pixels(UG_COLOR *colors, UG_U16 num, UG_U8 asyncable)
{
//    printf("%s: colors[0x%p], num[%d], asyncable[%d]\r\n", __FUNCTION__, colors, num, asyncable);
    lcd_mpu_flush_bitmap_wait(This_LCD, &ISR_Flag_LCD_DMA); // async wait

    qspi_multiplex_lcd(); // QSPI 分时复用 LCD 
    
    if (asyncable) {
        ISR_Flag_LCD_DMA = 1; //[async] set busy before [flush_bitmap] start
        if (0 == lcd_mpu_flush_bitmap_async(This_LCD, colors, num)) {
            return;
        }
        ISR_Flag_LCD_DMA = 0; // Fallback
    }
    lcd_mpu_flush_bitmap(This_LCD, colors, num); // sync
}

static int ui_data_read(UG_U32 offset, UG_U32 size, void *buf)
{
//    printf("%s: offset[0x%x], size[%d]\r\n", __FUNCTION__, offset, size);
    qspi_multiplex_flash(); // QSPI 分时复用 Flash 
    
    qspi_dma_read(offset, buf, size, 1, 2);

    return 0;
}

static void framework_ready(void)
{
    _ug_app_screen_register();
}

static void app_ready(void)
{
    // app界面准备就绪，打开背光
    for (uint32_t i = 0; i < 666666 * 8; ++i) __NOP(); // 稍微延时一下，让屏完成刷新
    lcd_mpu_set_backlight(This_LCD, 100);
}

static void pre_blue(const char* msg)
{
    // 蓝屏前确保打开背光
    lcd_mpu_set_backlight(This_LCD, 100);
}

static void main_tick(void) // 主循环每跑一次，都会触发一次这个函数
{
    __IMPORT_TEST_CASE(main_tick);
    
//    void test_case_screen_auto_play(void);
//    test_case_screen_auto_play();
}

static const SYS_OPS sys_ops = {
    .disp.area_set = area_set,
    .disp.flush_pixels = flush_pixels,
    .disp.flush_ready = NULL,
    .data.ui_data_read = ui_data_read,
    
    .app.framework_ready = framework_ready,
    .app.app_ready = app_ready,
    .app.pre_blue = pre_blue,
    
    .app.main_tick = main_tick,// main_tick, /* 主循环每跑一次，都会触发一次这个函数 */
    .app.main_tick_period = 50, /* 指定 main_tick 回调函数的间隔，毫秒为单位，默认为 50ms */
};

static const SYS_CONFIG sys_conf = {
    .display_width = DISP_WIDTH,   /* 屏幕宽度 */
    .display_height = DISP_HEIGHT, /* 屏幕高度 */

    /* 分配给每个显示缓存的像素个数(必须大于或等于屏幕宽度, 0表示取屏幕宽度) */
    .pixels_per_pfb = DISP_PFB_PIXELS,
    /* 显示缓存个数(可以是1: sync 或 2: async)*/
    .num_of_pfb = 1,

    .heap = platform_heap,             /* 内存堆地址 */
    .heap_len = sizeof(platform_heap), /* 内存堆大小(字节单位) */

    /* 刷新区域的对齐限定。如果屏或LCD控制器无特别限制，一般设为0即可。
     * 可以设为以下一个或多个限定条件(用|运算符)：
     *      FAA_EVEN_X:         限定刷新区的x为偶数
     *      FAA_EVEN_Y:         限定刷新区的y为偶数
     *      FAA_EVEN_W:         限定刷新区的宽度为偶数
     *      FAA_EVEN_H:         限定刷新区的高度为偶数
     *      FAA_EVEN_PIXELS:    限定刷新的总像素数为偶数
     * 
     */
    .flush_area_alignment = 0,
};

void systick_handler_hook(void)
{
    UG_TickInc(1);
    // insert user code(note: It is Systick ISR)

    __IMPORT_TEST_CASE(systick_handler);
}




static void _qspi_port_init(){

	//PORT_Init(PORTC, PIN2,  PORTC_PIN2_QSPI0_SSEL,  0);
	
	PORT_Init(PORTA, PIN5,  PORTA_PIN5_GPIO,  0);
	
	

	PORT_Init(PORTC, PIN3,  PORTC_PIN3_QSPI0_SCLK,  0);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_QSPI0_MOSI, 1);
	PORT_Init(PORTB, PIN0,  PORTB_PIN0_QSPI0_MISO,  1);
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_QSPI0_D2,    1);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_QSPI0_D3,    1);

	PORT_Init(PORTA, PIN2,  PORTA_PIN2_GPIO,  0);
	
	GPIO_INIT(GPIOA, PIN2, GPIO_OUTPUT);
	GPIO_INIT(GPIOA, PIN5, GPIO_OUTPUT);
}


static void _qspi_init(void)
{
	flash_qspi_port_init();
	
   QSPI_InitStructure QSPI_initStruct;
	QSPI_initStruct.Size = QSPI_Size_16MB;
	QSPI_initStruct.ClkDiv = 2;
	QSPI_initStruct.ClkMode = QSPI_ClkMode_3;
	QSPI_initStruct.SampleShift = (2 == QSPI_initStruct.ClkDiv) ? QSPI_SampleShift_1_SYSCLK : QSPI_SampleShift_NONE;
	QSPI_initStruct.IntEn = 0;
	QSPI_Init(QSPI0, &QSPI_initStruct);
	QSPI_Open(QSPI0);
	
	
}

#define LCD_Reset_Clr GPIO_AtomicClrBit(GPIOA, PIN2)
#define LCD_Reset_Set GPIO_AtomicSetBit(GPIOA, PIN2)

#define QSPI_Clr  GPIO_AtomicClrBit(GPIOA, PIN5)
#define QSPI_Set  GPIO_AtomicSetBit(GPIOA, PIN5)

static void _lcd_init(){

	_qspi_port_init();
	_qspi_init();
	

	LCD_Reset_Set;
	systick_delay_us(10);
	LCD_Reset_Clr;
	systick_delay_us(10);
	LCD_Reset_Set;
	systick_delay_ms(120);
	
	//QSPI_Set;
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


static void _cmd_qspi(uint32_t cmd, void *seq)
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


static void _board_init(){
    SystemInit();
    
    uart_debug_init();
    systick_init();
}




static void _wr_data(uint8_t *data, uint32_t count)
{
    for (uint32_t i = 0; i < (data ? count : 0); ++i)
    {
        while (QSPI_FIFOSpace(LCD_QSPI_X) < 1) __NOP();

        LCD_QSPI_X->DRB = data[i];
        //printf("[0x%x], ", data[i]);
    }
    while (QSPI_Busy(LCD_QSPI_X)) __NOP();
}

static void _rd_data(uint8_t *data, uint32_t count)
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


static void _lcd_st7709_init_bk(){
	lcd_qspi_seq_t seq;
	
	uint8_t t_f[] = {0x40,0x00, 0x3f, 0x00 ,0x0A,0x0A,0xEA,0xEA,0x03};
	
	
	QSPI_Clr;	
	ST77903_WR_CMD_SEQ(0xB0, &seq, 1);
	ST77903_WR_DATA_VA(0xA5);
	QSPI_Set;
	
	
	QSPI_Clr;	
	ST77903_WR_CMD_SEQ(0xCC, &seq, sizeof(t_f));	
	_wr_data(t_f, sizeof(t_f));
	QSPI_Set;
//	ST77903_WR_DATA_VA(0x40);	
//	ST77903_WR_DATA_VA(0x00);	
//	ST77903_WR_DATA_VA(0x3f);
//	ST77903_WR_DATA_VA(0x00);
//	ST77903_WR_DATA_VA(0x0A);
//	ST77903_WR_DATA_VA(0x0A);
//	ST77903_WR_DATA_VA(0xEA);
//	ST77903_WR_DATA_VA(0xEA);
//	ST77903_WR_DATA_VA(0x03);
	
	
}

static void _lcd_set_disp(uint8_t idx){
	lcd_qspi_seq_t seq;
	if(idx == 0){
		ST77903_WD_DATA(0x28, &seq,0,0);
	}else{
		ST77903_WD_DATA(0x29, &seq,0,0);
	}
}


static void _lcd_st7709_init(){
	lcd_qspi_seq_t seq;

	
	
#if 0	
	QSPI_Clr;	
	ST77903_WR_CMD_SEQ(0xf0, &seq, 1);
	_wr_data((uint8_t []){0xc3}, 1);
	QSPI_Set;
	
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xf0, &seq, 1);
	_wr_data((uint8_t []){0x96}, 1);	
	QSPI_Set;
	
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xf0, &seq, 1);
	_wr_data((uint8_t []){0xa5}, 1);	
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xb6, &seq, 2);
	_wr_data((uint8_t []){0xc7,0x31}, 2);	
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xb5, &seq,4);
	_wr_data((uint8_t []){0x00 ,0x08 ,0x00 ,0x08}, 4);	
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0x36, &seq, 1);
	_wr_data((uint8_t []){0x0C}, 1);	
	QSPI_Set;
	
//	QSPI_Clr;
//	ST77903_WR_CMD_SEQ(0x3a, &seq, 1);
//	_wr_data((uint8_t []){0x07}, 1);		
//	QSPI_Set;
	ST77903_WD_DATA(0x3a, &seq,(uint8_t []){0x05},1);
	
	
	QSPI_Clr;	
	ST77903_WR_CMD_SEQ(0xf0, &seq, 1);
	_wr_data((uint8_t []){0xc3}, 1);				
	QSPI_Set;
	
	
	QSPI_Clr;	
	ST77903_WR_CMD_SEQ(0xf0, &seq, 1);
	_wr_data((uint8_t []){0x96}, 1);				
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xf0, &seq, 1);
	_wr_data((uint8_t []){0xa5}, 1);		
	QSPI_Set;
	

	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xe9, &seq, 1);
	_wr_data((uint8_t []){0x20}, 1);	
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xe7, &seq, 4);
	_wr_data((uint8_t []){0x80, 0x77, 0x1f, 0xcc}, 4);		
	QSPI_Set;				 
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xc1, &seq, 4);
	_wr_data((uint8_t []){0x77, 0x07, 0xcf, 0x16}, 4);		
  QSPI_Set; 
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xc2, &seq, 4);
	_wr_data((uint8_t []){0x77, 0x07, 0xcf, 0x16}, 4);		
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xc3, &seq, 4);
	_wr_data((uint8_t []){0x22, 0x02, 0x22, 0x04}, 4);		
	QSPI_Set;	
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xc4, &seq, 4);
	_wr_data((uint8_t []){0x22, 0x02, 0x22, 0x04}, 4);		
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xc5, &seq, 1);
	_wr_data((uint8_t []){0xed}, 1);		
  QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xe0, &seq, 14);
	_wr_data((uint8_t []){0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f}, 14);		
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xe1, &seq, 14);
	_wr_data((uint8_t []){0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f}, 14);		
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xe5, &seq, 14);
	_wr_data((uint8_t []){0xbe, 0xf5, 0xb1, 0x22, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}, 14);		
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xe6, &seq, 14);
	_wr_data((uint8_t []){0xbe, 0xf5, 0xb1, 0x22, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}, 14);		
  QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xec, &seq, 2);
	_wr_data((uint8_t []){0x40, 0x03}, 2);				
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xb2, &seq, 1);
	_wr_data((uint8_t []){0x00}, 1);				
	QSPI_Set;  
  
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xb3, &seq, 1);
	_wr_data((uint8_t []){0x01}, 1);				
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xb4, &seq, 1);
	_wr_data((uint8_t []){0x00}, 1);	
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xa5, &seq, 9);
	_wr_data((uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x2a, 0x8a, 0x02}, 9);	
  QSPI_Set; 
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xa6, &seq, 9);
	_wr_data((uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x2a, 0x8a, 0x02}, 9);	
  QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xba, &seq, 7);
	_wr_data((uint8_t []){0x0a, 0x5a, 0x23, 0x10, 0x25, 0x02, 0x00}, 7);	
  QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xbb, &seq, 8);
	_wr_data((uint8_t []){0x00, 0x30, 0x00, 0x2c, 0x82, 0x87, 0x18, 0x00}, 8);	
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xbc, &seq, 8);
	_wr_data((uint8_t []){0x00, 0x30, 0x00, 0x2c, 0x82, 0x87, 0x18, 0x00}, 8);	
  QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0xbd, &seq, 11);
	_wr_data((uint8_t []){0xa1, 0xb2, 0x2b, 0x1a, 0x56, 0x43, 0x34, 0x65, 0xff, 0xff, 0x0f}, 11);	
	QSPI_Set;
	
	QSPI_Clr;	
	ST77903_WR_CMD_SEQ(0x35, &seq, 1);
	_wr_data((uint8_t []){0x00}, 1);		
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0x21, &seq, 1);
	_wr_data((uint8_t []){0x00}, 1);		
	QSPI_Set;
	
	QSPI_Clr;
	ST77903_WR_CMD_SEQ(0x1, &seq, 1);
	_wr_data((uint8_t []){0x00}, 1);		
	QSPI_Set;
	
	systick_delay_ms(120);
		
	
	

	
	
	QSPI_Set;	
#endif	


	
	
	

	ST77903_WD_DATA(0xf0, &seq,((uint8_t []){0xc3}),1);
	

	ST77903_WD_DATA(0xf0, &seq,((uint8_t []){0x96}),1);

	ST77903_WD_DATA(0xf0, &seq,((uint8_t []){0xa5}),1);

	ST77903_WD_DATA(0xb6, &seq,((uint8_t []){0xc7,0x31}),2);
	
	ST77903_WD_DATA(0xb5, &seq,((uint8_t []){0x00 ,0x08 ,0x00 ,0x08}),4);
	
	ST77903_WD_DATA(0x36, &seq,((uint8_t []){0x0C}),1);
	
	ST77903_WD_DATA(0x3a, &seq,((uint8_t []){0x05}),1);
	
	ST77903_WD_DATA(0xf0, &seq,((uint8_t []){0xc3}),1);

	ST77903_WD_DATA(0xf0, &seq,((uint8_t []){0x96}),1);

	ST77903_WD_DATA(0xf0, &seq,((uint8_t []){0xa5}),1);

	ST77903_WD_DATA(0xe9, &seq,((uint8_t []){0x20}),1);

	ST77903_WD_DATA(0xe7, &seq,((uint8_t []){0x80, 0x77, 0x1f, 0xcc}),4);

	ST77903_WD_DATA(0xc1, &seq,((uint8_t []){0x77, 0x07, 0xcf, 0x16}),4);
	

	ST77903_WD_DATA(0xc2, &seq,((uint8_t []){0x77, 0x07, 0xcf, 0x16}),4);
	

	ST77903_WD_DATA(0xc3, &seq,((uint8_t []){0x22, 0x02, 0x22, 0x04}),4);

	ST77903_WD_DATA(0xc4, &seq,((uint8_t []){0x22, 0x02, 0x22, 0x04}),4);
	

	ST77903_WD_DATA(0xc5, &seq,((uint8_t []){0xed}),1);
 
	ST77903_WD_DATA(0xe0, &seq,((uint8_t []){0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f}),14);

	ST77903_WD_DATA(0xe1, &seq,((uint8_t []){0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f}),14);

	ST77903_WD_DATA(0xe5, &seq,((uint8_t []){0xbe, 0xf5, 0xb1, 0x22, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}),14);

	ST77903_WD_DATA(0xe6, &seq,((uint8_t []){0xbe, 0xf5, 0xb1, 0x22, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}),14);

	ST77903_WD_DATA(0xec, &seq,((uint8_t []){0x40, 0x03}),2);	

	ST77903_WD_DATA(0xb2, &seq,((uint8_t []){0x00}),1);		

	ST77903_WD_DATA(0xb3, &seq,((uint8_t []){0x01}),1);		
	
	ST77903_WD_DATA(0xb4, &seq,((uint8_t []){0x00}),1);		

	ST77903_WD_DATA(0xa5, &seq,((uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x2a, 0x8a, 0x02}),9);		

	ST77903_WD_DATA(0xa6, &seq,((uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x2a, 0x8a, 0x02}),9);	

	ST77903_WD_DATA(0xba, &seq,((uint8_t []){0x0a, 0x5a, 0x23, 0x10, 0x25, 0x02, 0x00}),7);	

	ST77903_WD_DATA(0xbb, &seq,((uint8_t []){0x0a, 0x5a, 0x23, 0x10, 0x25, 0x02, 0x00}),7);	

	ST77903_WD_DATA(0xbc, &seq,((uint8_t []){0x00, 0x30, 0x00, 0x2c, 0x82, 0x87, 0x18, 0x00}),8);	

	ST77903_WD_DATA(0xbd, &seq,((uint8_t []){0xa1, 0xb2, 0x2b, 0x1a, 0x56, 0x43, 0x34, 0x65, 0xff, 0xff, 0x0f}),11);	

	ST77903_WD_DATA(0x35, &seq,((uint8_t []){0x00}),1);		

	ST77903_WD_DATA(0x21, &seq,((uint8_t []){0x00}),0);	

	ST77903_WD_DATA(0x11, &seq,((uint8_t []){0x00}),0);	
	
	systick_delay_ms(120);

}
static uint8_t test_rd[8] = {0};
static void _test_qspi_read(){
	lcd_qspi_seq_t seq;
	
	QSPI_Clr;	
	
	ST77903_RD_CMD_SEQ(0x09, &seq, 4);
	
	_rd_data(test_rd, 4);
	
	QSPI_Set;	

	printf("date[%02X][%02X][%02X][%02X]\r\n",test_rd[0],test_rd[1],test_rd[2],test_rd[3] );
	
}

int main(void)
{
    /* 在调测阶段(版本 Release 时可注释), 预防程序跑飞后, 上电瞬间锁死内核导致无法通过 SWD 访问, 也可用作等待个别硬件模块上电稳定 */
    for (uint32_t i = 0; i < 666666 * 8; ++i) __NOP();
		lcd_qspi_seq_t seq;
#if 0
    board_init();
    printf("Hi, World! SystemCoreClock: %d Hz\r\n", SystemCoreClock);
    
    char ver[48];
    synwit_ug_get_platform_version_name(ver, sizeof(ver));
    
    driver_init();
    printf("\r\n-------------------------------------\r\n");
    printf("[%s] port [%s] platform version: %s Demo \r\n", "SWM221", "ugui", ver);
    printf("build-release %s-%s \r\n", __DATE__, __TIME__);
    printf("@Copyright by Synwit Technology");
    printf("\r\n-------------------------------------\r\n");

    /* Start scheduler. */
    synwit_ug_start(&sys_ops, &sys_conf);
#endif
		_board_init();
		
		    printf("@Copyright by Synwit Technology");
    printf("\r\n-------------------------------------\r\n");
		
		
#if 1
		_lcd_init();
		
		_lcd_set_disp(1);
		_lcd_st7709_init();		
		_lcd_st7709_init_bk();
    /* Should not reach here as the scheduler is already started. */
    for (;;)
    {
			systick_delay_ms(50);
		
        __NOP();
			_test_qspi_read();
			
//			QSPI_Clr;
//			ST77903_WR_CMD_SEQ(0x55, &seq, 1);
//			ST77903_WR_DATA_VA(0x11);
//			QSPI_Set;
			
//			printf("Hi, World! SystemCoreClock: %d Hz\r\n", SystemCoreClock);
//			
//			systick_delay_ms(120);
//			
//			QSPI_Clr;
//			ST77903_WR_CMD_SEQ(0x29, &seq, 0);
//			QSPI_Set;
			//ST77903_WR_DATA_VA(0xc3);
			
	
			
    }
#else
	qspi_flash_init();
	
extern void test_case_qspi_flash(uint32_t addr, uint32_t size);
	test_case_qspi_flash(15 * 1024 * 1024, 256 * 1024);	
	
	for(;;){
		systick_delay_ms(400);
		__NOP();	
	}

#endif
    return 0;
}

static void driver_init(void)
{
    const lcd_mpu_cfg_t lcd_cfg = {
        .name = CFG_LCD_NAME,
        .hres = CFG_LCD_HDOT,
        .vres = CFG_LCD_VDOT,
        .interface = CFG_LCD_IF,
    };
    if (0 != lcd_mpu_init(This_LCD, &lcd_cfg))
    {
        printf("[%s]: [lcd_mpu_init] config error!\r\n", __FUNCTION__);
        while (1) __NOP(); /* capture LCD driver error! */
    }
    /* 若屏内 COG 驱动芯片支持横竖屏转换(对调分辨率),
     * 当用户有需求时可手动在 LCD 初始化后显式调用 lcd_mpu_set_rotate() 接口进行旋转,
     * 旋转方向请参考具体驱动 lcd_xxx.c 实现;
     */
    // lcd_mpu_set_rotate(This_LCD, 0);
        
    // lcd_mpu_set_backlight(This_LCD, 100); // 开光
    
    qspi_multiplex_flash(); // QSPI 分时复用 Flash 

    /* SPI_Flash */
    qspi_flash_init();
    
    qspi_multiplex_lcd(); // QSPI 分时复用 LCD 

    __IMPORT_TEST_CASE(driver_init);
}

void HardFault_Handler(void)
{
    printf("[%s]!\r\n", __FUNCTION__);
    while (1) __NOP();
}
