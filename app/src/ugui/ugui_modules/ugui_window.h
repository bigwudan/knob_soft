#ifndef UGUI_WINDOW_H
#define UGUI_WINDOW_H
#include "../ugui_config.h"
#include "ugui_object.h"
#include "ugui_colors.h"
#include "ugui_geometry.h"
#include "ugui_message.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct S_WINDOW
{
    UG_U8 objmax;
    UG_U8 objcnt;
    struct S_OBJECT** objlst;
    struct S_OBJECT* focused_obj;

    UG_AREA inv[10];
    UG_U8 invcnt;
    UG_U8 invmerged;
    UG_U8 state;
    UG_COLOR fc;
    UG_COLOR bc;
    UG_AREA area;
    UG_U8 style;
    void (*cb)(UG_MESSAGE*);
}UG_WINDOW;

    /* Window states */
#define WND_STATE_FREE                                (1<<0)
#define WND_STATE_VALID                               (1<<1)
#define WND_STATE_BUSY                                (1<<2)
#define WND_STATE_VISIBLE                             (1<<3)
#define WND_STATE_ENABLE                              (1<<4)
#define WND_STATE_UPDATE                              (1<<5)
#define WND_STATE_REDRAW_TITLE                        (1<<6)

/* Window styles */
#define WND_STYLE_2D                                  (0<<0)
#define WND_STYLE_3D                                  (1<<0)

/* Window functions */
EXPORT_API UG_WINDOW* UG_WindowCreate(UG_U8 objcnt, void (*cb)(UG_MESSAGE*));
EXPORT_API UG_RESULT UG_WindowDelete(UG_WINDOW* wnd);
EXPORT_API UG_RESULT UG_WindowShow(UG_WINDOW* wnd);
EXPORT_API UG_RESULT UG_WindowHide(UG_WINDOW* wnd);
EXPORT_API UG_RESULT UG_WindowResize(UG_WINDOW* wnd, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);
EXPORT_API UG_RESULT UG_WindowSetForeColor(UG_WINDOW* wnd, UG_COLOR fc);
EXPORT_API UG_RESULT UG_WindowSetBackColor(UG_WINDOW* wnd, UG_COLOR bc);
EXPORT_API UG_RESULT UG_WindowSetXStart(UG_WINDOW* wnd, UG_S16 xs);
EXPORT_API UG_RESULT UG_WindowSetYStart(UG_WINDOW* wnd, UG_S16 ys);
EXPORT_API UG_RESULT UG_WindowSetXEnd(UG_WINDOW* wnd, UG_S16 xe);
EXPORT_API UG_RESULT UG_WindowSetYEnd(UG_WINDOW* wnd, UG_S16 ye);
EXPORT_API UG_RESULT UG_WindowSetStyle(UG_WINDOW* wnd, UG_U8 style);
EXPORT_API UG_COLOR UG_WindowGetForeColor(UG_WINDOW* wnd);
EXPORT_API UG_COLOR UG_WindowGetBackColor(UG_WINDOW* wnd);
EXPORT_API UG_S16 UG_WindowGetXStart(UG_WINDOW* wnd);
EXPORT_API UG_S16 UG_WindowGetYStart(UG_WINDOW* wnd);
EXPORT_API UG_S16 UG_WindowGetXEnd(UG_WINDOW* wnd);
EXPORT_API UG_S16 UG_WindowGetYEnd(UG_WINDOW* wnd);
EXPORT_API UG_U8 UG_WindowGetStyle(UG_WINDOW* wnd);
EXPORT_API UG_RESULT UG_WindowGetArea(UG_WINDOW* wnd, UG_AREA* a);

#ifdef __cplusplus
}
#endif
#endif