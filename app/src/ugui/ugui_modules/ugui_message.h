#ifndef UGUI_MESSAGE_H
#define UGUI_MESSAGE_H
#include "../ugui_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    UG_U8 type;
    UG_U8 id;
    UG_ID sub_id;
    UG_U8 event;
    void* src;
} UG_MESSAGE;

#define MSG_TYPE_NONE                                 0
#define MSG_TYPE_WINDOW                               1
#define MSG_TYPE_OBJECT                               2

#ifdef __cplusplus
}
#endif
#endif