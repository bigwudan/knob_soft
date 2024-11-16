#ifndef UGUI_TIMER_H
#define UGUI_TIMER_H
#include "../ugui_config.h"
#include "ugui_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef UG_HANDLE       UG_TIMER;
typedef void (*UG_ON_TIMER)(UG_TIMER* timer, void* data);

/* 创建一个定时器，以period为间隔(ms为单位)定时触发
timer_cb回调，当timer_cb回调被触发时，可以携带用户
自定义数据user_data，如不需要，那么user_data可以传NULL。
auto_start参数传入1表示立即启动定时器，传入0则定时器创
建后默认处于停止状态，需要自定调用UG_TimerStart()接口
启动。
函数调用成功返回一个定时器对象指针，否则返回NULL
*/
EXPORT_API UG_TIMER* UG_TimerCreate(UG_U32 period, UG_ON_TIMER timer_cb, void* user_data, UG_U8 auto_start);

/* 设置定时器触发间隔，以ms为单位 */
EXPORT_API void UG_TimerSetPeriod(UG_TIMER* timer, UG_U32 period);

/* 启动定时器 */
EXPORT_API void UG_TimerStart(UG_TIMER* timer);

/* 暂停定时器 */
EXPORT_API void UG_TimerStop(UG_TIMER* timer);

/* 删除定时器 */
EXPORT_API void UG_TimerDelete(UG_TIMER* timer);

#ifdef __cplusplus
}
#endif
#endif