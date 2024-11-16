#ifndef UGUI_FONT_H
#define UGUI_FONT_H
#include "../ugui_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(1)
typedef struct {
    const UG_U16* unicode_list;
    UG_U16 list_length;

    UG_U16 code_start;
    UG_U16 code_length;
    UG_U16 glyph_id_start;
}UG_CMAP;

typedef struct {
    UG_U32 bitmap_index;
    UG_U8 adv_w;
    UG_U8 box_w;
    UG_U8 box_h;
    UG_S8 ofs_x;
    UG_S8 ofs_y;
}UG_GLYPH_DSC;

typedef struct {
    const UG_CMAP* cmaps;
    UG_U8 cmap_num;

    const UG_U8* bitmap;
    const UG_GLYPH_DSC* glyph_dsc;

    UG_U8 ops_type;
    UG_U8 bpp;
    UG_S16 base_line;
    UG_S16 line_height;
} UG_FONT;
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif