#include "synwit_ui_framework/synwit_ui.h"
#include "synwit_ui_framework/synwit_ui_internal.h"
#include "screen_id.h"

/*
 * Register screens
 */
APP_OBJ_DECLARE(screen001_cb_obj)

SCREEN_REG_BEGIN
    SCREEN_BIND(SCREEN001, screen001_cb_obj)
SCREEN_REG_END

#ifndef WIN32
/*
 * Register widgets
 */
WIDGET_OBJ_DECLARE(ug_image_reg)

WIDGET_REG_BEGIN(g_ug_widget_reg_map)
    WIDGET_REG_BIND(ug_image_reg)
WIDGET_REG_END(g_ug_widget_reg_map, void)
#endif /* WIN32 */

