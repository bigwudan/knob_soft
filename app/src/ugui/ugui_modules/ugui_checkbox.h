#ifndef UGUI_CHECKBOX_H
#define UGUI_CHECKBOX_H
#include "../ugui_config.h"
#include "ugui_types.h"
#include "ugui_object.h"
#include "ugui_window.h"
#include "ugui_font.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* Checkbox states */
#define CHB_STATE_RELEASED                            (0<<0)
#define CHB_STATE_PRESSED                             (1<<0)
#define CHB_STATE_ALWAYS_REDRAW                       (1<<1)

/* Checkbox style */
#define CHB_STYLE_2D                                  (0<<0)
#define CHB_STYLE_3D                                  (1<<0)
#define CHB_STYLE_TOGGLE_COLORS                       (1<<1)
#define CHB_STYLE_USE_ALTERNATE_COLORS                (1<<2)
#define CHB_STYLE_NO_BORDERS                          (1<<3)
#define CHB_STYLE_NO_FILL                             (1<<4)

/* Checkbox events */
#define CHB_EVENT_CLICKED                             OBJ_EVENT_CLICKED


/* Checkbox functions */
EXPORT_API UG_OBJECT* UG_CheckboxCreate(UG_WINDOW* wnd, UG_ID id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);
EXPORT_API UG_RESULT UG_CheckboxSetCheched(UG_OBJECT* obj, UG_U8 ch);
EXPORT_API UG_RESULT UG_CheckboxSetForeColor(UG_OBJECT* obj, UG_COLOR fc);
EXPORT_API UG_RESULT UG_CheckboxSetBackColor(UG_OBJECT* obj, UG_COLOR bc);
EXPORT_API UG_RESULT UG_CheckboxSetAlternateForeColor(UG_OBJECT* obj, UG_COLOR afc);
EXPORT_API UG_RESULT UG_CheckboxSetAlternateBackColor(UG_OBJECT* obj, UG_COLOR abc);
EXPORT_API UG_RESULT UG_CheckboxSetText(UG_OBJECT* obj, char* str);
EXPORT_API UG_RESULT UG_CheckboxSetFont(UG_OBJECT* obj, const UG_FONT* font);
EXPORT_API UG_RESULT UG_CheckboxSetStyle(UG_OBJECT* obj, UG_U8 style);
EXPORT_API UG_RESULT UG_CheckboxSetHSpace(UG_OBJECT* obj, UG_S8 hs);
EXPORT_API UG_RESULT UG_CheckboxSetVSpace(UG_OBJECT* obj, UG_S8 vs);
EXPORT_API UG_RESULT UG_CheckboxSetAlignment(UG_OBJECT* obj, UG_U8 align);
EXPORT_API UG_U8 UG_CheckboxGetChecked(UG_OBJECT* obj);
EXPORT_API UG_COLOR UG_CheckboxGetForeColor(UG_OBJECT* obj);
EXPORT_API UG_COLOR UG_CheckboxGetBackColor(UG_OBJECT* obj);
EXPORT_API UG_COLOR UG_CheckboxGetAlternateForeColor(UG_OBJECT* obj);
EXPORT_API UG_COLOR UG_CheckboxGetAlternateBackColor(UG_OBJECT* obj);
EXPORT_API char* UG_CheckboxGetText(UG_OBJECT* obj);
EXPORT_API UG_FONT* UG_CheckboxGetFont(UG_OBJECT* obj);
EXPORT_API UG_U8 UG_CheckboxGetStyle(UG_OBJECT* obj);
EXPORT_API UG_S8 UG_CheckboxGetHSpace(UG_OBJECT* obj);
EXPORT_API UG_S8 UG_CheckboxGetVSpace(UG_OBJECT* obj);
EXPORT_API UG_U8 UG_CheckboxGetAlignment(UG_OBJECT* obj);


#ifdef __cplusplus
}
#endif
#endif