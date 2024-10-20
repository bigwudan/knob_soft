#ifndef UGUI_BAR_H
#define UGUI_BAR_H
#include "../ugui_config.h"
#include "ugui_types.h"
#include "ugui_object.h"
#include "ugui_window.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Bar functions */
EXPORT_API UG_OBJECT* UG_BarCreate(UG_WINDOW* wnd, UG_ID id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);

/* ����Bar�Ľ���ֵ */
EXPORT_API UG_RESULT UG_BarSetValue(UG_OBJECT* obj, UG_S16 value);

/* ����Bar�Ľ�����Сֵ */
EXPORT_API UG_RESULT UG_BarSetValMin(UG_OBJECT* obj, UG_S16 min);

/* ����Bar�Ľ������ֵ */
EXPORT_API UG_RESULT UG_BarSetValMax(UG_OBJECT* obj, UG_S16 max);

/* ��ȡBar��ǰ�Ľ���ֵ */
EXPORT_API UG_S16 UG_BarGetValue(UG_OBJECT* obj);

/* ��ȡBar��ǰ�Ľ�����Сֵ */
EXPORT_API UG_S16 UG_BarGetValMin(UG_OBJECT* obj);

/* ��ȡBar��ǰ�Ľ������ֵ */
EXPORT_API UG_S16 UG_BarGetValMax(UG_OBJECT* obj);

/* ���ý���������ɫ */
EXPORT_API void UG_BarSetBackColor(UG_OBJECT* obj, UG_COLOR color);

/* ���ý�������ɫ */
EXPORT_API void UG_BarSetIndicColor(UG_OBJECT* obj, UG_COLOR color);

/* ���ý�����������ɫ */
EXPORT_API void UG_BarSetIndicGradColor(UG_OBJECT* obj, UG_COLOR color);

/* ���ý������߿���ɫ */
EXPORT_API void UG_BarSetBorderColor(UG_OBJECT* obj, UG_COLOR color);

/* ���ý������߿��ϸ */
EXPORT_API void UG_BarSetBorderWidth(UG_OBJECT* obj, UG_U8 width);

#ifdef __cplusplus
}
#endif

#endif