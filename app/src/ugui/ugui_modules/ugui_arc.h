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

/* ����Բ���뾶 */
EXPORT_API void UG_ArcSetRadius(UG_OBJECT* obj, UG_U16 r);

/* ����Բ����ϸ */
EXPORT_API void UG_ArcSetStrokeWidth(UG_OBJECT* obj, UG_U16 width);

/* ����Բ��ǰ��ɫ */
EXPORT_API void UG_ArcSetColor(UG_OBJECT* obj, UG_COLOR color);

/* ����Բ��ǰ������ɫ */
EXPORT_API void UG_ArcSetGradColor(UG_OBJECT* obj, UG_COLOR color);

/* ����Բ������ɫ */
EXPORT_API void UG_ArcSetBackColor(UG_OBJECT* obj, UG_COLOR color);

/* ����Բ������͸���� */
EXPORT_API void UG_ArcSetBackOpa(UG_OBJECT* obj, UG_U8 opa);

/* ����Բ��ǰ����ʼ�ǣ�����Ҫ�ڱ����Ƕȷ�Χ�ڣ�
angleȡֵ��ΧΪ0~3600��ÿ1����λ��ʾ0.1��
*/
EXPORT_API void UG_ArcSetStartAngle(UG_OBJECT* obj, UG_U16 angle);

/* ����Բ��ǰ�������ǣ�����Ҫ�ڱ����Ƕȷ�Χ�ڣ�
angleȡֵ��ΧΪ0~3600��ÿ1����λ��ʾ0.1��
*/
EXPORT_API void UG_ArcSetEndAngle(UG_OBJECT* obj, UG_U16 angle);

/* ����Բ��������ʼ��������
start_angle��end_angle��ȡֵ��ΧΪ0~3600��ÿ1����λ��ʾ0.1��
*/
EXPORT_API void UG_ArcSetBackAngle(UG_OBJECT* obj, UG_U16 start_angle, UG_U16 end_angle);

/* ����Բ���ĽǶ����ӷ���0Ϊ��ʱ�뷽��1Ϊ˳ʱ�뷽��*/
EXPORT_API void UG_ArcSetClockwise(UG_OBJECT* obj, UG_U8 true_or_false);

/* ����Բ����������ת�Ƕ�
rotȡֵ��ΧΪ0~3600��ÿ1����λ��ʾ0.1��
*/
EXPORT_API void UG_ArcSetRotation(UG_OBJECT* obj, UG_U16 rot);

/* ��Բ�����ɽ�����ʹ��ʱ������ӿڸ������趨һ����Сֵ������0 */
EXPORT_API void UG_ArcSetProgressMin(UG_OBJECT* obj, UG_S16 min);

/* ��Բ�����ɽ�����ʹ��ʱ������ӿڸ������趨һ�����ֵ������100 */
EXPORT_API void UG_ArcSetProgressMax(UG_OBJECT* obj, UG_S16 max);

/* ��Բ�����ɽ�����ʹ��ʱ������ӿ����ý���ֵ */
EXPORT_API void UG_ArcSetProgress(UG_OBJECT* obj, UG_S16 value);

/* ��Բ�����ɽ�����ʹ��ʱ������3���ӿڻ�ȡ��ǰ����ֵ/��Сֵ/���ֵ */
EXPORT_API UG_S16 UG_ArcGetProgress(UG_OBJECT* obj);
EXPORT_API UG_S16 UG_ArcGetProgressMin(UG_OBJECT* obj);
EXPORT_API UG_S16 UG_ArcGetProgressMax(UG_OBJECT* obj);

#ifdef __cplusplus
}
#endif

#endif