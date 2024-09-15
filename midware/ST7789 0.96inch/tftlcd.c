#include "hc32_ddl.h"
#include "tftlcd.h"
#include "tft_font.h"
//#include "spi.h"

// /* use dma transfer spi data */
//#define SPI_TRANS_DMA


//LCD�����С���ã��޸Ĵ�ֵʱ��ע�⣡�������޸�������ֵʱ���ܻ�Ӱ�����º���	LCD_Clear/LCD_Fill/LCD_DrawLine
#define LCD_TOTAL_BUF_SIZE	(160*80*2)
#define LCD_Buf_Size 1152
static uint8_t lcd_buf[LCD_Buf_Size];

uint16_t	POINT_COLOR = CYAN;	//������ɫ	Ĭ��Ϊ��ɫ
uint16_t	BACK_COLOR 	= BLACK;	//������ɫ	Ĭ��Ϊ��ɫ



/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Configure SPI peripheral function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Spi_Config(void)
{
    stc_spi_init_t stcSpiInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcSpiInit);

    /* Configuration peripheral clock */
    PWC_Fcg1PeriphClockCmd(SPI_UNIT_CLOCK, Enable);

    /* Configuration SPI pin */
    PORT_SetFunc(SPI_SCK_PORT, SPI_SCK_PIN, SPI_SCK_FUNC, Disable);
    PORT_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, SPI_MOSI_FUNC, Disable);
//    PORT_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, SPI_MISO_FUNC, Disable);

    /* Configuration SPI structure */
    stcSpiInit.enClkDiv = SpiClkDiv2;
    stcSpiInit.enFrameNumber = SpiFrameNumber1;
    stcSpiInit.enDataLength = SpiDataLengthBit8;
    stcSpiInit.enFirstBitPosition = SpiFirstBitPositionMSB;
    stcSpiInit.enSckPolarity = SpiSckIdleLevelHigh;
    stcSpiInit.enSckPhase = SpiSckOddChangeEvenSample;
    stcSpiInit.enReadBufferObject = SpiReadReceiverBuffer;
    stcSpiInit.enWorkMode = SpiWorkMode3Line;
    stcSpiInit.enTransMode = SpiTransOnlySend;
    stcSpiInit.enCommAutoSuspendEn = Disable;
    stcSpiInit.enModeFaultErrorDetectEn = Disable;
    stcSpiInit.enParitySelfDetectEn = Disable;
    stcSpiInit.enParityEn = Disable;
    stcSpiInit.enParity = SpiParityEven;


    stcSpiInit.enMasterSlaveMode = SpiModeMaster;
    stcSpiInit.stcDelayConfig.enSsSetupDelayOption = SpiSsSetupDelayCustomValue;
    stcSpiInit.stcDelayConfig.enSsSetupDelayTime = SpiSsSetupDelaySck1;
    stcSpiInit.stcDelayConfig.enSsHoldDelayOption = SpiSsHoldDelayCustomValue;
    stcSpiInit.stcDelayConfig.enSsHoldDelayTime = SpiSsHoldDelaySck1;
    stcSpiInit.stcDelayConfig.enSsIntervalTimeOption = SpiSsIntervalCustomValue;
    stcSpiInit.stcDelayConfig.enSsIntervalTime = SpiSsIntervalSck6PlusPck2;

    SPI_Init(SPI_UNIT, &stcSpiInit);
}

/**
 *******************************************************************************
 ** \brief Configure SPI DMA function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Spi_DmaConfig(void)
{
    stc_dma_config_t stcDmaCfg;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcDmaCfg);

    /* Configuration peripheral clock */
    PWC_Fcg0PeriphClockCmd(SPI_DMA_CLOCK_UNIT, Enable);
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_AOS, Enable);

    /* Configure TX DMA */
    stcDmaCfg.u16BlockSize = 1u;
		stcDmaCfg.u16TransferCnt = 1;
		stcDmaCfg.u32SrcAddr = (uint32_t)(0);
    stcDmaCfg.u32DesAddr = (uint32_t)(&SPI_UNIT->DR);
    stcDmaCfg.stcDmaChCfg.enSrcInc = AddressIncrease;
    stcDmaCfg.stcDmaChCfg.enDesInc = AddressFix;
    stcDmaCfg.stcDmaChCfg.enTrnWidth = Dma8Bit;
    stcDmaCfg.stcDmaChCfg.enIntEn = Disable;
    DMA_InitChannel(SPI_DMA_UNIT, SPI_DMA_TX_CHANNEL, &stcDmaCfg);

    DMA_SetTriggerSrc(SPI_DMA_UNIT, SPI_DMA_TX_CHANNEL, SPI_DMA_TX_TRIG_SOURCE);

    /* Enable DMA. */
    DMA_Cmd(SPI_DMA_UNIT, Enable);
}


/**
 * @brief	LCD���ƽӿڳ�ʼ��
 *
 * @param   void
 *
 * @return  void
 */
void lcd_gpio_init(void)
{
		Spi_Config();
    Spi_DmaConfig();

		/* ʧ��TDI TDO */
		PORT_DebugPortSetting(TDI,Disable);
		PORT_DebugPortSetting(TDO_SWO,Disable);			
	
    stc_port_init_t stcPortInit;
    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = Pin_Mode_Out;
    stcPortInit.enPullUp = Enable;
    PORT_Init(LCD_DC_GPIO_Port, LCD_DC_Pin, &stcPortInit);
    PORT_Init(LCD_RST_GPIO_Port, LCD_RST_Pin, &stcPortInit);
    PORT_Init(LCD_PWR_GPIO_Port, LCD_PWR_Pin, &stcPortInit);

		PORT_SetBits(LCD_DC_GPIO_Port, LCD_DC_Pin);
		PORT_SetBits(LCD_RST_GPIO_Port, LCD_RST_Pin);
		PORT_SetBits(LCD_PWR_GPIO_Port, LCD_PWR_Pin);			
	
}

static void lcd_spi_trans(uint8_t *dat, uint16_t len)
{
    DMA_SetSrcAddress (SPI_DMA_UNIT, SPI_DMA_TX_CHANNEL, (uint32_t)dat);
    DMA_SetTransferCnt(SPI_DMA_UNIT, SPI_DMA_TX_CHANNEL, len);
    
    /* Enable DMA channel */
    DMA_ChannelCmd(SPI_DMA_UNIT, SPI_DMA_TX_CHANNEL, Enable);
        
    /* Enable SPI to start DMA */
    SPI_Cmd(SPI_UNIT, Enable);   
		while (Reset == DMA_GetIrqFlag(SPI_DMA_UNIT, SPI_DMA_TX_CHANNEL, TrnCpltIrq))
		{
		}
		DMA_ClearIrqFlag(SPI_DMA_UNIT, SPI_DMA_TX_CHANNEL, TrnCpltIrq);

		/* Disable SPI */
		SPI_Cmd(SPI_UNIT, Disable);
}

/**
 * @brief	LCD�ײ�SPI�������ݺ���
 *
 * @param   data	���ݵ���ʼ��ַ
 * @param   size	�������ݴ�С
 *
 * @return  void
 */
static void LCD_SPI_Send(uint8_t *data, uint16_t size)
{
    lcd_spi_trans(data, size);
}

/**
 * @brief	д���LCD
 *
 * @param   cmd		��Ҫ���͵�����
 *
 * @return  void
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_DC(0);

    LCD_SPI_Send(&cmd, 1);
}

/**
 * @brief	д���ݵ�LCD
 *
 * @param   cmd		��Ҫ���͵�����
 *
 * @return  void
 */
static void LCD_Write_Data(uint8_t data)
{
    LCD_DC(1);

    LCD_SPI_Send(&data, 1);
}

/**
 * @brief	д����ֵ����ݵ�LCD
 *
 * @param   cmd		��Ҫ���͵�����
 *
 * @return  void
 */
void LCD_Write_HalfWord(const uint16_t da)
{
    uint8_t data[2] = {0};

    data[0] = da >> 8;
    data[1] = da;

    LCD_DC(1);
    LCD_SPI_Send(data, 2);
}


/**
 * ��������д��LCD��������
 *
 * @param   x1,y1	�������
 * @param   x2,y2	�յ�����
 *
 * @return  void
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1+24);
		LCD_Write_HalfWord(x2+24);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1);
		LCD_Write_HalfWord(y2);
		LCD_Write_Cmd(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1+24);
		LCD_Write_HalfWord(x2+24);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1);
		LCD_Write_HalfWord(y2);
		LCD_Write_Cmd(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1);
		LCD_Write_HalfWord(x2);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1+24);
		LCD_Write_HalfWord(y2+24);
		LCD_Write_Cmd(0x2c);//������д
	}
	else
	{
		LCD_Write_Cmd(0x2a);//�е�ַ����
		LCD_Write_HalfWord(x1);
		LCD_Write_HalfWord(x2);
		LCD_Write_Cmd(0x2b);//�е�ַ����
		LCD_Write_HalfWord(y1+24);
		LCD_Write_HalfWord(y2+24);
		LCD_Write_Cmd(0x2c);//������д
	}
}

/**
 * ��LCD��ʾ
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOn(void)
{
    LCD_PWR(0);
}
/**
 * �ر�LCD��ʾ
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOff(void)
{
    LCD_PWR(1);
}

/**
 * ��һ����ɫ���LCD��
 *
 * @param   color	������ɫ
 *
 * @return  void
 */
void LCD_Clear(uint16_t color)
{
//    uint16_t i, j;
//    uint8_t data[2] = {0};

//    data[0] = color >> 8;
//    data[1] = color;

//    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

//    for(j = 0; j < LCD_Buf_Size / 2; j++)
//    {
//        lcd_buf[j * 2] =  data[0];
//        lcd_buf[j * 2 + 1] =  data[1];
//    }

//    LCD_DC(1);

//    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
//    {
//        LCD_SPI_Send(lcd_buf, LCD_Buf_Size);
//    }
	LCD_Fill(0,0,LCD_Width,LCD_Height,color);
}

/**
 * ��һ����ɫ�����������
 *
 * @param   x_start,y_start     �������
 * @param   x_end,y_end			�յ�����
 * @param   color       		�����ɫ
 *
 * @return  void
 */
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_Buf_Size)
    {
        size_remain = size - LCD_Buf_Size;
        size = LCD_Buf_Size;
    }

    LCD_Address_Set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_Buf_Size)
        {
            size_remain = size_remain - LCD_Buf_Size;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}

/**
 * ���㺯��
 *
 * @param   x,y		��������
 *
 * @return  void
 */
void LCD_Draw_Point(uint16_t x, uint16_t y)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_HalfWord(POINT_COLOR);
}
void LCD_Draw_Point1(uint16_t x, uint16_t y,uint8_t t)
{
    LCD_Address_Set(x, y, x, y);
	if(t==1)
    LCD_Write_HalfWord(POINT_COLOR);
	if(t==0)
		LCD_Write_HalfWord(BACK_COLOR);
}
/**
 * �������ɫ����
 *
 * @param   x,y		��������
 *
 * @return  void
 */
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_Write_HalfWord(color);
}

/**
 * @brief	���ߺ���(ֱ�ߡ�б��)
 *
 * @param   x1,y1	�������
 * @param   x2,y2	�յ�����
 *
 * @return  void
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i = 0;

    if(y1 == y2)
    {
        /*���ٻ�ˮƽ��*/
        LCD_Address_Set(x1, y1, x2, y2);

        for(i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = POINT_COLOR >> 8;
            lcd_buf[2 * i + 1] = POINT_COLOR;
        }

        LCD_DC(1);
        LCD_SPI_Send(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if(delta_x > 0)incx = 1;

    else if(delta_x == 0)incx = 0;

    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)incy = 1;

    else if(delta_y == 0)incy = 0;

    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)distance = delta_x;

    else distance = delta_y;

    for(t = 0; t <= distance + 1; t++)
    {
        LCD_Draw_Point(row, col);
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief	��һ������
 *
 * @param   x1,y1	�������
 * @param   x2,y2	�յ�����
 *
 * @return  void
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}

/**
 * @brief	��һ��Բ
 *
 * @param   x0,y0	Բ������
 * @param   r       Բ�뾶
 *
 * @return  void
 */
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while(a <= b)
    {
        LCD_Draw_Point(x0 - b, y0 - a);
        LCD_Draw_Point(x0 + b, y0 - a);
        LCD_Draw_Point(x0 - a, y0 + b);
        LCD_Draw_Point(x0 - b, y0 - a);
        LCD_Draw_Point(x0 - a, y0 - b);
        LCD_Draw_Point(x0 + b, y0 + a);
        LCD_Draw_Point(x0 + a, y0 - b);
        LCD_Draw_Point(x0 + a, y0 + b);
        LCD_Draw_Point(x0 - b, y0 + a);
        a++;

        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }

        LCD_Draw_Point(x0 + a, y0 + b);
    }
}

/**
 * @brief	��ʾһ��ASCII���ַ�
 *
 * @param   x,y		��ʾ��ʼ����
 * @param   chr		��Ҫ��ʾ���ַ�
 * @param   size	�����С(֧��16/24/32������)
 *
 * @return  void
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size)
{
    uint8_t temp, t1, t;
    uint8_t csize;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    uint16_t colortemp;
    uint8_t sta;

    chr = chr - ' '; //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩

    if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;

    LCD_Address_Set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

    if((size == 16) || (size == 32) )	//16��32������
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == 16)temp = asc2_1608[chr][t];	//����1608����
            else if(size == 32)temp = asc2_3216[chr][t];	//����3216����
            else return;			//û�е��ֿ�

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

	else if  (size == 12)	//12������
	{
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[chr][t];

            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
	
    else if(size == 24)		//24������
    {
        csize = (size * 16) / 8;

        for(t = 0; t < csize; t++)
        {
            temp = asc2_2412[chr][t];

            if(t % 2 == 0)sta = 8;
            else sta = 4;

            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                LCD_Write_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
}

/**
 * @brief	m^n����
 *
 * @param   m,n		�������
 *
 * @return  m^n�η�
 */
static uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while(n--)result *= m;

    return result;
}

/**
 * @brief	��ʾ����,��λΪ0����ʾ
 *
 * @param   x,y		�������
 * @param   num		��Ҫ��ʾ������,���ַ�Χ(0~4294967295)
 * @param   len		��Ҫ��ʾ��λ��
 * @param   size	�����С
 *
 * @return  void
 */
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size);
                continue;
            }

            else enshow = 1;
        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size);
    }
}



/**
 * @brief	��ʾ����,��λΪ0,���Կ�����ʾΪ0���ǲ���ʾ
 *
 * @param   x,y		�������
 * @param   num		��Ҫ��ʾ������,���ַ�Χ(0~999999999)
 * @param   len		��Ҫ��ʾ��λ��
 * @param   size	�����С
 * @param   mode	1:��λ��ʾ0		0:��λ����ʾ
 *
 * @return  void
 */
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                if(mode)LCD_ShowChar(x + (size / 2)*t, y, '0', size);
                else
                    LCD_ShowChar(x + (size / 2)*t, y, ' ', size);

                continue;
            }

            else enshow = 1;
        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size);
    }
}


/**
 * @brief	��ʾ�ַ���
 *
 * @param   x,y		�������
 * @param   width	�ַ���ʾ������
 * @param   height	�ַ���ʾ����߶�
 * @param   size	�����С
 * @param   p		�ַ�����ʼ��ַ
 *
 * @return  void
 */
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
    {
        if(x >= width)
        {
            x = x0;
            y += size;
        }

        if(y >= height)break; //�˳�

        LCD_ShowChar(x, y, *p, size);
        x += size / 2;
        p++;
    }
}


/**
 * @brief	��ʾͼƬ
 *
 * @remark	Image2Lcdȡģ��ʽ��	C��������/ˮƽɨ��/16λ���ɫ(RGB565)/��λ��ǰ		�����Ĳ�Ҫѡ
 *
 * @param   x,y		�������
 * @param   width	ͼƬ���
 * @param   height	ͼƬ�߶�
 * @param   p		ͼƬ����������ʼ��ַ
 *
 * @return  void
 */
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_DC(1);

    LCD_SPI_Send((uint8_t *)p, width * height * 2);
}



/**
 * @brief	LCD��ʼ��
 *
 * @param   void
 *
 * @return  void
 */
 void LCD_Init(void)
{
//	HAL_Delay(120);

		lcd_gpio_init();
	
    LCD_RST(0);
    Ddl_Delay1ms(200);
    LCD_RST(1);
    Ddl_Delay1ms(20);	
    LCD_PWR(0);

		LCD_Write_Cmd(0x11); 
		Ddl_Delay1ms(100);

//		LCD_Write_Cmd(0x21); 

		LCD_Write_Cmd(0xB1); 
		LCD_Write_Data(0x05);
		LCD_Write_Data(0x3C);
		LCD_Write_Data(0x3C);

		LCD_Write_Cmd(0xB2);
		LCD_Write_Data(0x05);
		LCD_Write_Data(0x3C);
		LCD_Write_Data(0x3C);

		LCD_Write_Cmd(0xB3); 
		LCD_Write_Data(0x05);  
		LCD_Write_Data(0x3C);
		LCD_Write_Data(0x3C);
		LCD_Write_Data(0x05);
		LCD_Write_Data(0x3C);
		LCD_Write_Data(0x3C);

		LCD_Write_Cmd(0xB4);
		LCD_Write_Data(0x03);

		LCD_Write_Cmd(0xC0);
		LCD_Write_Data(0x0E);
		LCD_Write_Data(0x0E);
		LCD_Write_Data(0x04);

		LCD_Write_Cmd(0xC1);
		LCD_Write_Data(0xC5);

		LCD_Write_Cmd(0xC2);
		LCD_Write_Data(0x0D);
		LCD_Write_Data(0x00);

		LCD_Write_Cmd(0xC3);
		LCD_Write_Data(0x8D);
		LCD_Write_Data(0x2A);   

		LCD_Write_Cmd(0xC4);
		LCD_Write_Data(0x8D); 
		LCD_Write_Data(0x06); 

		LCD_Write_Cmd(0xC5);  /*VCOM*/
		LCD_Write_Data(0x06);    

		LCD_Write_Cmd(0x36);     //MX, MY, RGB mode
		if(USE_HORIZONTAL==0)LCD_Write_Data(0x08);
		else if(USE_HORIZONTAL==1)LCD_Write_Data(0xC8);
		else if(USE_HORIZONTAL==2)LCD_Write_Data(0x78);
		else LCD_Write_Data(0xA8);  

		LCD_Write_Cmd(0x3A);
		LCD_Write_Data(0x55);

		LCD_Write_Cmd(0xE0);
		LCD_Write_Data(0x0b);
		LCD_Write_Data(0x17);
		LCD_Write_Data(0x0a);
		LCD_Write_Data(0x0d);
		LCD_Write_Data(0x1a);
		LCD_Write_Data(0x19);
		LCD_Write_Data(0x16);
		LCD_Write_Data(0x1d);
		LCD_Write_Data(0x21);
		LCD_Write_Data(0x26);
		LCD_Write_Data(0x37);
		LCD_Write_Data(0x3c);
		LCD_Write_Data(0x00);
		LCD_Write_Data(0x09);
		LCD_Write_Data(0x05);
		LCD_Write_Data(0x10);

		LCD_Write_Cmd(0xE1);
		LCD_Write_Data(0x0c);
		LCD_Write_Data(0x19);
		LCD_Write_Data(0x09);
		LCD_Write_Data(0x0d);
		LCD_Write_Data(0x1b);
		LCD_Write_Data(0x19);
		LCD_Write_Data(0x15);
		LCD_Write_Data(0x1d);
		LCD_Write_Data(0x21);
		LCD_Write_Data(0x26);
		LCD_Write_Data(0x39);
		LCD_Write_Data(0x3E);
		LCD_Write_Data(0x00);
		LCD_Write_Data(0x09);
		LCD_Write_Data(0x05);
		LCD_Write_Data(0x10);

		Ddl_Delay1ms(120);
		LCD_Write_Cmd(0x29); 

}



