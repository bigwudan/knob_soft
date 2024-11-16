#ifndef UGUI_IMAGE_H
#define UGUI_IMAGE_H
#include "../ugui_config.h"
#include "ugui_types.h"
#include "ugui_object.h"
#include "ugui_window.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Image types */
#define IMG_TYPE_BMP                                  (1<<0)

/* Image functions */
EXPORT_API UG_OBJECT* UG_ImageCreate(UG_WINDOW* wnd, UG_ID id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);

/* ����ͼƬԴ */
EXPORT_API UG_RESULT UG_ImageSetBMP(UG_OBJECT* obj, const UG_BMP* bmp);

/* ��������ɫ(���ԻҶ�ͼƬ��Ч) */
EXPORT_API UG_RESULT UG_ImageSetRecolor(UG_OBJECT* obj, UG_COLOR recolor);


#ifdef __cplusplus
}
#endif
#endif