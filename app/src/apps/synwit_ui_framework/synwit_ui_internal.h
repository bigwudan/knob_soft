#ifndef SYNWIT_UGUI_UI_INTERNAL_H
#define SYNWIT_UGUI_UI_INTERNAL_H

#include "synwit_ui.h"

#ifdef __cplusplus
extern "C" {
#endif

/* App object */
typedef struct {
    UG_ID scr_id;
    const synwit_ug_screen_callback_t* app_obj;
}ug_scr_reg_t;

EXPORT_API void _synwit_ug_set_screen_map(const ug_scr_reg_t* map, int num);

#define APP_OBJ_DECLARE(obj)  extern const synwit_ug_screen_callback_t obj;
#define SCREEN_REG_BEGIN   const ug_scr_reg_t g_screen_map[] = {
#define SCREEN_REG_END   }; void _ug_app_screen_register() { _synwit_ug_set_screen_map(g_screen_map, sizeof(g_screen_map) / sizeof(g_screen_map[0])); }
#define SCREEN_BIND(id, obj) \
    { .scr_id = id, .app_obj = &obj },


/* Widget object */
typedef struct {
	UG_ID cls_id;
	int (*construct)(UG_WINDOW* win, UG_ID cls_id, UG_ID widget_id);
	void (*on_screen_start)(UG_OBJECT* obj);
	void (*on_screen_stop)(UG_OBJECT* obj);
}ug_widget_reg_t;

EXPORT_API void _synwit_ug_set_widget_map(const ug_widget_reg_t** map, int num);

#define WIDGET_OBJ_DECLARE(obj)  extern const ug_widget_reg_t obj;
#define WIDGET_REG_BEGIN(name)   const ug_widget_reg_t *name[] = {
#define WIDGET_REG_END(name, sign)   }; sign _ug_widget_register(){ _synwit_ug_set_widget_map(name, UG_DIM(name));}
#define WIDGET_REG_BIND(obj) &obj,

EXPORT_API void _ug_widget_register();

#ifdef WIN32_UGUI_SIMULATION
	void _ug_app_screen_register();
	#define SIMULATION_EXPORT __declspec(dllexport)
#else
	#ifndef WIN32
		void _ug_app_screen_register();
	#endif
	#define SIMULATION_EXPORT 
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* SYNWIT_UGUI_UI_INTERNAL_H */