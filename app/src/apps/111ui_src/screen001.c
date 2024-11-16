#include "synwit_ui_framework/synwit_ui.h"
#include "appkit/screen_id.h"
#include "appkit/screens/screen001.h"

/*
 * 界面回调函数
 */
static void screen001_on_timer(UG_TIMER * timer, void* data)
{
    /* 界面定时器回调函数 */
}

static void screen001_on_msg(UG_MESSAGE * msg, UG_WINDOW * wnd)
{
    /* 界面消息回调函数 */
}

static void screen001_start(UG_ID screen_id, UG_WINDOW *wnd)
{
    /* 在界面被显示给用户前，这个接口会被调用。 */

    // 打开下面的注释可以为本界面开启一个每100ms触发一次的定时器
    //synwit_ug_start_scr_timer(100, NULL);
}

static void screen001_stop(UG_ID screen_id, UG_WINDOW * wnd)
{
    /* 准备切换到其它界面前，这个接口会被调用。 */
}


/*
 * 界面注册对象
 */
const synwit_ug_screen_callback_t screen001_cb_obj = {
    .on_screen_message = screen001_on_msg,
    .on_screen_start = screen001_start,
    .on_screen_stop = screen001_stop,
    .on_screen_timer = screen001_on_timer,
};

