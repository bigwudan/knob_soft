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

/* �����ı����������ɫ */
EXPORT_API UG_RESULT UG_LabelSetForeColor(UG_OBJECT* obj, UG_COLOR fc);

/* �����ı�������ı�����ɫ */
EXPORT_API UG_RESULT UG_LabelSetBackColor(UG_OBJECT* obj, UG_COLOR bc);

/* �����ı�������ı���������ɫ */
EXPORT_API UG_RESULT UG_LabelSetBackGradColor(UG_OBJECT* obj, UG_COLOR grad);

/* �����ı�������ı���ɫ���䷽��
    GRAD_DIR_NONE   ��
    GRAD_DIR_VER    ��ֱ����
    GRAD_DIR_HOR    ˮƽ����
 */
EXPORT_API UG_RESULT UG_LabelSetBackGradDir(UG_OBJECT* obj, UG_U8 dir);

/* �����ı�������ı���͸����,0~255,0��ʾ��ȫ͸����255��ʾ��ȫ��͸�� */
EXPORT_API UG_RESULT UG_LabelSetBackOpa(UG_OBJECT* obj, UG_U8 opa);

/* �����ı����ݣ��ýӿڻὫstr���ݸ���һ�ݣ�������ɺ�str�����û����и��û��ͷ� */
EXPORT_API UG_RESULT UG_LabelSetText(UG_OBJECT* obj, char* str);

/* �����ı����ݣ��ýӿڲ��Ḵ��static_str���ݣ�����ֱ�����øõ�ַ */
EXPORT_API UG_RESULT UG_LabelSetTextStatic(UG_OBJECT* obj, char* static_str);

/* �����ı������� */
EXPORT_API UG_RESULT UG_LabelSetFont(UG_OBJECT* obj, const UG_FONT* font);

/* �����ı��ּ�� */
EXPORT_API UG_RESULT UG_LabelSetHSpace(UG_OBJECT* obj, UG_S8 hs);

/* �����ı��м�� */
EXPORT_API UG_RESULT UG_LabelSetVSpace(UG_OBJECT* obj, UG_S8 vs);

/* �����ı��ж��뷽ʽ���ο�ugui.h�ڵ�Alignments���� */
EXPORT_API UG_RESULT UG_LabelSetAlignment(UG_OBJECT* obj, UG_U8 align);

/* �����ı���ģʽ���ο�ugui.h�ڵ�Text long modes���� */
EXPORT_API UG_RESULT UG_LabelSetLongMode(UG_OBJECT* obj, UG_U8 long_mode);

/* ��ȡ��ǰʹ�õ�������ɫ */
EXPORT_API UG_COLOR UG_LabelGetForeColor(UG_OBJECT* obj);

/* ��ȡ��ǰʹ�õı�����ɫ */
EXPORT_API UG_COLOR UG_LabelGetBackColor(UG_OBJECT* obj);

/* ��ȡ��ǰʹ�õı���͸���� */
EXPORT_API UG_U8 UG_LabelGetBackOpa(UG_OBJECT* obj);

/* ��ȡ��ǰ�ı����� */
EXPORT_API char* UG_LabelGetText(UG_OBJECT* obj);

/* ��ȡ��ǰʹ�õ�����ָ�� */
EXPORT_API UG_FONT* UG_LabelGetFont(UG_OBJECT* obj);

/* ��ȡ�ּ�� */
EXPORT_API UG_S8 UG_LabelGetHSpace(UG_OBJECT* obj);

/* ��ȡ�м�� */
EXPORT_API UG_S8 UG_LabelGetVSpace(UG_OBJECT* obj);

/* ��ȡ��ǰʹ�õ��ı�������ɫ */
EXPORT_API UG_U8 UG_LabelGetAlignment(UG_OBJECT* obj);

#ifdef __cplusplus
}
#endif
#endif