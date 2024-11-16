#ifndef UGUI_ARC_H
#define UGUI_ARC_H
#include "../ugui_config.h"
#include "ugui_types.h"
#include "ugui_object.h"
#include "ugui_window.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Arc functions*/
EXPORT_API UG_OBJECT* UG_ArcCreate(UG_WINDOW* wnd, UG_ID id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);

/* 设置圆弧半径 */
EXPORT_API void UG_ArcSetRadius(UG_OBJECT* obj, UG_U16 r);

/* 设置圆弧粗细 */
EXPORT_API void UG_ArcSetStrokeWidth(UG_OBJECT* obj, UG_U16 width);

/* 设置圆弧前景色 */
EXPORT_API void UG_ArcSetColor(UG_OBJECT* obj, UG_COLOR color);

/* 设置圆弧前景渐变色 */
EXPORT_API void UG_ArcSetGradColor(UG_OBJECT* obj, UG_COLOR color);

/* 设置圆弧背景色 */
EXPORT_API void UG_ArcSetBackColor(UG_OBJECT* obj, UG_COLOR color);

/* 设置圆弧背景透明度 */
EXPORT_API void UG_ArcSetBackOpa(UG_OBJECT* obj, UG_U8 opa);

/* 设置圆弧前景起始角（必须要在背景角度范围内）
angle取值范围为0~3600，每1个单位表示0.1°
*/
EXPORT_API void UG_ArcSetStartAngle(UG_OBJECT* obj, UG_U16 angle);

/* 设置圆弧前景结束角（必须要在背景角度范围内）
angle取值范围为0~3600，每1个单位表示0.1°
*/
EXPORT_API void UG_ArcSetEndAngle(UG_OBJECT* obj, UG_U16 angle);

/* 设置圆弧背景起始、结束角
start_angle和end_angle的取值范围为0~3600，每1个单位表示0.1°
*/
EXPORT_API void UG_ArcSetBackAngle(UG_OBJECT* obj, UG_U16 start_angle, UG_U16 end_angle);

/* 设置圆弧的角度增加方向，0为逆时针方向，1为顺时针方向*/
EXPORT_API void UG_ArcSetClockwise(UG_OBJECT* obj, UG_U8 true_or_false);

/* 设置圆弧的整体旋转角度
rot取值范围为0~3600，每1个单位表示0.1°
*/
EXPORT_API void UG_ArcSetRotation(UG_OBJECT* obj, UG_U16 rot);

/* 把圆弧当成进度条使用时，这个接口给进度设定一个最小值，比如0 */
EXPORT_API void UG_ArcSetProgressMin(UG_OBJECT* obj, UG_S16 min);

/* 把圆弧当成进度条使用时，这个接口给进度设定一个最大值，比如100 */
EXPORT_API void UG_ArcSetProgressMax(UG_OBJECT* obj, UG_S16 max);

/* 把圆弧当成进度条使用时，这个接口设置进度值 */
EXPORT_API void UG_ArcSetProgress(UG_OBJECT* obj, UG_S16 value);

/* 把圆弧当成进度条使用时，以下3个接口获取当前进度值/最小值/最大值 */
EXPORT_API UG_S16 UG_ArcGetProgress(UG_OBJECT* obj);
EXPORT_API UG_S16 UG_ArcGetProgressMin(UG_OBJECT* obj);
EXPORT_API UG_S16 UG_ArcGetProgressMax(UG_OBJECT* obj);

#ifdef __cplusplus
}
#endif

#endif