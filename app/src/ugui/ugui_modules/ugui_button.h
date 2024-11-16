#ifndef UGUI_BUTTON_H
#define UGUI_BUTTON_H
#include "../ugui_config.h"
#include "ugui_types.h"
#include "ugui_object.h"
#include "ugui_window.h"
#include "ugui_font.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* Button states */
#define BTN_STATE_RELEASED                            (0<<0)
#define BTN_STATE_PRESSED                             (1<<0)
#define BTN_STATE_ALWAYS_REDRAW                       (1<<1)

/* Button events */
#define BTN_EVENT_CLICKED                             OBJ_EVENT_CLICKED

/* Button style */
#define BTN_STYLE_2D                                  (0<<0)
#define BTN_STYLE_3D                                  (1<<0)
#define BTN_STYLE_TOGGLE_COLORS                       (1<<1)
#define BTN_STYLE_USE_ALTERNATE_COLORS                (1<<2)
#define BTN_STYLE_NO_BORDERS                          (1<<3)
#define BTN_STYLE_NO_FILL                             (1<<4)

/* Button functions */
EXPORT_API UG_OBJECT* UG_ButtonCreate(UG_WINDOW* wnd, UG_ID id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);
EXPORT_API UG_RESULT UG_ButtonSetForeColor(UG_OBJECT* obj, UG_COLOR fc);
EXPORT_API UG_RESULT UG_ButtonSetBackColor(UG_OBJECT* obj, UG_COLOR bc);
EXPORT_API UG_RESULT UG_ButtonSetAlternateForeColor(UG_OBJECT* obj, UG_COLOR afc);
EXPORT_API UG_RESULT UG_ButtonSetAlternateBackColor(UG_OBJECT* obj, UG_COLOR abc);
EXPORT_API UG_RESULT UG_ButtonSetText(UG_OBJECT* obj, char* str);
EXPORT_API UG_RESULT UG_ButtonSetFont(UG_OBJECT* obj, const UG_FONT* font);
EXPORT_API UG_RESULT UG_ButtonSetStyle(UG_OBJECT* obj, UG_U8 style);
EXPORT_API UG_RESULT UG_ButtonSetHSpace(UG_OBJECT* obj, UG_S8 hs);
EXPORT_API UG_RESULT UG_ButtonSetVSpace(UG_OBJECT* obj, UG_S8 vs);
EXPORT_API UG_RESULT UG_ButtonSetAlignment(UG_OBJECT* obj, UG_U8 align);
EXPORT_API UG_COLOR UG_ButtonGetForeColor(UG_OBJECT* obj);
EXPORT_API UG_COLOR UG_ButtonGetBackColor(UG_OBJECT* obj);
EXPORT_API UG_COLOR UG_ButtonGetAlternateForeColor(UG_OBJECT* obj);
EXPORT_API UG_COLOR UG_ButtonGetAlternateBackColor(UG_OBJECT* obj);
EXPORT_API char* UG_ButtonGetText(UG_OBJECT* obj);
EXPORT_API UG_FONT* UG_ButtonGetFont(UG_OBJECT* obj);
EXPORT_API UG_U8 UG_ButtonGetStyle(UG_OBJECT* obj);
EXPORT_API UG_S8 UG_ButtonGetHSpace(UG_OBJECT* obj);
EXPORT_API UG_S8 UG_ButtonGetVSpace(UG_OBJECT* obj);
EXPORT_API UG_U8 UG_ButtonGetAlignment(UG_OBJECT* obj);

#ifdef __cplusplus
}
#endif
#endif