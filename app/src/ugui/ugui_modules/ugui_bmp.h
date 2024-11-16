#ifndef UGUI_BMP_H
#define UGUI_BMP_H
#include "../ugui_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    void* p;
    void* a;
    void* compress_info;
    UG_U16 width;
    UG_U16 height;
    UG_COLOR recolor;
    UG_U8 cf;
    UG_U8 access;
} UG_BMP;

#define BMP_CF_RGB565                       (0)
#define BMP_CF_RGB565_COMPRESSED            (1)
#define BMP_CF_ALPHA_1BPP                   (2)
#define BMP_CF_ALPHA_2BPP                   (3)
#define BMP_CF_ALPHA_4BPP                   (4)
#define BMP_CF_ALPHA_8BPP                   (5)

#define BMP_ACCESS_MODE_MEMCPY        0     // Default
#define BMP_ACCESS_MODE_UI_DATA       1

#ifdef __cplusplus
}
#endif
#endif