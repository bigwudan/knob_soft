/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCD.c
Purpose     : Link between GUI and LCD_L0
              Performs most of the clipping.
---------------------------END-OF-HEADER------------------------------
*/

#define LCD_C

#include <stdio.h>
#include "GUI_Private.h"
#include "LCD_Private.h"
#include "GUIDebug.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define RETURN_IF_Y_OUT() \
  if (y < GUI_Context.ClipRect.y0) return;             \
  if (y > GUI_Context.ClipRect.y1) return;

#define RETURN_IF_X_OUT() \
  if (x < GUI_Context.ClipRect.x0) return;             \
  if (x > GUI_Context.ClipRect.x1) return;

#define CLIP_X() \
  if (x0 < GUI_Context.ClipRect.x0) { x0 = GUI_Context.ClipRect.x0; } \
  if (x1 > GUI_Context.ClipRect.x1) { x1 = GUI_Context.ClipRect.x1; }

#define CLIP_Y() \
  if (y0 < GUI_Context.ClipRect.y0) { y0 = GUI_Context.ClipRect.y0; } \
  if (y1 > GUI_Context.ClipRect.y1) { y1 = GUI_Context.ClipRect.y1; }

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetColorIndex
*/
static int _GetColorIndex(int i)  /* i is 0 or 1 */ {

}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_SetColorIndex
*/
void LCD_SetColorIndex(int Index) {

}

/*********************************************************************
*
*       LCD_SetBkColorIndex
*/
void LCD_SetBkColorIndex(int Index) {

}

/*********************************************************************
*
*       LCD_SetDrawMode
*/
LCD_DRAWMODE LCD_SetDrawMode(LCD_DRAWMODE dm) {
  LCD_DRAWMODE OldDM = GUI_Context.DrawMode;

  return OldDM;
}

/*********************************************************************
*
*       LCD_DrawPixel
*/
void LCD_DrawPixel(int x, int y) {
 
}

/*********************************************************************
*
*       LCD_DrawHLine
*/
void LCD_DrawHLine(int x0, int y,  int x1) {
	LCDDEV_L0_DrawHLine(x0, y, x1);
}

/*********************************************************************
*
*       LCD_FillRect
*/
void LCD_FillRect(int x0, int y0, int x1, int y1) {

}

/*********************************************************************
*
*       LCD_DrawBitmap
*/
void LCD_DrawBitmap(int x0, int y0, int xsize, int ysize, int xMul, int yMul,
                       int BitsPerPixel, int BytesPerLine,
                       const U8 GUI_UNI_PTR * pPixel, const LCD_PIXELINDEX* pTrans)
{


}

/*********************************************************************
*
*       LCD_SetClipRectMax
*/
void LCD_SetClipRectMax(void) {
 
}

/*********************************************************************
*
*       LCD_Init
*/
int LCD_Init(void) {
  int r = 0;

  return r;
}

/*********************************************************************
*
*       LCD_Color2Index
*/
int LCD_Color2Index(LCD_COLOR Color) {

}

/*********************************************************************
*
*       LCD_Index2Color
*/
LCD_COLOR LCD_Index2Color(int Index) {
 
}

/*********************************************************************
*
*       LCD_SetBkColor
*/
void LCD_SetBkColor(GUI_COLOR color) {
 
}

/*********************************************************************
*
*       LCD_SetColor
*/
void LCD_SetColor(GUI_COLOR color) {
  
}

/*************************** End of file ****************************/
