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
  LCD_ACOLORINDEX[_GetColorIndex(1)] = Index;
}

/*********************************************************************
*
*       LCD_SetBkColorIndex
*/
void LCD_SetBkColorIndex(int Index) {
  LCD_ACOLORINDEX[_GetColorIndex(0)] = Index;
}

/*********************************************************************
*
*       LCD_SetDrawMode
*/
LCD_DRAWMODE LCD_SetDrawMode(LCD_DRAWMODE dm) {
  LCD_DRAWMODE OldDM = GUI_Context.DrawMode;
  if ((GUI_Context.DrawMode^dm) & LCD_DRAWMODE_REV) {
    LCD_PIXELINDEX temp = LCD_BKCOLORINDEX;
    LCD_BKCOLORINDEX    = LCD_COLORINDEX;
    LCD_COLORINDEX = temp;
  }
  GUI_Context.DrawMode = dm;
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
  /* Perform clipping and check if there is something to do */
  CLIP_X();
  if (x1<x0)
    return;
  CLIP_Y();
  if (y1<y0)
    return;
  /* Call driver to draw */
  LCDDEV_L0_FillRect(x0,y0,x1,y1);
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
  LCDDEV_L0_GetRect(&GUI_Context.ClipRect); 
}

/*********************************************************************
*
*       LCD_Init
*/
int LCD_Init(void) {
  int r = 0;
  GUI_DEBUG_LOG("\nLCD_Init...");
  LCD_SetClipRectMax();
  r |= LCD_L0_Init();
  #if GUI_NUM_LAYERS > 1
    r |= LCD_L0_1_Init();
  #endif
  #if GUI_NUM_LAYERS > 2
    r |= LCD_L0_2_Init();
  #endif
  #if GUI_NUM_LAYERS > 3
    r |= LCD_L0_3_Init();
  #endif
  #if GUI_NUM_LAYERS > 4
    r |= LCD_L0_4_Init();
  #endif
  LCD_InitLUT();
  {
  #if GUI_NUM_LAYERS > 1
    int i;
    for (i = GUI_NUM_LAYERS - 1; i >= 0; i--) {
      GUI_SelectLayer(i);
  #else
    {
  #endif
      #if (GUI_DEFAULT_BKCOLOR != GUI_INVALID_COLOR)
        /* Clear video memory */
        LCD_SetDrawMode(GUI_DRAWMODE_REV);
        LCD_FillRect(0,0, GUI_XMAX, GUI_YMAX);
        LCD_SetDrawMode(0);
      #endif
    }
  }
  /* Switch LCD on */
  LCD_On();
  return r;
}

/*********************************************************************
*
*       LCD_Color2Index
*/
int LCD_Color2Index(LCD_COLOR Color) {
	return Color;
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
  if (GUI_Context.BkColor != color) {
    GUI_Context.BkColor = color;
    LCD_SetBkColorIndex(LCD_Color2Index(color));
  } 
}

/*********************************************************************
*
*       LCD_SetColor
*/
void LCD_SetColor(GUI_COLOR color) {
  if (GUI_Context.Color != color) {
    GUI_Context.Color = color;
    //LCD_SetColorIndex(LCD_Color2Index(color));
  }  
}

/*************************** End of file ****************************/
