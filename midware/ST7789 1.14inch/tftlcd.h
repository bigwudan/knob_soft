#ifndef __LCD_H
#define __LCD_H
#include "hc32_ddl.h"
#include "stdio.h"
extern uint16_t	POINT_COLOR;	//默认画笔颜色
extern uint16_t	BACK_COLOR;		//默认背景颜色

//LCD的宽和高定义
#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_Width 135
#define LCD_Height 240

#else
#define LCD_Width 240
#define LCD_Height 135
#endif

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


/* SPI_SCK Port/Pin definition */
#define SPI_SCK_PORT                    (PortB)
#define SPI_SCK_PIN                     (Pin06)
#define SPI_SCK_FUNC                    (Func_Spi3_Sck)

/* SPI_MOSI Port/Pin definition */
#define SPI_MOSI_PORT                   (PortB)
#define SPI_MOSI_PIN                    (Pin07)
#define SPI_MOSI_FUNC                   (Func_Spi3_Mosi)

/* SPI unit and clock definition */
#define SPI_UNIT                        (M4_SPI3)
#define SPI_UNIT_CLOCK                  (PWC_FCG1_PERIPH_SPI3)

/* SPI DMA unit and channel definition */
#define SPI_DMA_UNIT                    (M4_DMA1)
#define SPI_DMA_CLOCK_UNIT              (PWC_FCG0_PERIPH_DMA1)
#define SPI_DMA_TX_CHANNEL              (DmaCh1)
#define SPI_DMA_RX_CHANNEL              (DmaCh0)
#define SPI_DMA_TX_TRIG_SOURCE          (EVT_SPI3_SPTI)
#define SPI_DMA_RX_TRIG_SOURCE          (EVT_SPI3_SPRI)

#define LCD_DC_GPIO_Port                  (PortB)
#define LCD_DC_Pin                        (Pin08)

#define LCD_RST_GPIO_Port                 (PortB)
#define LCD_RST_Pin                       (Pin09)

#define LCD_PWR_GPIO_Port                 (PortB)
#define LCD_PWR_Pin                       (Pin03)


#define	LCD_PWR(n)		(n?PORT_SetBits(LCD_PWR_GPIO_Port,LCD_PWR_Pin):PORT_ResetBits(LCD_PWR_GPIO_Port,LCD_PWR_Pin))
#define	LCD_RST(n)		(n?PORT_SetBits(LCD_RST_GPIO_Port,LCD_RST_Pin):PORT_ResetBits(LCD_RST_GPIO_Port,LCD_RST_Pin))
#define	LCD_DC(n)			(n?PORT_SetBits(LCD_DC_GPIO_Port,LCD_DC_Pin):PORT_ResetBits(LCD_DC_GPIO_Port,LCD_DC_Pin))


void LCD_Init(void);																	//初始化
void LCD_DisplayOn(void);																//开显示
void LCD_DisplayOff(void);																//关显示
void LCD_Write_HalfWord(const uint16_t da);													//写半个字节数据到LCD
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);									//设置数据显示区域
void LCD_Clear(uint16_t color);																//清屏
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);				//填充单色
void LCD_Draw_Point(uint16_t x, uint16_t y);														//画点
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color);										//画带颜色点
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);										//画线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);									//画矩形
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);												//画圆
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size);										//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);									//显示一个数字
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);							//显示数字
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p);					//显示字符串
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);					//显示图片
void Display_ALIENTEK_LOGO(uint16_t x,uint16_t y);												//显示ALIENTEK LOGO
void LCD_Draw_Point1(uint16_t x, uint16_t y,uint8_t t);



#endif


