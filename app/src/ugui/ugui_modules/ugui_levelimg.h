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

/* ���÷ּ�ͼ�񼶱� */
EXPORT_API UG_RESULT UG_LevelImgSetLevel(UG_OBJECT* obj, UG_U16 level);

/* ��ȡ�ּ�ͼ��ļ������� */
EXPORT_API UG_U16 UG_LevelImgGetLevelNum(UG_OBJECT* obj);

/* ��ȡ�ּ�ͼ��ǰ���� */
EXPORT_API UG_U16 UG_LevelImgGetLevel(UG_OBJECT* obj);

/* ֹͣ�Զ����� */
EXPORT_API void UG_LevelImgStopAutoAnim(UG_OBJECT* obj);

/* ��ʼ�Զ����� */
EXPORT_API void UG_LevelImgStartAutoAnim(UG_OBJECT* obj);

/* ���÷ּ�ͼ���Զ����Ž�����Ļص�����cb��
 (ֻ�е�UI Creator���ּ�ͼ����ظ��������ò�Ϊ0ʱ��cb���л��ᱻ�ص�  
 */
EXPORT_API void UG_LevelImgSetAnimReadyCb(UG_OBJECT* obj, void (*cb)(UG_OBJECT* obj, void* data), void* data);

/* ��������ɫ(���ԻҶ�ͼƬ��Ч) */
EXPORT_API UG_RESULT UG_LevelImgSetRecolor(UG_OBJECT* obj, UG_COLOR recolor);

#ifdef __cplusplus
}
#endif
#endif