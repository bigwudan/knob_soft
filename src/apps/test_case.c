
#include <string.h>
#include "ugui/ugui.h"
#include "synwit_ui_framework/synwit_ui.h"
#include "synwit_ui_framework/synwit_ui_internal.h"

#include "board.h"
#include "app_cfg.h"

#include "sfud.h"

#define DISP_WIDTH     CFG_LCD_HDOT
#define DISP_HEIGHT    CFG_LCD_VDOT

/* driver */
extern lcd_mpu_desc_t LCD_Obj;
#define This_LCD     (&LCD_Obj)
static tp_desc_t TP_Obj;
#define This_TP (&TP_Obj)
static volatile uint8_t Key_Event = NONE_PRESS;

/*******************************************************************************************************************************************
 * test case public
 *******************************************************************************************************************************************/
void test_case_main_tick(void) // 主循环每跑一次，都会触发一次这个函数
{
    void test_case_led(void);
    test_case_led();

    void test_case_key(void);
    test_case_key();

    void test_case_tp_report(void);
    test_case_tp_report();
}

void test_case_systick_handler(void) //insert user code(note: It is Systick ISR)
{
    static uint8_t test_init_flag = 1;
    if (test_init_flag) {
        test_init_flag = 0;
        user_key_init();
    }
    static uint32_t timestamp = 0;
    if (systick_get_tick() - timestamp >= 5)
    {
        timestamp = systick_get_tick();
        button_ticks(); // 每 5ms 调用 1 次
    }
    uint8_t event = user_key_handler(USER_BTN_ID_0);
    if (event != NONE_PRESS) {
        Key_Event = event;
    }
}

void test_case_driver_init(void)
{
    /* 如果实现了 PWM 背光调控, 可以观察到呼吸灯效果 */
    void test_case_lcd_bg_pwm(void);
    test_case_lcd_bg_pwm();

    /* Simple 色块测试自检, 以验证 LCD 显示驱动是否正常 */
    void test_case_draw_rect(void);
    test_case_draw_rect();

#if 0 /* Simple 擦读写, 以验证 SPI Flash 驱动是否正常(注意此测试项会覆盖 SPI Flash 中的原有数据) */
    extern void qspi_multiplex_flash(void);
    qspi_multiplex_flash(); // QSPI 分时复用 Flash 
    
    void test_case_qspi_flash(uint32_t addr, uint32_t size);
    test_case_qspi_flash(15 * 1024 * 1024, 256 * 1024);
    
    extern void qspi_multiplex_lcd(void);
    qspi_multiplex_lcd(); // QSPI 分时复用 LCD 

//    #define SFUD_DEMO_TEST_BUFFER_SIZE                     (1024)
//    uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE];
//    void test_case_sfud_demo(uint32_t addr, size_t size, uint8_t *data);
//    test_case_sfud_demo(0, sizeof(sfud_demo_test_buf), sfud_demo_test_buf);
#endif
}

/*******************************************************************************************************************************************
 * test case static
 *******************************************************************************************************************************************/
void test_case_screen_next(void)
{
#if 0
    #include "./ui_src/appkit/screen_id.h"
    UG_ID scr_id_table[] = {
        SCREEN001, SCREEN002, SCREEN003, SCREEN004, SCREEN005, /*SCREEN006, SCREEN007, SCREEN008, */
    };
    
    UG_ID scr_id;
    scr_id = synwit_ug_get_cur_screen_id();
    if (0 == scr_id) {
        printf("[%s]: synwit_ug_get_cur_screen_id erro code = [%d]\r\n", __FUNCTION__, scr_id);
        return ;
    }
    for (uint32_t i = 0; i < sizeof(scr_id_table) / sizeof(scr_id_table[0]); ++i) {
        if (scr_id_table[i] == scr_id) { //遍历查表找到当前页面ID的表索引
            i += 1;
            if (i >= sizeof(scr_id_table) / sizeof(scr_id_table[0])) {
                i = 0;
            }
            scr_id = scr_id_table[i]; //表索引 +1 下一个页面(循环)
        }
    }
    int result = synwit_ug_load_screen(scr_id);
    if (0 > result) {
        printf("[%s]: synwit_ug_load_screen erro code = [%d]\r\n", __FUNCTION__, result);
        return ;
    }
#endif
}

void test_case_wdt(void)
{
    static uint8_t test_init_flag = 1;
    if (test_init_flag) {
        test_init_flag = 0;
        WDT_Init(WDT, 0, 1000 / 2); //1000 => 1s
        WDT_Start(WDT);
    }
    /* 注意: 两次喂狗之间必须间隔 5 个看门狗时钟周期以上, 低于 5 个频繁喂狗将视为程序执行流不正常而引发 WDT 复位.
     * 可看作是下限固定的窗口看门狗
     */
    static uint32_t timestamp = 0;
    if (systick_get_tick() - timestamp >= 5)
    {
        timestamp = systick_get_tick();
        WDT_Feed(WDT);
    }
}

void test_case_led(void)
{
    static uint32_t time_stamp = 0;
    if (systick_get_tick() - time_stamp >= 1000) {
        time_stamp = systick_get_tick();
        
        static uint8_t test_init_flag = 1;
        if (test_init_flag) {
            test_init_flag = 0;
            GPIO_INIT(GPIOB, PIN10, GPIO_OUTPUT); //LED Test
        }
        GPIO_AtomicInvBit(GPIOB, PIN10);
    }
}
    
void test_case_key(void)
{
    if (NONE_PRESS != Key_Event) {
        if (SINGLE_CLICK == Key_Event) {
            printf("Key SINGLE_CLICK!\r\n");

            void test_case_screen_next(void);
            test_case_screen_next();
            
        } else if (DOUBLE_CLICK == Key_Event) {
            printf("Key DOUBLE_CLICK!\r\n");
        } else if (LONG_PRESS_HOLD == Key_Event) {
            printf("Key LONG_PRESS_HOLD!\r\n");
            
            extern void qspi_multiplex_flash(void);
            qspi_multiplex_flash(); // QSPI 分时复用 Flash 
            
            /* 测试 Modbus 更新片外 SPI Flash 数据 */
            printf("Run while(1) UI update of Modbus-UART!\r\n");
            void test_case_modbus(void);
            test_case_modbus();
            
            extern void qspi_multiplex_lcd(void);
            qspi_multiplex_lcd(); // QSPI 分时复用 LCD 
        }
        Key_Event = NONE_PRESS;
    }
}

static void driver_init_tp(void)
{
    //see [app_cfg.h] : LCD to TP map 
    const struct /* 用于 TP 校准基准原点坐标和边界值, 方便映射不同的 LCD 模组 */
    {
        const char *lcd_name;
        tp_align_pos_t tp_pos;
    } LCD_TP_Map[] = {
        /* LCD 模组名, X 镜像, Y 镜像, X/Y 坐标对调, X 坐标偏移, Y 坐标偏移, X 坐标上限, Y 坐标上限 */
        {"ZZW180WBS", 0, 0, 0, 0, 0, CFG_LCD_HDOT, CFG_LCD_VDOT},
    };
    tp_cfg_t tp_cfg;
    memset(&tp_cfg, 0, sizeof(tp_cfg_t));
    tp_cfg.pos.max_x = CFG_LCD_HDOT;
    tp_cfg.pos.max_y = CFG_LCD_VDOT;
    tp_cfg.work_mode = TP_MODE_INT; //TP_MODE_POLL \ TP_MODE_INT
#ifdef CFG_TP_NAME
    tp_cfg.name = CFG_TP_NAME;
#else 
    tp_cfg.name = "NO_instances";
#endif
    for (uint32_t i = 0; i < sizeof(LCD_TP_Map) / sizeof(LCD_TP_Map[0]); ++i) {
        if (0 == strcmp(CFG_LCD_NAME, LCD_TP_Map[i].lcd_name)) {
            memcpy(&tp_cfg.pos, &LCD_TP_Map[i].tp_pos, sizeof(tp_align_pos_t));
            break;
        }
    }
    if (0 != tp_init(This_TP, &tp_cfg))
    {
        printf("[%s]: Touch Driver warning, You also check The [%s] Touch Driver!\r\n", __FUNCTION__, tp_cfg.name);
        //while (1) __NOP(); /* Tip: warning, No capture Touch driver error! */
    }
}

void test_case_tp_report(void)
{
    static uint8_t test_init_flag = 1;
    if (test_init_flag) {
        test_init_flag = 0;
        void driver_init_tp(void);
        driver_init_tp();
    }

    #define TP_READ_POINTS      ( 1 ) //期望读取的触摸点数
    static tp_data_t tp_data[TP_READ_POINTS] = {0};
    uint8_t points = tp_read_points(This_TP, tp_data, TP_READ_POINTS);
    //printf("[%s]: get points[%d] / [%d]\r\n", __FUNCTION__, points, TP_READ_POINTS);        
    #undef TP_READ_POINTS
    
    static uint8_t TP_PR_Flag = 0;
    /* 回报给平台（最后一次更新的坐标值需要保证持续） */
    if(TP_STA_PRESSED == tp_data[0].sta) {
        TP_PR_Flag = 1;
        //UG_TouchUpdate(tp_data[0].x, tp_data[0].y, TOUCH_STATE_PRESSED);
    } else {
        if (TP_PR_Flag > 0) {
            TP_PR_Flag = 0; //按下松手触发一次
            
            //UG_TouchUpdate(tp_data[0].x, tp_data[0].y, TOUCH_STATE_RELEASED);
            
            void test_case_screen_next(void);
            test_case_screen_next();
        }
    }
}

void test_case_lcd_bg_pwm(void) /* Simple 呼吸灯效果 */
{
    printf("\r\n[%s]: entry!\r\n", __FUNCTION__);
    const uint32_t bg_max_val = 100, stride_bg = 1, stride_delay = 10;
    for (uint32_t i = 0; i < bg_max_val; i += stride_bg) {
        lcd_mpu_set_backlight(This_LCD, i);
        systick_delay_ms(stride_delay);
    }
    for (uint32_t i = 0; i < bg_max_val; i += stride_bg) {
        lcd_mpu_set_backlight(This_LCD, bg_max_val - i);
        systick_delay_ms(stride_delay);
    }
    for (uint32_t i = 0; i < bg_max_val; i += stride_bg) {
        lcd_mpu_set_backlight(This_LCD, i);
        systick_delay_ms(stride_delay);
    }
    printf("\r\n[%s]: exit!\r\n", __FUNCTION__);
}

static void UserPixelSetFunction( UG_S16 x , UG_S16 y , UG_COLOR c )
{
    lcd_mpu_set_disp_area(This_LCD, x, x, y, y);
    lcd_mpu_draw_point(This_LCD, c);
}
void test_case_draw_rect(void)
{
    printf("\r\n[%s]: entry!\r\n", __FUNCTION__);
    const UG_U16 color_test[] = {C_RED, C_GREEN, C_BLUE, C_WHITE, C_BLACK}; //背景 + 四边线: 至少五种颜色
    uint32_t i = 0;
    for (i = 0; i < sizeof(color_test) / sizeof(color_test[0]); ++i)
    {
        uint32_t color_i = i;
        uint16_t x = 0, y = 0;
        
#if 1 /* 全屏刷纯色 */
        lcd_mpu_set_disp_area(This_LCD, 0, 0 + DISP_WIDTH - 1, 0, 0 + DISP_HEIGHT - 1);
        lcd_mpu_fill_color(This_LCD, color_test[color_i], DISP_WIDTH * DISP_HEIGHT);
#else 
        for (y = 0; y < DISP_HEIGHT; ++y) {
            for (x = 0; x < DISP_WIDTH; ++x) {
                UserPixelSetFunction(x, y, color_test[color_i]);
            }
        }

#endif
        if (++color_i >= sizeof(color_test) / sizeof(color_test[0])) color_i = 0;
        //systick_delay_ms(1000);
        
#if 1 /* 四边线测试 */
        uint16_t stride = 10; //边界线的宽度(四边等宽) / pixels
        for (uint16_t s = 0; s < stride; ++s) {
            for (x = 0; x < DISP_WIDTH; ++x) {
                UserPixelSetFunction(x, s, color_test[color_i]);
            }
        }
        if (++color_i >= sizeof(color_test) / sizeof(color_test[0])) color_i = 0;
        
        for (uint16_t s = 0; s < stride; ++s) {
            for (x = 0; x < DISP_WIDTH; ++x) {
                UserPixelSetFunction(x, DISP_HEIGHT - 1 - s, color_test[color_i]);
            }
        }
        if (++color_i >= sizeof(color_test) / sizeof(color_test[0])) color_i = 0;
        
        for (uint16_t s = 0; s < stride; ++s) {
            for (y = 0; y < DISP_HEIGHT; ++y) {
                UserPixelSetFunction(s, y, color_test[color_i]);
            }
        }
        if (++color_i >= sizeof(color_test) / sizeof(color_test[0])) color_i = 0;
        
        for (uint16_t s = 0; s < stride; ++s) {
            for (y = 0; y < DISP_HEIGHT; ++y) {
                UserPixelSetFunction(DISP_WIDTH - 1 - s, y, color_test[color_i]);
            }
        }
        if (++color_i >= sizeof(color_test) / sizeof(color_test[0])) color_i = 0;
#endif
        systick_delay_ms(1000); //等待 1s 观察
    }
    printf("\r\n[%s]: exit!\r\n", __FUNCTION__);
}

#include "chry_ringbuffer.h"
static chry_ringbuffer_t rb;
static volatile uint8_t ISR_Flag_UART_Debug = 0;
#define MODBUS_UART       UART1

#if 0 // UART DEBUG 回显
void uart_debug_handler_readbyte_hook(uint8_t chr)
{
    printf("%c", chr);
}
void uart_debug_handler_timeout_hook(void)
{
    printf("\r\n");
}
#else // UART modbus
void uart_debug_handler_readbyte_hook(uint8_t chr)
{
    chry_ringbuffer_write_byte(&rb, chr);
    ISR_Flag_UART_Debug = 1;
}
void uart_debug_handler_timeout_hook(void)
{
    if (ISR_Flag_UART_Debug == 1) {
        ISR_Flag_UART_Debug = 0;
    } else {
        ISR_Flag_UART_Debug = 2; //continue timeout(fix uart_init first timeout interrupt)
    }
}
#endif

#include "modbus_rtu.h"
static uint32_t modbus_rx_data(uint8_t *buff, uint32_t len)
{
    uint32_t back_len = chry_ringbuffer_read(&rb, buff, len);
    ISR_Flag_UART_Debug = 2;
    return back_len;
}
/**
 * @brief   接收数据是否超时
 * @param   \
 * @retval  MB_STA_OK: timeout && data ready    MB_STA_BUSY: rx data busy    MB_STA_NO_DATA: timeout && no data
 */
static uint8_t modbus_rx_timeout(void)
{
    if (0 == ISR_Flag_UART_Debug) {
        return MB_STA_OK;
    }
    else if (1 == ISR_Flag_UART_Debug) {
        return MB_STA_BUSY;
    }
    return MB_STA_NO_DATA; //2 == ISR_Flag_UART_Debug
}

static uint32_t modbus_tx_data(const uint8_t *buff, uint32_t len)
{
    uart_send_msg(MODBUS_UART, buff, len);
    return len;
}

static void modbus_uart_init(void)
{
    /**
     * 需要注意的点是，init 函数第三个参数是内存池的大小（字节为单位）
     * 也是 ringbuffer 的深度，必须为 2 的幂次！！！。
     * 例如 4、16、32、64、128、1024、8192、65536等
     */
    static uint8_t uart_rx_buff[256];
    if (0 == chry_ringbuffer_init(&rb, uart_rx_buff, 256)) {
        //printf("chry_ringbuffer_init success\r\n");
    } else {
        printf("chry_ringbuffer_init error\r\n");
        while (1) __NOP();
    }
    PORT_Init(PORTB, PIN8, PORTB_PIN8_UART1_RX, 1);
    PORT_Init(PORTB, PIN7, PORTB_PIN7_UART1_TX, 0);

    UART_InitStructure UART_initStruct;
    UART_initStruct.Baudrate = 115200; //250000;
    UART_initStruct.DataBits = UART_DATA_8BIT;
    UART_initStruct.Parity = UART_PARITY_NONE;
    UART_initStruct.StopBits = UART_STOP_1BIT;
    UART_initStruct.RXThreshold = 3;
    UART_initStruct.RXThresholdIEn = 1;
    UART_initStruct.TXThreshold = 3;
    UART_initStruct.TXThresholdIEn = 0;
    UART_initStruct.TimeoutTime = 5; //old: 5 or 20
    /* 1.5/3.5 char times (>= 19200 bps fixed 750us / 1750us) */
    UART_initStruct.TimeoutIEn = 1;
    UART_Init(MODBUS_UART, &UART_initStruct);
    UART_Open(MODBUS_UART);
}

/**
 * @brief 单元测试用例: Modbus 下载更新片外 SPI Flash
 */
void test_case_modbus(void)
{
    printf("\r\n[%s]: entry!\r\n", __FUNCTION__);
    modbus_cfg_t modbus_cfg = {
        .rx_data = modbus_rx_data,
        .tx_data = modbus_tx_data,
        .rx_timeout = modbus_rx_timeout,
        //.tx_done = modbus_tx_done,
    };
    modbus_session_t modbus_session;
    modbus_session_init(&modbus_session, &modbus_cfg);
    
    /* 数据缓冲区, 大小不会超过 Modbus 协议规定的长度 */
    uint8_t modbus_buff[MB_ADU_MAX_SIZE];
    uint16_t buff_offset = 0;
    uint32_t addr_offset = 0;
    uint32_t cnt = 0; /* 传输累计量 / Bytes */
    
    /* Modbus 上位机自定义通讯帧中目前未加入整个文件传输过程的总长度字段,
     * 故无法预先擦除对应扇区, 只能数据传输与擦写交替进行.
     */
    for (modbus_uart_init();;) //目前退出循环的触发信号 Key_Event
    {
        if (DOUBLE_CLICK == Key_Event) break;
        
        uint16_t len = 0; /* 单包数据量 / Bytes */
        if (MB_STA_OK != modbus_handler(&modbus_session, &modbus_buff[buff_offset], (uint8_t *)&len))
        {
            continue;
        }
        if (len == 0)
        {
            continue;
        }
        /* 每 4096/128 个包执行擦除, 1 个扇区 4K 字节 */
        if (0 == modbus_session.extra_synwit.pkt_id % (4096 / modbus_session.extra_synwit.pkt_unit))
        {
            uint32_t addr = addr_offset + modbus_session.extra_synwit.pkt_id * modbus_session.extra_synwit.pkt_unit, size = 4096;
            QSPI_Erase(QSPI0, QSPI_CMD_ERASE_SECTOR, addr, 1);
        }
        /* 每 2 包凑成 1 页执行写入(1 包 128 字节) */
        if (buff_offset == 0)
        {
            buff_offset = len;
        }
        else /* if (offset == 128) */
        {
            uint8_t *data = &modbus_buff[0];
            uint32_t addr = addr_offset + (modbus_session.extra_synwit.pkt_id - 1) * modbus_session.extra_synwit.pkt_unit, size = buff_offset + len;
            QSPI_Write(QSPI0, addr, data, size);
            buff_offset = 0;
            
            uint8_t check_buff[256];
            QSPI_Read_4bit(QSPI0, addr, check_buff, size);
            if (0 != memcmp(check_buff, data, size)) {
                printf("addr : 0x[%x], size: %d!\r\n", addr, size);
                for (uint32_t i = 0; i < size; ++i) {
                    printf("[%d]: 0x%02X, 0x%02X \r\n", i, data[i], check_buff[i]);
                }
                while (1) __NOP();
            }
        }
        cnt += len;
    }
    printf("\r\n[%s]: exit!\r\n", __FUNCTION__);
}

/**
 * SFUD demo for the first flash device test.
 *
 * @param addr flash entry address
 * @param size test flash size
 * @param size test flash data buffer
 */
void test_case_sfud_demo(uint32_t addr, size_t size, uint8_t *data)
{
    printf("\r\n[%s]: entry!\r\n", __FUNCTION__);
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    size_t i;
    /* prepare write data */
    for (i = 0; i < size; i++) {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(flash, addr, size);
    if (result == SFUD_SUCCESS) {
        printf("Erase the %s flash data finish. Start from 0x%08X, size is %d.\r\n", flash->name, addr,
                size);
    } else {
        printf("Erase the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* write test */
    result = sfud_write(flash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        printf("Write the %s flash data finish. Start from 0x%08X, size is %d.\r\n", flash->name, addr,
                size);
    } else {
        printf("Write the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* read test */
    result = sfud_read(flash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        printf("Read the %s flash data success. Start from 0x%08X, size is %d. The data is:\r\n", flash->name, addr,
                size);
        printf("Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (i = 0; i < size; i++) {
            if (i % 16 == 0) {
                printf("[%08X] ", addr + i);
            }
            printf("%02X ", data[i]);
            if (((i + 1) % 16 == 0) || i == size - 1) {
                printf("\r\n");
            }
        }
        printf("\r\n");
    } else {
        printf("Read the %s flash data failed.\r\n", flash->name);
    }
    /* data check */
    for (i = 0; i < size; i++) {
        if (data[i] != i % 256) {
            printf("Read and check write data has an error. Write the %s flash data failed.\r\n", flash->name);
			break;
        }
    }
    if (i == size) {
        printf("The %s flash test is success.\r\n", flash->name);
    }
    printf("\r\n[%s]: exit!\r\n", __FUNCTION__);
}

void test_case_qspi_flash(uint32_t addr, uint32_t size)
{
    printf("\r\n[%s]: entry!\r\n", __FUNCTION__);
    #define QSPI_X       QSPI0
    #define RWLEN        (256)
    uint8_t WrBuff[RWLEN];
    uint8_t RdBuff[RWLEN];
    uint32_t single_size = RWLEN;
    /* prepare write data */
	//printf("\n\nprepare write data: \n");
    for (uint32_t i = 0; i < sizeof(WrBuff) / sizeof(WrBuff[0]); ++i) {
        WrBuff[i] = i;
        //printf("0x%02X, ", WrBuff[i]);
    }
    for (uint32_t i = 0; i < size; ) {
        if ( 0 == ( addr & (4096 - 1) ) ) {
            QSPI_Erase(QSPI_X, QSPI_CMD_ERASE_SECTOR, addr, 1);
            //printf("\n\nAfter Erase: \n");
        }
        //QSPI_Write(QSPI_X, addr, WrBuff, single_size);
        QSPI_Write_4bit(QSPI_X, addr, WrBuff, single_size);
        //printf("\n\nAfter Write: \n");

        /* 测试多种读取方式 */
        //QSPI_Read(QSPI_X, addr, RdBuff, single_size);
        //QSPI_Read_2bit(QSPI_X, addr, RdBuff, single_size);
        //QSPI_Read_4bit(QSPI_X, addr, RdBuff, single_size);
        //QSPI_Read_IO4bit(QSPI_X, addr, RdBuff, single_size);
        //qspi_dma_read(addr, RdBuff, single_size, 1, 1);
        //qspi_dma_read(addr, RdBuff, single_size, 1, 2);
        qspi_dma_read(addr, RdBuff, single_size, 4, 4);
        //printf("\n\nAfter Read: \n");
    
        if (0 != memcmp(WrBuff, RdBuff, single_size)) {
            printf("error: addr = 0x[%x], size = %d!\r\n", addr, single_size);
            for (uint32_t i = 0; i < single_size; ++i) {
                printf("[%d]: wr = 0x%02X, rd = 0x%02X \r\n", i, WrBuff[i], RdBuff[i]);
            }
            //continue;
            while (1) __NOP();
        }
        printf("\n\nAfter memcmp: \n");
        addr += single_size;
        i += single_size;
    }
    printf("\r\n[%s]: exit!\r\n", __FUNCTION__);
}
