#ifndef UGUI_LEVELIMG_H
#define UGUI_LEVELIMG_H
#include "../ugui_config.h"
#include "ugui_types.h"
#include "ugui_object.h"
#include "ugui_window.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_API UG_OBJECT* UG_LevelImgCreate(UG_WINDOW* wnd, UG_ID id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);

/* 设置分级图像级别 */
EXPORT_API UG_RESULT UG_LevelImgSetLevel(UG_OBJECT* obj, UG_U16 level);

/* 获取分级图像的级别总数 */
EXPORT_API UG_U16 UG_LevelImgGetLevelNum(UG_OBJECT* obj);

/* 获取分级图像当前级别 */
EXPORT_API UG_U16 UG_LevelImgGetLevel(UG_OBJECT* obj);

/* 停止自动播放 */
EXPORT_API void UG_LevelImgStopAutoAnim(UG_OBJECT* obj);

/* 开始自动播放 */
EXPORT_API void UG_LevelImgStartAutoAnim(UG_OBJECT* obj);

/* 设置分级图像自动播放结束后的回调函数cb。
 (只有当UI Creator将分级图像的重复次数设置不为0时，cb才有机会被回调  
 */
EXPORT_API void UG_LevelImgSetAnimReadyCb(UG_OBJECT* obj, void (*cb)(UG_OBJECT* obj, void* data), void* data);

/* 设置重着色(仅对灰度图片有效) */
EXPORT_API UG_RESULT UG_LevelImgSetRecolor(UG_OBJECT* obj, UG_COLOR recolor);

#ifdef __cplusplus
}
#endif
#endif