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

/* 设置Bar的进度值 */
EXPORT_API UG_RESULT UG_BarSetValue(UG_OBJECT* obj, UG_S16 value);

/* 设置Bar的进度最小值 */
EXPORT_API UG_RESULT UG_BarSetValMin(UG_OBJECT* obj, UG_S16 min);

/* 设置Bar的进度最大值 */
EXPORT_API UG_RESULT UG_BarSetValMax(UG_OBJECT* obj, UG_S16 max);

/* 获取Bar当前的进度值 */
EXPORT_API UG_S16 UG_BarGetValue(UG_OBJECT* obj);

/* 获取Bar当前的进度最小值 */
EXPORT_API UG_S16 UG_BarGetValMin(UG_OBJECT* obj);

/* 获取Bar当前的进度最大值 */
EXPORT_API UG_S16 UG_BarGetValMax(UG_OBJECT* obj);

/* 设置进度条背景色 */
EXPORT_API void UG_BarSetBackColor(UG_OBJECT* obj, UG_COLOR color);

/* 设置进度条颜色 */
EXPORT_API void UG_BarSetIndicColor(UG_OBJECT* obj, UG_COLOR color);

/* 设置进度条渐变终色 */
EXPORT_API void UG_BarSetIndicGradColor(UG_OBJECT* obj, UG_COLOR color);

/* 设置进度条边框颜色 */
EXPORT_API void UG_BarSetBorderColor(UG_OBJECT* obj, UG_COLOR color);

/* 设置进度条边框粗细 */
EXPORT_API void UG_BarSetBorderWidth(UG_OBJECT* obj, UG_U8 width);

#ifdef __cplusplus
}
#endif

#endif