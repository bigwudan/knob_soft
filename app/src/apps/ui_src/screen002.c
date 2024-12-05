#include "synwit_ui_framework/synwit_ui.h"
#include "appkit/screen_id.h"
#include "appkit/screens/screen002.h"

/*
 * 界面回调函数
 */
static void screen002_on_timer(UG_TIMER * timer, void* data)
{

}

static void screen002_on_msg(UG_MESSAGE * msg, UG_WINDOW * wnd)
{
    /* 界面消息回调函数 */
}

static void screen002_start(UG_ID screen_id, UG_WINDOW *wnd)
{

}

static void screen002_stop(UG_ID screen_id, UG_WINDOW * wnd)
{

}


/*
 * 界面注册对象
 */
const synwit_ug_screen_callback_t screen002_cb_obj = {
    .on_screen_message = screen002_on_msg,
    .on_screen_start = screen002_start,
    .on_screen_stop = screen002_stop,
    .on_screen_timer = screen002_on_timer,
};

