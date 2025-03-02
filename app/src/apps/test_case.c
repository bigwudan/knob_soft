
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

const unsigned char gImage_av40_40[3200] = { /* 0X00,0X10,0X28,0X00,0X28,0X00,0X01,0X1B, */
0X42,0X00,0X01,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X08,0X40,0X10,0X00,0X00,
0X00,0X00,0X00,0X00,0X61,0X08,0X00,0X00,0X20,0X00,0X20,0X00,0X24,0X21,0X6D,0X6B,
0X75,0XAD,0X75,0XAD,0X59,0XCE,0X7D,0XEF,0X9E,0XF7,0X18,0XC6,0XD6,0XB5,0X54,0XA5,
0X4C,0X6B,0X44,0X29,0X00,0X00,0X81,0X08,0X00,0X00,0X00,0X00,0X40,0X00,0X00,0X00,
0X20,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X01,0X00,0X42,0X00,0X00,0X00,0X00,0X00,0X20,0X08,0X00,0X00,0X00,0X00,0X61,0X10,
0X81,0X10,0X00,0X00,0X00,0X00,0X82,0X08,0X0B,0X5B,0X38,0XC6,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XFF,0XFF,0XDE,0XFF,0X9E,0XF7,0XDF,0XFF,0XFF,0XFF,
0XDE,0XFF,0XDE,0XF7,0X79,0XCE,0XA9,0X52,0XC2,0X18,0X60,0X08,0X00,0X00,0X40,0X00,
0X40,0X00,0X00,0X00,0X20,0X00,0X81,0X08,0X00,0X00,0X00,0X00,0X41,0X08,0X00,0X00,
0X00,0X00,0X21,0X00,0X21,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X08,0X00,0X00,
0X00,0X00,0XA2,0X10,0X8D,0X6B,0X3C,0XE7,0XFF,0XFF,0XFF,0XF7,0XBD,0XF7,0XDE,0XFF,
0X5C,0XEF,0XDE,0XFF,0X5C,0XEF,0XDE,0XFF,0X7D,0XEF,0XDE,0XF7,0XBE,0XF7,0X9D,0XF7,
0X7D,0XEF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XFB,0XDE,0XCB,0X5A,0X61,0X08,0X00,0X00,
0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X20,0X00,0X00,0X00,0X00,0X00,
0X21,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X28,0X42,0X9A,0XCE,0XFF,0XF7,0XDF,0XF7,0X7D,0XEF,0X9D,0XEF,0X7C,0XEF,0X9D,0XEF,
0X7C,0XEF,0XDE,0XFF,0X5C,0XEF,0X5C,0XEF,0X1B,0XE7,0X1B,0XE7,0X1B,0XE7,0XFB,0XDE,
0XD6,0XBD,0X3C,0XE7,0XFC,0XDE,0XDF,0XFF,0XFF,0XFF,0X5E,0XEF,0X19,0XC6,0X66,0X29,
0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X08,0X41,0X08,0X00,0X00,0X82,0X08,0X6E,0X63,
0X7E,0XE7,0XFF,0XFF,0X1C,0XDF,0X5D,0XE7,0XBE,0XF7,0X9D,0XEF,0X7C,0XEF,0XFE,0XFF,
0X9C,0XF7,0XBD,0XF7,0X5C,0XEF,0X3B,0XE7,0X5C,0XEF,0X79,0XCE,0X58,0XCE,0X38,0XC6,
0X59,0XCE,0XD8,0XC5,0X5A,0XCE,0X1D,0XE7,0X7B,0XD6,0X7F,0XF7,0XFF,0XFF,0X7F,0XF7,
0XEC,0X5A,0X61,0X08,0X00,0X00,0X61,0X08,0X00,0X00,0X00,0X00,0X61,0X08,0X00,0X00,
0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC4,0X10,0X92,0X84,0XFF,0XEF,
0XDF,0XEF,0XBB,0XCE,0X59,0XBE,0X5D,0XE7,0X5C,0XE7,0X9D,0XF7,0XDE,0XFF,0X5C,0XEF,
0X5C,0XEF,0X7C,0XEF,0X7D,0XEF,0X5C,0XEF,0X7C,0XEF,0X95,0XB5,0XD3,0X9C,0X92,0X94,
0XD7,0XBD,0XF9,0XC5,0XD4,0X9C,0XF5,0XA4,0X16,0XA5,0X3A,0XCE,0XDD,0XE6,0X5F,0XEF,
0X9F,0XF7,0X4D,0X63,0X41,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X21,0X00,0X00,0X00,
0X00,0X08,0X00,0X08,0X41,0X08,0X00,0X00,0X42,0X00,0X93,0X84,0XFF,0XF7,0XBF,0XE7,
0XF4,0X8C,0XD3,0X8C,0XDB,0XCE,0XFF,0XF7,0X7D,0XEF,0X7C,0XEF,0XBE,0XFF,0X5C,0XEF,
0XBE,0XF7,0X9E,0XF7,0X9A,0XD6,0X18,0XC6,0XD7,0XBD,0X92,0X94,0X10,0X84,0X52,0X8C,
0XF1,0X83,0XB8,0XBD,0X32,0X84,0X0D,0X63,0X6C,0X52,0X0A,0X42,0XF2,0X83,0X1A,0XC6,
0XFF,0XFF,0X1C,0XDF,0X4D,0X63,0X41,0X00,0X21,0X00,0X21,0X00,0X00,0X00,0X00,0X00,
0X00,0X08,0X00,0X08,0X00,0X00,0X00,0X00,0X4E,0X5B,0XDF,0XEF,0XFF,0XEF,0X73,0X7C,
0X11,0X6C,0XB3,0X84,0XFB,0XD6,0X5D,0XE7,0XBE,0XF7,0X9D,0XF7,0X1B,0XE7,0XBA,0XD6,
0X72,0X8C,0X93,0X94,0X4E,0X63,0X2E,0X63,0X2E,0X63,0X4E,0X63,0X4E,0X6B,0X32,0X84,
0X4A,0X4A,0X4A,0X42,0X6B,0X4A,0X46,0X21,0XC9,0X39,0X68,0X31,0X88,0X31,0XB0,0X73,
0X36,0XA5,0XFF,0XFF,0X7E,0XEF,0XAB,0X52,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X00,
0X00,0X08,0X00,0X08,0X41,0X08,0X87,0X29,0X5E,0XDF,0XDF,0XE7,0X93,0X7C,0X6F,0X53,
0X52,0X74,0X97,0XA5,0X1C,0XDF,0XBE,0XEF,0X5C,0XE7,0XDE,0XFF,0XB6,0XB5,0X4D,0X6B,
0X2E,0X63,0XD1,0X73,0XD1,0X73,0X2F,0X63,0X0A,0X3A,0X88,0X29,0X6B,0X4A,0X0E,0X5B,
0X4B,0X42,0X47,0X21,0X4B,0X42,0X06,0X19,0X26,0X19,0X26,0X21,0XA4,0X08,0X0A,0X3A,
0XCC,0X5A,0X52,0X8C,0XDF,0XF7,0X3D,0XE7,0XC3,0X10,0X00,0X00,0X00,0X00,0X00,0X00,
0X40,0X10,0X00,0X08,0X00,0X00,0XF8,0XB5,0XFF,0XF7,0X98,0X9D,0X4B,0X2A,0X6F,0X53,
0X35,0X95,0XF8,0XAD,0X1C,0XDF,0X3C,0XE7,0XBE,0XF7,0XD7,0XBD,0X51,0X8C,0X4A,0X4A,
0XCA,0X39,0XCA,0X39,0X48,0X29,0X27,0X21,0X07,0X21,0XE6,0X18,0X68,0X29,0X88,0X29,
0X2B,0X42,0XE9,0X39,0XC9,0X31,0XA8,0X29,0X05,0X19,0X46,0X21,0XC4,0X10,0XC8,0X31,
0X6B,0X4A,0X88,0X31,0X77,0XAD,0XFF,0XFF,0X35,0XA5,0X00,0X00,0X41,0X08,0X00,0X00,
0X00,0X08,0X00,0X00,0XAB,0X52,0XFF,0XF7,0X3E,0XD7,0XED,0X42,0X2A,0X2A,0X8F,0X53,
0X35,0X95,0X9E,0XE7,0XFF,0XFF,0X9E,0XF7,0XB6,0XB5,0XEC,0X5A,0XA7,0X31,0X47,0X29,
0X48,0X29,0XE7,0X20,0X65,0X10,0XA5,0X18,0XE6,0X18,0XC6,0X18,0X06,0X21,0XE6,0X18,
0X47,0X21,0XA8,0X31,0X26,0X19,0XE9,0X31,0X67,0X21,0X05,0X19,0X46,0X19,0X67,0X21,
0X2B,0X42,0XC5,0X10,0XAC,0X52,0X5E,0XEF,0XBF,0XF7,0X4A,0X4A,0X00,0X00,0X00,0X00,
0X00,0X00,0X20,0X00,0X9B,0XCE,0XDF,0XE7,0XF5,0X84,0X4B,0X2A,0X0D,0X43,0X4E,0X4B,
0X35,0X95,0X7D,0XE7,0X5D,0XEF,0X38,0XC6,0X30,0X84,0XCF,0X7B,0X0D,0X5B,0XAC,0X52,
0X47,0X31,0X27,0X29,0XA5,0X20,0XA5,0X18,0X64,0X10,0X43,0X08,0XC5,0X18,0X06,0X21,
0XA4,0X10,0XE5,0X18,0XC5,0X18,0X46,0X21,0X87,0X29,0XA4,0X08,0XA8,0X29,0X47,0X21,
0X0A,0X3A,0X47,0X29,0X06,0X21,0X93,0X8C,0XBF,0XF7,0X39,0XC6,0X00,0X00,0X21,0X00,
0X40,0X08,0X8A,0X4A,0XFF,0XEF,0X7F,0XD7,0XD1,0X5B,0X4F,0X4B,0XCC,0X3A,0XAF,0X5B,
0XFB,0XCE,0X9E,0XEF,0XFF,0XFF,0X50,0X8C,0X49,0X4A,0X8B,0X52,0XED,0X5A,0X6B,0X4A,
0X05,0X29,0X05,0X31,0XA4,0X20,0XA4,0X20,0X83,0X20,0X83,0X18,0XE5,0X28,0X26,0X29,
0XC4,0X18,0X83,0X10,0XC4,0X18,0XA4,0X10,0XC4,0X18,0X83,0X08,0X87,0X29,0X88,0X29,
0XA9,0X31,0XE6,0X18,0XC5,0X18,0XE9,0X39,0X3E,0XE7,0XDF,0XF7,0XC7,0X39,0X20,0X00,
0X00,0X00,0XD7,0XB5,0XFF,0XEF,0X19,0XA6,0X12,0X64,0X90,0X4B,0X2D,0X43,0XF8,0XA5,
0XFF,0XF7,0XFB,0XDE,0XBA,0XDE,0X6D,0X73,0X66,0X31,0X09,0X42,0X88,0X29,0XE6,0X20,
0X83,0X20,0XC3,0X30,0XE3,0X30,0XA6,0X49,0X49,0X5A,0X49,0X5A,0XE8,0X49,0X05,0X29,
0XC4,0X20,0X63,0X10,0X83,0X10,0XE5,0X20,0X42,0X08,0XC5,0X18,0X06,0X19,0X27,0X21,
0X47,0X29,0XE6,0X18,0XC5,0X10,0X05,0X19,0XB4,0X94,0XFF,0XFF,0XF4,0X9C,0X00,0X00,
0X82,0X08,0XBE,0XE7,0XFF,0XE7,0X52,0X6C,0X53,0X64,0X6F,0X4B,0XF1,0X5B,0X3D,0XCF,
0X7D,0XE7,0X3C,0XE7,0X58,0XD6,0X30,0X8C,0X05,0X29,0X06,0X21,0X84,0X08,0XA8,0X31,
0XAA,0X6A,0X4C,0X8B,0XEE,0X9B,0XD2,0XB4,0X95,0XCD,0X95,0XCD,0XB3,0XAC,0X4D,0X7B,
0X87,0X41,0XE5,0X28,0XA4,0X18,0X06,0X29,0X43,0X10,0XC5,0X18,0XE6,0X20,0XC5,0X18,
0XE6,0X20,0X48,0X29,0X84,0X10,0XE5,0X18,0XC8,0X39,0XBF,0XF7,0X1C,0XE7,0XA2,0X10,
0X8A,0X42,0XFF,0XEF,0X5D,0XD7,0X11,0X64,0X73,0X6C,0XD1,0X53,0X8F,0X53,0X5A,0XB6,
0X9E,0XE7,0XBA,0XD6,0X14,0XA5,0X8A,0X52,0X87,0X39,0XC4,0X18,0X27,0X21,0X4A,0X4A,
0X50,0XA4,0X53,0XCD,0XD5,0XD5,0X16,0XDE,0X37,0XDE,0X58,0XE6,0X17,0XD6,0X14,0XB5,
0X4D,0X7B,0X08,0X52,0X25,0X31,0X83,0X18,0X63,0X10,0X43,0X10,0X68,0X31,0XC5,0X18,
0XA5,0X18,0XE6,0X20,0X84,0X10,0XE5,0X18,0X46,0X21,0X7E,0XEF,0X9E,0XF7,0X29,0X42,
0X71,0X84,0XFF,0XEF,0X9A,0XBE,0X72,0X74,0XD4,0X74,0X94,0X6C,0X4F,0X43,0X11,0X64,
0X7E,0XD7,0X31,0X7C,0X08,0X3A,0XE8,0X41,0X05,0X29,0XC4,0X20,0X26,0X31,0X72,0X9C,
0XD5,0XCD,0X15,0XDE,0X36,0XDE,0X57,0XE6,0X77,0XDE,0X98,0XE6,0X37,0XDE,0XF6,0XCD,
0XB5,0XCD,0X70,0XA4,0X28,0X5A,0X82,0X20,0X83,0X18,0X05,0X21,0XE5,0X20,0X84,0X10,
0XA5,0X18,0XC5,0X18,0XA5,0X10,0XC5,0X18,0X88,0X31,0X16,0XA5,0XFF,0XFF,0X4E,0X63,
0X95,0XA5,0XFF,0XF7,0XF8,0XAD,0X72,0X74,0X36,0X85,0X57,0X85,0XB4,0X6C,0XD5,0X74,
0X3E,0XCF,0X8F,0X5B,0X09,0X32,0XC7,0X31,0X25,0X29,0X62,0X18,0X4E,0X7B,0X96,0XC5,
0X77,0XE6,0XF5,0XD5,0X77,0XE6,0X77,0XE6,0X78,0XE6,0X78,0XDE,0XB9,0XE6,0X16,0XD6,
0X95,0XCD,0XD5,0XCD,0X50,0XA4,0X28,0X52,0X25,0X31,0X46,0X31,0X26,0X29,0XA5,0X10,
0XA5,0X18,0X85,0X10,0XC5,0X18,0X06,0X21,0X88,0X31,0X73,0X8C,0XDF,0XFF,0XF1,0X7B,
0X17,0XB6,0XDF,0XEF,0X96,0X9D,0XF4,0X84,0XB8,0X95,0XB8,0X8D,0X36,0X7D,0X53,0X64,
0XDD,0XBE,0X15,0X8D,0X4A,0X3A,0XC4,0X18,0X87,0X39,0X62,0X18,0XAF,0X83,0X79,0XE6,
0X16,0XDE,0X56,0XE6,0XB8,0XEE,0XD9,0XEE,0X99,0XE6,0X99,0XE6,0XB9,0XEE,0XBA,0XEE,
0X17,0XDE,0XB5,0XCD,0X13,0XB5,0X51,0X9C,0X0D,0X6B,0X66,0X31,0XA4,0X18,0X06,0X21,
0XA5,0X18,0XC5,0X18,0X68,0X29,0X88,0X31,0X88,0X31,0X90,0X73,0XDF,0XFF,0X76,0XAD,
0XFB,0XD6,0XFF,0XF7,0X96,0X9D,0XF4,0X84,0X56,0X85,0X57,0X85,0XB8,0X8D,0XD5,0X74,
0X16,0X85,0XB8,0XA5,0X52,0X84,0X08,0X3A,0XC4,0X20,0XC4,0X20,0X2D,0X73,0X69,0X62,
0XF2,0XB4,0X36,0XDE,0XF6,0XD5,0XB5,0XCD,0XD2,0XAC,0XEE,0X8B,0X07,0X52,0XA5,0X41,
0X2C,0X7B,0XCE,0X8B,0X50,0X9C,0XB2,0XA4,0X52,0X9C,0X0D,0X6B,0X47,0X29,0X64,0X10,
0XA4,0X10,0XA4,0X10,0X06,0X21,0X26,0X21,0X46,0X29,0X4E,0X63,0XDF,0XF7,0XB7,0XB5,
0XBA,0XCE,0XFF,0XF7,0X76,0X95,0XB3,0X74,0XD4,0X74,0XF5,0X74,0X57,0X7D,0X94,0X6C,
0XF1,0X5B,0X52,0X74,0XD4,0X8C,0XCF,0X73,0X6A,0X52,0X05,0X29,0XCB,0X62,0X49,0X5A,
0X64,0X41,0XCD,0X93,0XD6,0XD5,0XB1,0XAC,0XA9,0X62,0XE7,0X49,0X0B,0X73,0XCE,0X8B,
0X13,0XB5,0X54,0XBD,0X75,0XC5,0X55,0XBD,0XB3,0XA4,0X2D,0X73,0X88,0X31,0XE5,0X18,
0XE5,0X18,0XC5,0X10,0XA4,0X10,0XC4,0X10,0X87,0X29,0X31,0X84,0XFF,0XFF,0X35,0XA5,
0XD7,0XAD,0XFF,0XEF,0XB7,0X9D,0X52,0X6C,0X73,0X6C,0X73,0X64,0X33,0X5C,0X90,0X4B,
0X52,0X6C,0XB0,0X5B,0X93,0X84,0X72,0X8C,0X31,0X8C,0XE4,0X28,0X4D,0X73,0XD3,0XAC,
0XA5,0X49,0XA8,0X6A,0X98,0XE6,0X95,0XC5,0XAE,0X8B,0XE7,0X49,0X0B,0X73,0XE3,0X28,
0X03,0X31,0X48,0X5A,0X30,0X9C,0XB6,0XC5,0X34,0XB5,0X0D,0X6B,0X67,0X31,0X46,0X29,
0X26,0X19,0XE9,0X31,0XC8,0X31,0XA8,0X29,0XE8,0X39,0XF4,0X94,0XFF,0XFF,0XB3,0X94,
0X55,0XA5,0XFF,0XF7,0X5A,0XB6,0XF0,0X63,0XF1,0X53,0X53,0X5C,0X32,0X5C,0X94,0X6C,
0X94,0X74,0X73,0X74,0XB7,0XA5,0X35,0XA5,0X0C,0X63,0X86,0X39,0X96,0XBD,0X0F,0X94,
0X8C,0X8B,0X0D,0X94,0XF9,0XF6,0X57,0XDE,0XF2,0XAC,0X6C,0X7B,0X50,0X94,0XA9,0X62,
0X6C,0X83,0X54,0XBD,0X78,0XDE,0XF6,0XCD,0XF3,0XA4,0XF0,0X83,0XEC,0X5A,0X29,0X42,
0X29,0X3A,0XB0,0X6B,0X8F,0X63,0X6E,0X63,0XEC,0X52,0XB7,0XAD,0XBF,0XF7,0XCF,0X73,
0X8E,0X6B,0XFF,0XF7,0XDB,0XC6,0X11,0X64,0XD1,0X53,0X32,0X5C,0XF2,0X53,0X36,0X7D,
0XD4,0X7C,0XB7,0X9D,0XF4,0X94,0X55,0XA5,0X2C,0X63,0X69,0X52,0XFB,0XEE,0X99,0XE6,
0X77,0XE6,0XF9,0XF6,0XF9,0XF6,0XD9,0XEE,0X78,0XDE,0X37,0XCE,0XF6,0XC5,0X37,0XD6,
0X98,0XDE,0XB9,0XE6,0X57,0XD6,0XB5,0XBD,0X34,0XAD,0XF3,0XA4,0XF3,0X9C,0X14,0X9D,
0X72,0X84,0XB7,0XAD,0XF4,0X94,0X14,0X95,0XD3,0X8C,0X5D,0XE7,0XFF,0XFF,0X0C,0X63,
0XC7,0X31,0X9E,0XEF,0X7E,0XDF,0X11,0X64,0XF1,0X5B,0XD1,0X53,0XF5,0X74,0XF9,0X95,
0X15,0X85,0X76,0X9D,0X35,0X95,0X51,0X84,0XCE,0X7B,0XAA,0X5A,0XDB,0XE6,0XB9,0XE6,
0XF9,0XF6,0XF8,0XF6,0X19,0XF7,0XD8,0XE6,0XD9,0XE6,0XB8,0XDE,0XFA,0XE6,0XB8,0XDE,
0XB9,0XDE,0X98,0XDE,0X16,0XCE,0X33,0XAD,0X33,0XAD,0XF6,0XBD,0X54,0XAD,0XF3,0X9C,
0X14,0X95,0X76,0X9D,0XF4,0X94,0X35,0X9D,0XB2,0X8C,0XFF,0XFF,0X7D,0XEF,0X03,0X19,
0X20,0X00,0XBB,0XCE,0XFF,0XF7,0XF4,0X84,0XD0,0X5B,0X73,0X64,0X94,0X6C,0XF9,0X9D,
0X15,0X85,0X52,0X74,0XF3,0X94,0XAE,0X73,0X92,0X94,0X07,0X4A,0X17,0XCE,0XDA,0XEE,
0X97,0XE6,0X19,0XF7,0X3A,0XF7,0XF8,0XE6,0X77,0XD6,0XF9,0XDE,0X1A,0XE7,0XD9,0XDE,
0X37,0XCE,0X36,0XCE,0XD5,0XBD,0X53,0XAD,0X53,0XAD,0X95,0XB5,0X33,0X9D,0XB6,0XAD,
0X92,0X84,0X72,0X7C,0X72,0X84,0X35,0X9D,0XF8,0XB5,0XDF,0XF7,0X18,0XC6,0X20,0X00,
0X00,0X00,0XD3,0X94,0XBF,0XE7,0X5A,0XB6,0X93,0X74,0XF1,0X5B,0X57,0X85,0X15,0X7D,
0X14,0X85,0XD3,0X84,0X2C,0X5B,0X6D,0X6B,0XAE,0X7B,0X85,0X39,0X70,0X9C,0X36,0XD6,
0XF8,0XEE,0X76,0XDE,0XF0,0XAC,0X8F,0X9C,0X16,0XC6,0X78,0XCE,0X36,0XC6,0X98,0XCE,
0X36,0XC6,0XF1,0X9C,0X2F,0X8C,0XD1,0X9C,0X54,0XAD,0XF2,0X9C,0XB6,0XB5,0X34,0X9D,
0X92,0X84,0XB3,0X84,0X30,0X74,0XD3,0X8C,0XFC,0XD6,0XBE,0XF7,0XCF,0X7B,0X00,0X00,
0X00,0X00,0X24,0X19,0XFF,0XF7,0X9F,0XDF,0X73,0X74,0XF1,0X5B,0X56,0X85,0X97,0X95,
0X92,0X74,0XB6,0XA5,0XAE,0X6B,0XCE,0X73,0X07,0X42,0XE6,0X49,0X0B,0X6B,0X77,0XDE,
0XB7,0XE6,0X55,0XD6,0XB0,0XA4,0XF5,0XC5,0XF5,0XBD,0XB9,0XD6,0X95,0XB5,0X37,0XC6,
0X94,0XAD,0XAD,0X73,0XCD,0X7B,0X13,0XA5,0X13,0X9D,0X50,0X84,0X34,0X9D,0X4D,0X5B,
0X10,0X74,0XF0,0X73,0XF4,0X94,0XF4,0X94,0XBF,0XEF,0XDF,0XF7,0X41,0X08,0X00,0X00,
0X81,0X10,0X00,0X00,0X55,0X9D,0XDF,0XE7,0X5A,0XAE,0X77,0X8D,0X77,0X8D,0X72,0X74,
0XF4,0X84,0X14,0X8D,0X71,0X84,0X2B,0X63,0XEA,0X62,0X4B,0X73,0X88,0X62,0X32,0XB5,
0X77,0XDE,0X92,0XBD,0X36,0XCE,0X77,0XD6,0X78,0XCE,0XB5,0XAD,0XF7,0XB5,0X54,0XA5,
0X71,0X8C,0X0F,0X84,0X4C,0X63,0XB2,0X94,0X69,0X42,0X4D,0X63,0XAB,0X4A,0XCC,0X4A,
0XCC,0X4A,0X0D,0X53,0XAF,0X63,0X7A,0XC6,0XFF,0XFF,0X10,0X7C,0X00,0X00,0X00,0X00,
0X00,0X00,0X61,0X08,0XC7,0X31,0XDF,0XE7,0XFF,0XEF,0X15,0X85,0XB4,0X7C,0X72,0X6C,
0X14,0X8D,0X96,0XA5,0X91,0X8C,0X74,0XAD,0XDA,0XE6,0X57,0XD6,0X78,0XDE,0XD9,0XEE,
0XF1,0XAC,0XE8,0X62,0X26,0X4A,0XC9,0X5A,0X58,0XC6,0X71,0X84,0X6E,0X63,0X6E,0X63,
0XF0,0X7B,0X4D,0X63,0XB2,0X8C,0X29,0X42,0X29,0X42,0XAB,0X4A,0X05,0X11,0X4E,0X5B,
0X87,0X21,0XE8,0X29,0X8E,0X63,0XFF,0XFF,0XFC,0XDE,0X21,0X00,0XA3,0X10,0X00,0X00,
0X40,0X08,0X00,0X00,0X00,0X00,0X31,0X74,0XFF,0XEF,0X3D,0XC7,0X93,0X74,0XB3,0X7C,
0X38,0XAE,0XFF,0XEF,0X58,0XC6,0XBD,0XF7,0X7C,0XF7,0X36,0XD6,0X3A,0XF7,0X98,0XE6,
0X57,0XDE,0X7B,0XFF,0XB5,0XBD,0X17,0XC6,0XF3,0X9C,0X6D,0X63,0X86,0X29,0XB3,0X8C,
0X09,0X3A,0X93,0X8C,0XC8,0X31,0X11,0X7C,0X8B,0X4A,0X4B,0X42,0X0A,0X32,0XA9,0X29,
0XE5,0X10,0XE8,0X31,0XFC,0XD6,0XFF,0XFF,0X8F,0X73,0X42,0X08,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XA2,0X08,0X9A,0XBE,0XFF,0XEF,0XFC,0XCE,0X75,0X9D,
0X7D,0XDF,0X5C,0XDF,0X3B,0XE7,0X98,0XD6,0X1A,0XEF,0X5A,0XF7,0XD9,0XEE,0X16,0XCE,
0X98,0XDE,0X78,0XD6,0XB5,0XBD,0X6D,0X6B,0X6A,0X4A,0XAB,0X4A,0XB0,0X6B,0XF0,0X73,
0X4A,0X42,0X67,0X21,0X2A,0X3A,0XE9,0X31,0X26,0X19,0X68,0X21,0X67,0X19,0XC5,0X08,
0X66,0X19,0XD8,0XB5,0XFF,0XFF,0X18,0XBE,0X00,0X00,0X00,0X00,0X01,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X21,0X00,0X00,0X00,0X86,0X29,0X1C,0XDF,0XFF,0XFF,0X7D,0XEF,
0X7C,0XEF,0X1A,0XE7,0X3B,0XE7,0X3A,0XEF,0X3A,0XEF,0X19,0XEF,0X77,0XD6,0X56,0XD6,
0X58,0XD6,0X75,0XB5,0X92,0X94,0XF0,0X7B,0XD3,0X94,0X0D,0X5B,0X8B,0X4A,0XA8,0X29,
0X27,0X19,0X88,0X21,0X26,0X19,0X06,0X11,0XE5,0X10,0X26,0X19,0XE9,0X31,0X2A,0X3A,
0XD7,0XB5,0XFF,0XFF,0X59,0XCE,0XC3,0X18,0X00,0X00,0XA2,0X10,0X00,0X00,0X20,0X00,
0X21,0X00,0X00,0X00,0X00,0X00,0X61,0X08,0X20,0X00,0X44,0X29,0X5C,0XEF,0XFF,0XFF,
0X9D,0XF7,0X7C,0XF7,0X3A,0XEF,0X1A,0XE7,0X19,0XEF,0XF9,0XE6,0XB8,0XDE,0X57,0XCE,
0X17,0XC6,0X14,0XA5,0X31,0X84,0X19,0XC6,0XF0,0X7B,0X32,0X84,0XED,0X5A,0XD1,0X73,
0XED,0X52,0X26,0X19,0XA8,0X29,0X26,0X19,0X0A,0X32,0XAC,0X4A,0X6B,0X42,0XBB,0XCE,
0XFF,0XFF,0XFB,0XDE,0X86,0X31,0X00,0X00,0X20,0X00,0X00,0X00,0X61,0X08,0X00,0X00,
0X00,0X00,0X41,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X24,0X21,0XDA,0XDE,
0XDE,0XFF,0X7C,0XF7,0XB9,0XDE,0X1A,0XEF,0X3A,0XEF,0X77,0XD6,0X98,0XDE,0X57,0XD6,
0XCE,0X7B,0X4D,0X6B,0XCF,0X7B,0X8A,0X52,0X4D,0X63,0XC8,0X31,0XAC,0X4A,0XA8,0X29,
0X67,0X21,0XA8,0X29,0XE5,0X10,0X87,0X21,0XA7,0X29,0X8F,0X6B,0XBF,0XEF,0XFF,0XF7,
0X59,0XCE,0XC7,0X39,0X00,0X00,0X00,0X00,0X00,0X00,0X41,0X08,0X00,0X00,0X20,0X00,
0X41,0X08,0X00,0X00,0X00,0X00,0X82,0X10,0X00,0X00,0X20,0X00,0X20,0X08,0X24,0X21,
0XF6,0XBD,0XFE,0XFF,0XFE,0XFF,0XDA,0XE6,0XF6,0XC5,0X16,0XC6,0X98,0XDE,0X54,0XB5,
0X69,0X52,0X71,0X8C,0X08,0X42,0X25,0X21,0X45,0X21,0X46,0X21,0X26,0X21,0XA4,0X08,
0X63,0X00,0X26,0X19,0X46,0X21,0X8F,0X6B,0X9B,0XCE,0XFF,0XFF,0XFF,0XF7,0X55,0XA5,
0X04,0X21,0X00,0X00,0X00,0X00,0X41,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X82,0X10,0X00,0X00,0X00,0X00,0XA2,0X10,0X00,0X00,0X41,0X08,0X00,0X00,
0X00,0X00,0XAE,0X7B,0X79,0XD6,0XFF,0XFF,0XFE,0XFF,0X1B,0XEF,0X37,0XCE,0X30,0X8C,
0XE7,0X39,0XEB,0X5A,0X65,0X29,0XA3,0X10,0XC4,0X10,0X42,0X00,0X63,0X08,0X46,0X21,
0XC8,0X31,0XD0,0X73,0XBB,0XCE,0X7E,0XEF,0XBF,0XF7,0XDB,0XD6,0X0C,0X63,0X00,0X00,
0X00,0X00,0X41,0X08,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X82,0X10,0X00,0X00,
0X21,0X00,0X00,0X00,0X41,0X08,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X40,0X08,
0X61,0X10,0X00,0X00,0X61,0X10,0X30,0X8C,0X5C,0XEF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X5D,0XEF,0X76,0XAD,0XD3,0X9C,0XCF,0X73,0X6E,0X6B,0X93,0X8C,0X76,0XAD,0X3D,0XE7,
0XFF,0XFF,0XBF,0XF7,0X9F,0XEF,0X9E,0XEF,0XCF,0X7B,0X20,0X00,0X20,0X00,0X00,0X00,
0X41,0X08,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X41,0X08,
0X41,0X08,0X00,0X00,0X00,0X00,0X82,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X08,0XA2,0X18,0X49,0X4A,0X14,0XAD,0X7D,0XF7,
0XFF,0XFF,0X3D,0XE7,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9F,0XEF,
0XFC,0XDE,0X96,0XAD,0X8A,0X52,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X41,0X08,
0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X61,0X08,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X41,0X08,0X00,0X00,0X00,0X00,0X61,0X08,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X20,0X08,0X00,0X00,0X00,0X00,0X21,0X08,0X00,0X08,0X21,0X08,
0X28,0X42,0XEF,0X7B,0X8E,0X73,0XF4,0X9C,0XD3,0X94,0X4D,0X63,0X2D,0X63,0X6A,0X4A,
0X21,0X00,0X00,0X00,0X62,0X08,0X00,0X00,0X00,0X00,0X20,0X08,0X00,0X00,0X00,0X00,
0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,
};



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
        SCREEN001, SCREEN002, //SCREEN003, SCREEN004, SCREEN005, /*SCREEN006, SCREEN007, SCREEN008, */
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
        
#if 0 /* 四边线测试 */
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

uint8_t uart_chr = 0;

#if 1 // UART DEBUG 回显
void uart_debug_handler_readbyte_hook(uint8_t chr)
{
    printf("%c", chr);
	uart_chr = chr;
}
void uart_debug_handler_timeout_hook(void)
{
    printf("\r\n");
}
#else // UART modbus
void uart_debug_handler_readbyte_hook(uint8_t chr)
{
    if (!rb.pool) return ;
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
    if (!rb.pool) return 0;
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
        //printf("\n\nAfter memcmp: \n");
        addr += single_size;
        i += single_size;
    }
    printf("\r\n[%s]: exit!\r\n", __FUNCTION__);
}

void test_case_task(){
	printf("\r\n[%s]: entry!\r\n", __FUNCTION__);
	#define QSPI_X       QSPI0
	#define RWLEN        (256)	

	uint8_t RdBuff[RWLEN];
	uint32_t single_size = RWLEN;	
	uint32_t addr = 0;
	qspi_dma_read(addr, RdBuff, single_size, 4, 4);
	
	for (uint32_t i = 0; i < single_size; ++i) {
			printf("[%d]: rd = 0x%02X,  \r\n", i, RdBuff[i]);
	}	
	
	return ;
}
