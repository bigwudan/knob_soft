#ifndef UGUI_TIMER_H
#define UGUI_TIMER_H
#include "../ugui_config.h"
#include "ugui_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef UG_HANDLE       UG_TIMER;
typedef void (*UG_ON_TIMER)(UG_TIMER* timer, void* data);

/* ����һ����ʱ������periodΪ���(msΪ��λ)��ʱ����
timer_cb�ص�����timer_cb�ص�������ʱ������Я���û�
�Զ�������user_data���粻��Ҫ����ôuser_data���Դ�NULL��
auto_start��������1��ʾ����������ʱ��������0��ʱ����
����Ĭ�ϴ���ֹͣ״̬����Ҫ�Զ�����UG_TimerStart()�ӿ�
������
�������óɹ�����һ����ʱ������ָ�룬���򷵻�NULL
*/
EXPORT_API UG_TIMER* UG_TimerCreate(UG_U32 period, UG_ON_TIMER timer_cb, void* user_data, UG_U8 auto_start);

/* ���ö�ʱ�������������msΪ��λ */
EXPORT_API void UG_TimerSetPeriod(UG_TIMER* timer, UG_U32 period);

/* ������ʱ�� */
EXPORT_API void UG_TimerStart(UG_TIMER* timer);

/* ��ͣ��ʱ�� */
EXPORT_API void UG_TimerStop(UG_TIMER* timer);

/* ɾ����ʱ�� */
EXPORT_API void UG_TimerDelete(UG_TIMER* timer);

#ifdef __cplusplus
}
#endif
#endif