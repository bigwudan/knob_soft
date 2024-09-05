/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUICore.C
Purpose     : Core routines
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>           /* needed for definition of NULL */

#define  GL_CORE_C

#include "GUI_Private.h"
#include "GUIDebug.h"





/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitContext
*
* Purpose
*   Initializes the given context.
*
* Special considerations
*   Make sure that no GUI functions are called, because locking is
*   not allowed here (GUITASK_INIT not yet called).
*/
static void _InitContext(GUI_CONTEXT* pContext) {
  /* memset(..,0,..) is not required, as this function is called only at startup of the GUI when data is 0 */
  #if GUI_SUPPORT_DEVICES
    pContext->pDeviceAPI   = LCD_aAPI[0]; /* &LCD_L0_APIList; */
  #endif
  pContext->pClipRect_HL = &GUI_Context.ClipRect;
  LCD_L0_GetRect(&pContext->ClipRect);
  #if GUI_SUPPORT_AA
    pContext->pLCD_HL      = &_HL_APIList;
  #endif
  pContext->pAFont       = NULL;
  pContext->pClipRect_HL = &GUI_Context.ClipRect;
  pContext->PenSize      = 1;
  /* Variables in WM module */
  #if GUI_WINSUPPORT
    pContext->hAWin    = WM_GetDesktopWindow();
  #endif
  /* Variables in GUI_AA module */
  #if GUI_SUPPORT_AA
    pContext->AA_Factor = 3;
  #endif
  pContext->Color   = GUI_INVALID_COLOR;
  pContext->BkColor = GUI_INVALID_COLOR;
  LCD_SetBkColor(GUI_DEFAULT_BKCOLOR);
  LCD_SetColor(GUI_DEFAULT_COLOR);
  pContext->pUC_API = &GUI__API_TableNone; 
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_ClearRect
*/
void GUI_ClearRect(int x0, int y0, int x1, int y1) {
 
}

/*********************************************************************
*
*       GUI_Clear
*/
void GUI_Clear(void) {

}

/*********************************************************************
*
*       GUI_Init
*
* Purpose:
*   Init of GUI internal data structures & variables
*/
int GUI_Init(void) {
  int r;
  GUI_DEBUG_LOG("\nGUI_Init()");
  /* Init system wide globals first */
  GUI_DecChar = '.';
  /* Init context */
  _InitContext(&GUI_Context);
  GUITASK_INIT();
  r = LCD_Init();
  #if GUI_WINSUPPORT
    WM_Init();
  #endif
  GUITASK_COPY_CONTEXT();
  #if defined(GUI_TRIAL_VERSION)
  {
    int i;
    for (i = 0; i < 10; i++) {
      GUI_DispString("This uC-GUI library\n"
	                     "is for evaluation\n"
	                     "purpose only.\n"
	                     "A license is\n"
	                     "required to use\n"
	                     "it in a product\n\n"
	                     "www.micrium.com\n");
      GUI_GotoXY(0, 0);
    }
  }
  GUI_Clear();
  #endif
}

/*************************** End of file ****************************/