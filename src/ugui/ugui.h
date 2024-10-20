/* -------------------------------------------------------------------------------- */
/* -- µGUI - Generic GUI module (C)Achim Döbler, 2015                            -- */
/* -------------------------------------------------------------------------------- */
// µGUI is a generic GUI module for embedded systems.
// This is a free software that is open for education, research and commercial
// developments under license policy of following terms.
//
//  Copyright (C) 2015, Achim Döbler, all rights reserved.
//  URL: http://www.embeddedlightning.com/
//
// * The µGUI module is a free software and there is NO WARRANTY.
// * No restriction on use. You can use, modify and redistribute it for
//   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
// * Redistributions of source code must retain the above copyright notice.
//
/* -------------------------------------------------------------------------------- */
#ifndef __UGUI_H
#define __UGUI_H

#include "ugui_config.h"
#include "ug_mem.h"

#include "ugui_modules/ugui_types.h"
#include "ugui_modules/ugui_geometry.h"
#include "ugui_modules/ugui_colors.h"
#include "ugui_modules/ugui_font.h"
#include "ugui_modules/ugui_bmp.h"
#include "ugui_modules/ugui_message.h"
#include "ugui_modules/ugui_draw.h"

#include "ugui_modules/ugui_object.h"
#include "ugui_modules/ugui_window.h"
#include "ugui_modules/ugui_button.h"
#include "ugui_modules/ugui_checkbox.h"
#include "ugui_modules/ugui_timer.h"
#include "ugui_modules/ugui_image.h"
#include "ugui_modules/ugui_levelimg.h"
#include "ugui_modules/ugui_bar.h"
#include "ugui_modules/ugui_arc.h"
#include "ugui_modules/ugui_label.h"

#ifdef __cplusplus
extern "C" {
#endif
/* -------------------------------------------------------------------------------- */
/* -- µGUI FONTS                                                                 -- */
/* -------------------------------------------------------------------------------- */
EXPORT_VAR extern __UG_FONT_DATA UG_FONT UG_FONT_MONTSERRAT_10;


/* -------------------------------------------------------------------------------- */
/* -- TYPEDEFS                                                                   -- */
/* -------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------- */
/* -- DEFINES                                                                    -- */
/* -------------------------------------------------------------------------------- */
#ifndef NULL
   #define NULL ((void*) 0)
#endif

#define UG_MIN(a, b)    ((a) < (b) ? (a) : (b))
#define UG_MAX(a, b)    ((a) < (b) ? (b) : (a))
#define UG_DIM(x)   (sizeof(x) / sizeof((x)[0]))

/* Text alignments */
#define ALIGN_H_LEFT                                  (1<<0)
#define ALIGN_H_CENTER                                (1<<1)
#define ALIGN_H_RIGHT                                 (1<<2)
#define ALIGN_V_TOP                                   (1<<3)
#define ALIGN_V_CENTER                                (1<<4)
#define ALIGN_V_BOTTOM                                (1<<5)
#define ALIGN_BOTTOM_RIGHT                            (ALIGN_V_BOTTOM|ALIGN_H_RIGHT)
#define ALIGN_BOTTOM_CENTER                           (ALIGN_V_BOTTOM|ALIGN_H_CENTER)
#define ALIGN_BOTTOM_LEFT                             (ALIGN_V_BOTTOM|ALIGN_H_LEFT)
#define ALIGN_CENTER_RIGHT                            (ALIGN_V_CENTER|ALIGN_H_RIGHT)
#define ALIGN_CENTER                                  (ALIGN_V_CENTER|ALIGN_H_CENTER)
#define ALIGN_CENTER_LEFT                             (ALIGN_V_CENTER|ALIGN_H_LEFT)
#define ALIGN_TOP_RIGHT                               (ALIGN_V_TOP|ALIGN_H_RIGHT)
#define ALIGN_TOP_CENTER                              (ALIGN_V_TOP|ALIGN_H_CENTER)
#define ALIGN_TOP_LEFT                                (ALIGN_V_TOP|ALIGN_H_LEFT)

/* Text long modes */
#define TEXT_LONG_MODE_NONE                                (0)
#define TEXT_LONG_MODE_SCROLL_CIRCLE                       (1) /* Roll the text circularly */
#define TEXT_LONG_MODE_SCROLL                              (2) /* Roll the text back and forth */

/* Gradient directions */
#define GRAD_DIR_NONE   0
#define GRAD_DIR_VER    1
#define GRAD_DIR_HOR    2

/* -------------------------------------------------------------------------------- */
/* -- FUNCTION RESULTS                                                           -- */
/* -------------------------------------------------------------------------------- */
#define UG_RESULT_FAIL                               -1
#define UG_RESULT_OK                                  0

/* -------------------------------------------------------------------------------- */
/* -- TOUCH                                                                      -- */
/* -------------------------------------------------------------------------------- */
/* Touch structure */
typedef struct
{
   UG_U8 state;
   UG_S16 xp;
   UG_S16 yp;
} UG_TOUCH;


#define TOUCH_STATE_PRESSED                           1
#define TOUCH_STATE_RELEASED                          0

/* -------------------------------------------------------------------------------- */
/* -- OBJECTS                                                                    -- */
/* -------------------------------------------------------------------------------- */

/* Currently supported objects */
#define OBJ_TYPE_NONE                                 0
#define OBJ_TYPE_OBJECT                               OBJ_TYPE_NONE
#define OBJ_TYPE_BUTTON                               1
#define OBJ_TYPE_LABEL                                2
#define OBJ_TYPE_IMAGE                                3
#define OBJ_TYPE_CHECKBOX                             4
#define OBJ_TYPE_BAR                                  5
#define OBJ_TYPE_LEVELIMG                             6
#define OBJ_TYPE_ARC                                  7

/* Standard object events */
#define OBJ_EVENT_NONE                                0
#define OBJ_EVENT_CLICKED                             1
#ifdef USE_PRERENDER_EVENT
#define OBJ_EVENT_PRERENDER                           2
#endif
#ifdef USE_POSTRENDER_EVENT
#define OBJ_EVENT_POSTRENDER                          3
#endif
#define OBJ_EVENT_PRESSED                             4
#define OBJ_EVENT_RELEASED                            5

/* Object touch states */
#define OBJ_TOUCH_STATE_CHANGED                       (1<<0)
#define OBJ_TOUCH_STATE_PRESSED_ON_OBJECT             (1<<1)
#define OBJ_TOUCH_STATE_PRESSED_OUTSIDE_OBJECT        (1<<2)
#define OBJ_TOUCH_STATE_RELEASED_ON_OBJECT            (1<<3)
#define OBJ_TOUCH_STATE_RELEASED_OUTSIDE_OBJECT       (1<<4)
#define OBJ_TOUCH_STATE_IS_PRESSED_ON_OBJECT          (1<<5)
#define OBJ_TOUCH_STATE_IS_PRESSED                    (1<<6)
#define OBJ_TOUCH_STATE_CLICK_ON_OBJECT               (1<<7)
#define OBJ_TOUCH_STATE_INIT                          0

/* -------------------------------------------------------------------------------- */
/* -- µGUI DRIVER                                                                -- */
/* -------------------------------------------------------------------------------- */
typedef struct
{
  void* driver;
  UG_U8 state;
} UG_DRIVER;

#define DRIVER_REGISTERED                             (1<<0)
#define DRIVER_ENABLED                                (1<<1)

/* Supported drivers */
#define NUMBER_OF_DRIVERS                             3
#define DRIVER_DRAW_LINE                              0
#define DRIVER_FILL_FRAME                             1
#define DRIVER_FILL_AREA                              2

/* -------------------------------------------------------------------------------- */
/* -- µGUI CORE STRUCTURE                                                        -- */
/* -------------------------------------------------------------------------------- */

typedef struct {
    void (*area_set)(UG_S16 x, UG_S16 y, UG_S16 w, UG_S16 h);
    void (*flush_pixels)(UG_COLOR* colors, UG_U16 num, UG_U8 asyncable);
    void (*flush_ready)(UG_S16 x, UG_S16 y, UG_S16 w, UG_S16 h);
}DISPLAY_OPS;

typedef struct {
    int (*ui_data_read)(UG_U32 offset, UG_U32 size, void* buf);
}DATA_OPS;

typedef struct {
    void (*framework_ready)(void);
    void (*app_ready)(void);

    void (*main_tick)(void);
    /* 指定main_tick回调函数的间隔，毫秒为单位，默认为50ms */
    UG_U32 main_tick_period;

    /* 钩子函数，进入蓝屏界面前会被调用 */
    void (*pre_blue)(const char* msg);
}APP_CALLBACK;

typedef struct {
    DISPLAY_OPS disp;
    DATA_OPS data;
    APP_CALLBACK app;
}SYS_OPS;

typedef struct {
    UG_S16 display_width;
    UG_S16 display_height;

    MEM_UNIT* heap;
    UG_U32 heap_len;

    UG_U16 pixels_per_pfb;
    UG_U8 num_of_pfb;

    UG_U8 flush_area_alignment;
}SYS_CONFIG;
/* Flush area align */
#define FAA_EVEN_X          0x01
#define FAA_EVEN_Y          0x02
#define FAA_EVEN_W          0x04
#define FAA_EVEN_H          0x08
#define FAA_EVEN_PIXELS     0x10

typedef struct
{
   const SYS_OPS *sys_ops;
   void* render;
   UG_U8 asyncable_pfb;
   UG_S16 x_dim;
   UG_S16 y_dim;
   UG_TOUCH touch;
   UG_WINDOW* next_window;
   UG_WINDOW* active_window;
   UG_WINDOW* last_window;

   UG_FONT font;
   UG_S8 char_h_space;
   UG_S8 char_v_space;
   UG_COLOR desktop_color;
   UG_U8 state;
   UG_DRIVER driver[NUMBER_OF_DRIVERS];
} UG_GUI;

#define UG_SATUS_WAIT_FOR_UPDATE                      (1<<0)

#define UG_MALLOC   ug_mem_alloc
#define UG_FREE     ug_mem_free
/* -------------------------------------------------------------------------------- */
/* -- PROTOTYPES                                                                 -- */
/* -------------------------------------------------------------------------------- */
EXPORT_API UG_OBJECT* UG_FindObject(UG_WINDOW* wnd, UG_ID id);

/* Classic functions */
EXPORT_API UG_S16 UG_Init( UG_GUI* g, const SYS_OPS *ops, const SYS_CONFIG* conf);
EXPORT_API void UG_Deinit(UG_GUI* g);
EXPORT_API UG_S16 UG_SelectGUI( UG_GUI* g );
EXPORT_API UG_GUI *UG_GetGUI();

EXPORT_API UG_S16 UG_GetXDim( void );
EXPORT_API UG_S16 UG_GetYDim( void );

/* Font functions */
EXPORT_API void UG_FontSelect(const UG_FONT* font);
EXPORT_API void UG_FontSetHSpace( UG_U16 s );
EXPORT_API void UG_FontSetVSpace( UG_U16 s );
EXPORT_API UG_S8 UG_FontGetHSpace();
EXPORT_API UG_S8 UG_FontGetVSpace();

/* Miscellaneous functions */
EXPORT_API void UG_WaitForUpdate( void );
EXPORT_API void UG_Update( void );
EXPORT_API void UG_TickInc(UG_U32 tick_period);
EXPORT_API UG_U32 UG_TickGet(void);
EXPORT_API void UG_TouchUpdate( UG_S16 xp, UG_S16 yp, UG_U8 state );

/* Driver functions */
EXPORT_API void UG_DriverRegister( UG_U8 type, void* driver );
EXPORT_API void UG_DriverEnable( UG_U8 type );
EXPORT_API void UG_DriverDisable( UG_U8 type );


#ifdef __cplusplus
}
#endif
#endif
