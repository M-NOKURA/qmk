#include QMK_KEYBOARD_H
#include <print.h> // Debug
#include "quantum.h"
#include "i2c_master.h"

// Debug
void keyboard_post_init_user(void) {
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
}

// AZ1UBALL
uint8_t AZ1UBALL_DEVADDR = 0x0A; // https://booth.pm/ja/items/4202085
uint8_t AZ1UBALL_INIT_COMMAND = 0x91; // https://github.com/palette-system/az1uball/blob/main/firmware/tiny424_trackball/tiny424_trackball.ino
uint16_t I2C_TRANSMIT_TIMEOUT = 10;

// マウスレイヤー w/ QMK 0.18
// https://github.com/takashicompany/qmk_firmware/blob/minizone/pimoroni_trackball/keyboards/takashicompany/minizone/keymaps/pimoroni_trackball/keymap.c
// enum custom_keycodes {
//     KC_MY_BTN1 = SAFE_RANGE,    // 左クリック
//     KC_MY_BTN2,                 // 中クリック
//     KC_MY_BTN3,                 // 右クリック
//     KC_MY_SCR,                  // スクロール
// };
//
// enum click_state {
//     NONE = 0,
//     WAITING,    // マウスレイヤーが有効になるのを待つ。 Wait for mouse layer to activate.
//     CLICKABLE,  // マウスレイヤー有効になりクリック入力が取れる。 Mouse layer is enabled to take click input.
//     CLICKING,   // クリック中。 Clicking.
//     SCROLLING   // スクロール中。 Scrolling.
// };
//
// enum click_state state;     // 現在のクリック入力受付の状態 Current click input reception status
// uint16_t click_timer;       // タイマー。状態に応じて時間で判定する。 Timer. Time to determine the state of the system.
// uint16_t to_clickable_time = 1000;   // この秒数(千分の一秒)、WAITING状態ならクリックレイヤーが有効になる。  For this number of seconds (milliseconds), if in WAITING state, the click layer is activated.
// uint16_t to_reset_time = 1000; // この秒数(千分の一秒)、CLICKABLE状態ならクリックレイヤーが無効になる。 For this number of seconds (milliseconds), the click layer is disabled if in CLICKABLE state.
// uint16_t click_layer = 3;   // マウス入力が可能になった際に有効になるレイヤー。Layers enabled when mouse input is enabled
// int16_t scroll_v_mouse_interval_counter;   // 垂直スクロールの入力をカウントする。　Counting Vertical Scroll Inputs
// int16_t scroll_h_mouse_interval_counter;   // 水平スクロールの入力をカウントする。  Counts horizontal scrolling inputs.
// int16_t scroll_v_threshold = 30;    // この閾値を超える度に垂直スクロールが実行される。 Vertical scrolling is performed each time this threshold is exceeded.
// int16_t scroll_h_threshold = 30;    // この閾値を超える度に水平スクロールが実行される。 Each time this threshold is exceeded, horizontal scrolling is performed.
// int16_t after_click_lock_movement = 0;      // クリック入力後の移動量を測定する変数。 Variable that measures the amount of movement after a click input.

int16_t mouse_record_threshold = 30;    // ポインターの動きを一時的に記録するフレーム数。 Number of frames in which the pointer movement is temporarily recorded.
int16_t mouse_move_count_ratio = 4;     // ポインターの動きを再生する際の移動フレームの係数。 The coefficient of the moving frame when replaying the pointer movement.
int16_t mouse_record_x;
int16_t mouse_record_y;
int16_t mouse_record_count;
int16_t mouse_move_remain_count;
bool is_record_mouse;
bool is_mouse_move_x_min;
int16_t mouse_move_x_sign;
int16_t mouse_move_y_sign;
double mouse_interval_delta;
double mouse_interval_counter;

// OLED
static unsigned int type_count = 0;
void count_type(void) { type_count++; }

// レイヤー
enum {
    _BASE = 0,
    _WIN = 1,
    _FUNC = 2,
    _NUM = 3,
    _MOUSE = 9
};

// タップダンス
enum {
    TD_ESC = 0,
    TD_F2 = 1,
    TD_CAD = 2
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_TAB, KC_ESC),
    [TD_F2] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_F2),
    [TD_CAD] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, LCA(KC_DEL))
};

// キーマップ
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_0, QK_BOOT
    ),
    [_WIN] = LAYOUT(
        KC_1, KC_B
    ),
    [_FUNC] = LAYOUT(
        KC_2, KC_B
    ),
    [_NUM] = LAYOUT(
        KC_3, KC_B
    ),
    [_MOUSE] = LAYOUT(
        KC_9, KC_B
    )
};

// -----------------------------------------------------------------------------
// トラックボール制御用 ユーザ関数
// -----------------------------------------------------------------------------
// マウスレイヤー w/ QMK 0.18
// https://github.com/takashicompany/qmk_firmware/blob/minizone/pimoroni_trackball/keyboards/takashicompany/minizone/keymaps/pimoroni_trackball/keymap.c
// クリック用のレイヤーを有効にする
// void enable_click_layer(void) {
//     layer_on(click_layer);
//     click_timer = timer_read();
//     state = CLICKABLE;
// }
//
// クリック用のレイヤーを無効にする
// void disable_click_layer(void) {
//     state = NONE;
//     layer_off(click_layer);
//     scroll_v_mouse_interval_counter = 0;
//     scroll_h_mouse_interval_counter = 0;
// }
//
// 現在クリックが可能な状態か
// bool is_clickable_mode(void) {
//     return state == CLICKABLE || state == CLICKING || state == SCROLLING;
// }

// 自前の絶対数を返す関数
int16_t my_abs(int16_t num) {
    if (num < 0) { num = -num; }
    return num;
}

// 自前の符号を返す関数
int16_t mmouse_move_y_sign(int16_t num) {
    if (num < 0) { return -1; }
    return 1;
}

// -----------------------------------------------------------------------------
// キー入力時 ユーザ関数
// -----------------------------------------------------------------------------
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // デバッグ
#ifdef CONSOLE_ENABLE
    dprintf("keycode: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
#endif

    // OLED
#ifdef OLED_ENABLE
    if (record->event.pressed) {
        count_type();
    }
#endif

    // マウスレイヤー w/ QMK 0.18
    // https://github.com/takashicompany/qmk_firmware/blob/minizone/pimoroni_trackball/keyboards/takashicompany/minizone/keymaps/pimoroni_trackball/keymap.c
#ifdef QMK018
    // クリックレイヤーでのマウスボタン操作は、レイヤーは維持する
    // switch (keycode) {
    //     // 左・中・右クリック
    //     case KC_MY_BTN1:
    //     case KC_MY_BTN2:
    //     case KC_MY_BTN3:
    //     {
    //         report_mouse_t currentReport = pointing_device_get_report();

    //         // どこのビットを対象にするか
    //         uint8_t btn = 1 << (keycode - KC_MY_BTN1);

    //         if (record->event.pressed) {
    //             // ビットORは演算子の左辺と右辺の同じ位置にあるビットを比較して、両方のビットのどちらかが「1」の場合に「1」にします。
    //             currentReport.buttons |= btn;
    //             state = CLICKING;
    //             after_click_lock_movement = 30;
    //         }
    //         else {
    //             // ビットANDは演算子の左辺と右辺の同じ位置にあるビットを比較して、両方のビットが共に「1」の場合だけ「1」にします。
    //             currentReport.buttons &= ~btn;
    //             enable_click_layer();
    //         }

    //         pointing_device_set_report(currentReport);
    //         pointing_device_send();

    //         return false;
    //     }

    //     // スクロールボタン
    //     case KC_MY_SCR:
    //         // スクロール中
    //         if (record->event.pressed) {
    //             state = SCROLLING;
    //         }
    //         // スクロールボタン
    //         else {
    //             enable_click_layer();   // スクロールキーを離した時に再度クリックレイヤーを有効にする。 Enable click layer again when the scroll key is released.
    //         }

    //         return false;

    //     // デフォルト(という名の例外処理)
    //     default:
    //         if  (record->event.pressed) {
    //             disable_click_layer();
    //         }
    // }
#endif

    return true;
}

// -----------------------------------------------------------------------------
// マウス(トラックボール)初期化 ユーザ関数
// -----------------------------------------------------------------------------
void i2c_init_user(void) {
    // AZ1UBALLで加速度をONにする
    i2c_start(AZ1UBALL_DEVADDR);
    i2c_transmit(AZ1UBALL_DEVADDR, &AZ1UBALL_INIT_COMMAND, sizeof(AZ1UBALL_INIT_COMMAND), I2C_TRANSMIT_TIMEOUT);
}

void pointing_device_init_user(void) {
    // Automatic Mouse Layer with QMK 0.19
    set_auto_mouse_layer(9);
    set_auto_mouse_enable(true);
}

// -----------------------------------------------------------------------------
// マウス(トラックボール)操作時 ユーザ関数
// -----------------------------------------------------------------------------
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (!is_record_mouse) {
        if (mouse_report.x != 0 || mouse_report.y != 0) {
            is_record_mouse = true;
            mouse_record_x = 0;
            mouse_record_y = 0;
            mouse_record_count = 0;
        }
    }

    if (is_record_mouse) {
        mouse_record_x += mouse_report.x;
        mouse_record_y += mouse_report.y;
        mouse_record_count++;

        if (mouse_record_count >= mouse_record_threshold) {
            mouse_interval_counter = 0;
            int16_t absX = my_abs(mouse_record_x);
            int16_t absY = my_abs(mouse_record_y);
            is_mouse_move_x_min = absX < absY;

            mouse_move_remain_count = absY + absX;
            mouse_move_remain_count *= mouse_move_count_ratio;

            mouse_move_x_sign = mmouse_move_y_sign(mouse_record_x);
            mouse_move_y_sign = mmouse_move_y_sign(mouse_record_y);

            if (is_mouse_move_x_min) {
                if (mouse_record_x == 0) {
                    mouse_interval_delta = 0;
                }
                else {
                    mouse_interval_delta = (double)absX / (double)absY;
                }
            }
            else {
                if (mouse_record_y == 0) {
                    mouse_interval_delta = 0;
                }
                else {
                    mouse_interval_delta = (double)absY / (double)absX;
                }
            }

            is_record_mouse = false;
            mouse_record_count = 0;
        }
    }

    if (mouse_move_remain_count > 0) {
        mouse_interval_counter += mouse_interval_delta;

        bool can_move_min = mouse_interval_counter >= 1;

        if (can_move_min) {
            mouse_interval_counter -= 1;
        }

        if (is_mouse_move_x_min) {
            mouse_report.y = mouse_move_y_sign;

            if (can_move_min) {
                mouse_report.x = mouse_move_x_sign;
            }
        }
        else {
            mouse_report.x = mouse_move_x_sign;

            if (can_move_min) {
                mouse_report.y = mouse_move_y_sign;
            }
        }

        mouse_report.x *= 1 + mouse_move_remain_count / 10;
        mouse_report.y *= 1 + mouse_move_remain_count / 10;

        mouse_move_remain_count--;
    }
    else {
        mouse_report.x = 0;
        mouse_report.y = 0;
    }

    int16_t current_x = mouse_report.x;
    int16_t current_y = mouse_report.y;
    int16_t current_h = 0;
    int16_t current_v = 0;

    // マウスレイヤー w/ QMK 0.18
    // https://github.com/takashicompany/qmk_firmware/blob/minizone/pimoroni_trackball/keyboards/takashicompany/minizone/keymaps/pimoroni_trackball/keymap.c
#ifdef QMK018
    // if (current_x != 0 || current_y != 0) {
    //     switch (state) {
    //         case CLICKABLE:
    //             click_timer = timer_read();
    //             break;

    //         case CLICKING:
    //             after_click_lock_movement -= my_abs(current_x) + my_abs(current_y);

    //             if (after_click_lock_movement > 0) {
    //                 current_x = 0;
    //                 current_y = 0;
    //             }

    //             break;

    //         case SCROLLING:
    //         {
    //             int8_t rep_v = 0;
    //             int8_t rep_h = 0;

    //             // 垂直スクロールの方の感度を高める。 Increase sensitivity toward vertical scrolling.
    //             if (my_abs(current_y) * 2 > my_abs(current_x)) {
    //                 scroll_v_mouse_interval_counter += current_y;

    //                 while (my_abs(scroll_v_mouse_interval_counter) > scroll_v_threshold) {
    //                     if (scroll_v_mouse_interval_counter < 0) {
    //                         scroll_v_mouse_interval_counter += scroll_v_threshold;
    //                         rep_v += scroll_v_threshold;
    //                     }
    //                     else {
    //                         scroll_v_mouse_interval_counter -= scroll_v_threshold;
    //                         rep_v -= scroll_v_threshold;
    //                     }
    //                 }
    //             }
    //             else {
    //                 scroll_h_mouse_interval_counter += current_x;

    //                 while (my_abs(scroll_h_mouse_interval_counter) > scroll_h_threshold) {
    //                     if (scroll_h_mouse_interval_counter < 0) {
    //                         scroll_h_mouse_interval_counter += scroll_h_threshold;
    //                         rep_h += scroll_h_threshold;
    //                     }
    //                     else {
    //                         scroll_h_mouse_interval_counter -= scroll_h_threshold;
    //                         rep_h -= scroll_h_threshold;
    //                     }
    //                 }
    //             }

    //             current_h = rep_h / scroll_h_threshold;
    //             current_v = -rep_v / scroll_v_threshold;
    //             current_x = 0;
    //             current_y = 0;
    //         }
    //             break;

    //         case WAITING:
    //             if (timer_elapsed(click_timer) > to_clickable_time) {
    //                 enable_click_layer();
    //             }
    //             break;

    //         default:
    //             click_timer = timer_read();
    //             state = WAITING;
    //     }
    // }
    // else
    // {
    //     switch (state) {
    //         case CLICKING:
    //         case SCROLLING:
    //             break;

    //         case CLICKABLE:
    //             if (timer_elapsed(click_timer) > to_reset_time) {
    //                 disable_click_layer();
    //             }
    //             break;

    //         case WAITING:
    //             if (timer_elapsed(click_timer) > 50) {
    //                 state = NONE;
    //             }
    //             break;

    //         default:
    //             state = NONE;
    //     }
    // }
#endif

    mouse_report.x = current_x;
    mouse_report.y = current_y;
    mouse_report.h = current_h;
    mouse_report.v = current_v;

    return mouse_report;
}

// -----------------------------------------------------------------------------
// OLED
// -----------------------------------------------------------------------------
#ifdef OLED_ENABLE

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void oled_write_2digit(unsigned int digit) {
    static char buf[6];
    itoa(digit, buf, 10);
    if (digit < 10) {
        oled_write_char('0', false);
    }
    oled_write(buf, false);
}

void oled_write_uptime(void) {
    static uint32_t uptime_s;
    uptime_s = timer_read32() / 1000;
    oled_write_P(PSTR("Upt:"), false);
    oled_write_char('\n', false);

    // hour
    oled_write_2digit((uptime_s / 3600) % 60);
    // oled_write_char(':', false);

    // minutes
    oled_write_2digit((uptime_s / 60) % 60);
    oled_write_char(':', false);
    oled_write_char('\n', false);
    oled_write_char('\n', false);

    // seconds
    // oled_write_2digit(uptime_s % 60);
    // oled_write_char('\n', false);
}

void oled_write_type_count(void) {
    static char type_count_str[7];
    oled_write_P(PSTR("Cnt:"), false);
    oled_write_char('\n', false);

    itoa(type_count, type_count_str, 10);
    oled_write_ln(type_count_str, false);
    oled_write_char('\n', false);
    oled_write_char('\n', false);
}

bool oled_task_user(void) {
    oled_write_P(PSTR("Lyr:\n"), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Base\n\n"), false);
            break;
        case _WIN:
            oled_write_P(PSTR("Win\n\n"), false);
            break;
        case _FUNC:
            oled_write_P(PSTR("Func\n\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Num\n\n"), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("Mouse\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef\n"), false);
    }

    oled_write_uptime();
    oled_write_type_count();

    return false;
}
#endif