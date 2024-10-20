#ifndef UGUI_LABEL_H
#define UGUI_LABEL_H
#include "../ugui_config.h"

#include "ugui_types.h"
#include "ugui_object.h"
#include "ugui_window.h"
#include "ugui_font.h"
#include "ugui_geometry.h"
#include "ugui_colors.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_API UG_OBJECT* UG_LabelCreate(UG_WINDOW* wnd, UG_ID id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);

/* 设置文本框的字体颜色 */
EXPORT_API UG_RESULT UG_LabelSetForeColor(UG_OBJECT* obj, UG_COLOR fc);

/* 设置文本框区域的背景颜色 */
EXPORT_API UG_RESULT UG_LabelSetBackColor(UG_OBJECT* obj, UG_COLOR bc);

/* 设置文本框区域的背景渐变终色 */
EXPORT_API UG_RESULT UG_LabelSetBackGradColor(UG_OBJECT* obj, UG_COLOR grad);

/* 设置文本框区域的背景色渐变方向：
    GRAD_DIR_NONE   无
    GRAD_DIR_VER    垂直渐变
    GRAD_DIR_HOR    水平渐变
 */
EXPORT_API UG_RESULT UG_LabelSetBackGradDir(UG_OBJECT* obj, UG_U8 dir);

/* 设置文本框区域的背景透明度,0~255,0表示完全透明，255表示完全不透明 */
EXPORT_API UG_RESULT UG_LabelSetBackOpa(UG_OBJECT* obj, UG_U8 opa);

/* 设置文本内容，该接口会将str内容复制一份，调用完成后，str可由用户自行复用或释放 */
EXPORT_API UG_RESULT UG_LabelSetText(UG_OBJECT* obj, char* str);

/* 设置文本内容，该接口不会复制static_str内容，而是直接引用该地址 */
EXPORT_API UG_RESULT UG_LabelSetTextStatic(UG_OBJECT* obj, char* static_str);

/* 设置文本框字体 */
EXPORT_API UG_RESULT UG_LabelSetFont(UG_OBJECT* obj, const UG_FONT* font);

/* 设置文本字间距 */
EXPORT_API UG_RESULT UG_LabelSetHSpace(UG_OBJECT* obj, UG_S8 hs);

/* 设置文本行间距 */
EXPORT_API UG_RESULT UG_LabelSetVSpace(UG_OBJECT* obj, UG_S8 vs);

/* 设置文本行对齐方式，参考ugui.h内的Alignments部分 */
EXPORT_API UG_RESULT UG_LabelSetAlignment(UG_OBJECT* obj, UG_U8 align);

/* 设置文本长模式，参考ugui.h内的Text long modes部分 */
EXPORT_API UG_RESULT UG_LabelSetLongMode(UG_OBJECT* obj, UG_U8 long_mode);

/* 获取当前使用的文字颜色 */
EXPORT_API UG_COLOR UG_LabelGetForeColor(UG_OBJECT* obj);

/* 获取当前使用的背景颜色 */
EXPORT_API UG_COLOR UG_LabelGetBackColor(UG_OBJECT* obj);

/* 获取当前使用的背景透明度 */
EXPORT_API UG_U8 UG_LabelGetBackOpa(UG_OBJECT* obj);

/* 获取当前文本内容 */
EXPORT_API char* UG_LabelGetText(UG_OBJECT* obj);

/* 获取当前使用的字体指针 */
EXPORT_API UG_FONT* UG_LabelGetFont(UG_OBJECT* obj);

/* 获取字间距 */
EXPORT_API UG_S8 UG_LabelGetHSpace(UG_OBJECT* obj);

/* 获取行间距 */
EXPORT_API UG_S8 UG_LabelGetVSpace(UG_OBJECT* obj);

/* 获取当前使用的文本对齐颜色 */
EXPORT_API UG_U8 UG_LabelGetAlignment(UG_OBJECT* obj);

#ifdef __cplusplus
}
#endif
#endif