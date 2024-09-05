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

  return r;
}

/*************************** End of file ****************************/
