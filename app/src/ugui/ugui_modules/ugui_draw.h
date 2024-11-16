#ifndef UGUI_DRAW_H
#define UGUI_DRAW_H
#include "../ugui_config.h"
#include "ugui_types.h"
#include "ugui_colors.h"
#include "ugui_bmp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Drawing functions */
EXPORT_API void UG_FillScreen(UG_COLOR c);
EXPORT_API void UG_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
EXPORT_API void UG_FillRoundFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c);
EXPORT_API void UG_DrawMesh(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
EXPORT_API void UG_DrawFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
EXPORT_API void UG_DrawRoundFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c);
EXPORT_API void UG_DrawPixel(UG_S16 x0, UG_S16 y0, UG_COLOR c);
EXPORT_API void UG_DrawCircle(UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c);
EXPORT_API void UG_FillCircle(UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c);
EXPORT_API void UG_DrawArc(UG_S16 x0, UG_S16 y0, UG_S16 r, UG_U8 s, UG_COLOR c);
EXPORT_API void UG_DrawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
EXPORT_API void UG_PutString(UG_S16 x, UG_S16 y, const char* str, UG_COLOR c);
EXPORT_API void UG_MeasureString(const char* str, UG_S16* w, UG_S16* h);
EXPORT_API void UG_DrawBMP(UG_S16 xp, UG_S16 yp, UG_BMP* bmp);

#ifdef __cplusplus
}
#endif

#endif