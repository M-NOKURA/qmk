#include QMK_KEYBOARD_H
#include <print.h>
// #include "quantum.h"
// #include "i2c_master.h"
#include "keymap_japanese.h"
#include "os_detection.h"

// レイヤー
enum {
    _BASE = 0,
    _WIN = 1,
    _FUNC = 2,
    _NUM = 3
};

// タップダンス
enum {
    ESC_LCADEL = 0,
    TAB_LCADEL = 1,
    Q_ALTF4 = 2,
    W_F2 = 3,
    TD_CAD = 4
};

tap_dance_action_t tap_dance_actions[] = { // QMK 0.21から
    [ESC_LCADEL] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, LCA(KC_DEL)),
    [TAB_LCADEL] = ACTION_TAP_DANCE_DOUBLE(KC_TAB, LCA(KC_DEL)),
    [Q_ALTF4] = ACTION_TAP_DANCE_DOUBLE(KC_Q, LALT(KC_F4)),
    [W_F2] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_F2),
    [TD_CAD] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, LCA(KC_DEL))
};

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        TD(TAB_LCADEL),TD(Q_ALTF4), TD(W_F2),   KC_E,         KC_R,           KC_T,                               KC_Y,           KC_U,             KC_I,           KC_O,           KC_P,           KC_BSPC,
        LT(2,KC_ENT),  LT(2,KC_A),  KC_S,       KC_D,         KC_F,           KC_G,                               KC_H,           KC_J,             KC_K,           KC_L,           KC_MINUS,       KC_ENT,
        KC_LSFT,       KC_Z,        KC_X,       KC_C,         KC_V,           KC_B,                               KC_N,           KC_M,             KC_COMM,        KC_DOT,         KC_UP,          RSFT_T(KC_INT1),
        KC_LCTL,       KC_LALT,     KC_LGUI,    KC_LGUI,      KC_LGUI,        LT(3,KC_ESC),    KC_F17, KC_F18,    LT(3,KC_SPC),   LT(3,KC_RGUI),    LT(3,KC_SLSH),  KC_LEFT,        KC_DOWN,        KC_RGHT
    ),
    [_WIN] = LAYOUT(
        KC_TRNS,       KC_TRNS,     KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,                            KC_TRNS,        KC_TRNS,          KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,
        LT(2,KC_F2),   KC_TRNS,     KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,                            KC_TRNS,        KC_TRNS,          KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,       KC_TRNS,     KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,                            KC_TRNS,        KC_TRNS,          KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS,
        KC_TRNS,       KC_LGUI,     KC_LALT,    KC_LALT,      LALT_T(JP_MHEN),KC_TRNS,         KC_F17, KC_F18,    KC_TRNS,        RALT_T(JP_HENK),  KC_TRNS,        KC_TRNS,        KC_TRNS,        KC_TRNS
    ),
    [_FUNC] = LAYOUT(
        LCTL(KC_TAB),  LCTL(KC_1),  KC_F2,      KC_F4,        KC_F5,          KC_F10,                             KC_F12,         LCTL(KC_U),       KC_F7,          KC_F8,          LCTL(KC_T),     KC_INT3,
        KC_TRNS,       KC_HOME,     KC_BSPC,    KC_DEL,       KC_END,         LSG(KC_S),                          KC_LEFT,        KC_DOWN,          KC_UP,          KC_RGHT,        KC_SCLN,        KC_QUOT,
        KC_TRNS,       XXXXXXX,     XXXXXXX,    XXXXXXX,      XXXXXXX,        XXXXXXX,                            KC_RBRC,        KC_NUHS,          LSFT(KC_RBRC),  LSFT(KC_NUHS),  LCTL(KC_HOME),  KC_INT1,
        KC_TRNS,       XXXXXXX,     XXXXXXX,    XXXXXXX,      XXXXXXX,        KC_PGDN,         KC_F17, KC_F18,    KC_PGUP,        XXXXXXX,          KC_LBRC,        LCTL(KC_PGUP),  LCTL(KC_END),   LCTL(KC_PGDN)
    ),
    [_NUM] = LAYOUT(
        LCA(KC_DEL),   LSFT(KC_1),  LSFT(KC_2), LSFT(KC_3),   LSFT(KC_4),     LSFT(KC_5),                         LSFT(KC_6),     LSFT(KC_7),       LSFT(KC_8),     LSFT(KC_9),     KC_EQL,         KC_INT3,
        KC_DOT,        KC_1,        KC_2,       KC_3,         KC_4,           KC_5,                               KC_6,           KC_7,             KC_8,           KC_9,           KC_0,           KC_QUOT,
        KC_TRNS,       DF(0),       DF(1),      XXXXXXX,      XXXXXXX,        XXXXXXX,                            LSFT(KC_MINS),  LSFT(KC_SCLN),    KC_MINUS,       LSFT(KC_QUOT),  KC_VOLU,        XXXXXXX,
        KC_TRNS,       XXXXXXX,     XXXXXXX,    XXXXXXX,      LCA(KC_DEL),    KC_PGDN,         KC_F17, KC_F18,    KC_PGUP,        XXXXXXX,          XXXXXXX,        XXXXXXX,        KC_VOLD,        XXXXXXX
    )
};

// -----------------------------------------------------------------------------
// 起動後処理
// -----------------------------------------------------------------------------
void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
#endif

#ifdef OS_DETECTION_ENABLE
    wait_ms(500);
    switch (detected_host_os()) {
        case OS_WINDOWS:
            layer_off(_BASE);
            layer_on(_WIN);
            // rgblight_setrgb(RGB_BLUE);
            break;
        // case OS_MACOS:
        //     layer_off(_WIN);
        //     layer_on(_BASE);
        //     break;
        default:
            // rgblight_setrgb(RGB_RED);
            break;
    }
#endif
}

// -----------------------------------------------------------------------------
// キー入力時関数
// -----------------------------------------------------------------------------
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    // dprintf("keycode: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
#endif

    switch (keycode) {
        case DF(0):
            if (record->event.pressed) {
                layer_off(_WIN);
                layer_on(_BASE);
                // rgblight_setrgb(RGB_RED);
            }
            break;
        case DF(1):
            if (record->event.pressed) {
                layer_off(_BASE);
                layer_on(_WIN);
                // rgblight_setrgb(RGB_BLUE);
            }
            break;
        case KC_HOME:
            if (record->event.pressed) {
                if(layer_state_is(_BASE)){ tap_code16(LGUI(KC_LEFT)); }
                else { tap_code16(keycode); }
            }
            break;
        case KC_END:
            if (record->event.pressed) {
                if(layer_state_is(_BASE)){ tap_code16(LGUI(KC_RGHT)); }
                else { tap_code16(keycode); }
            }
            break;
        case KC_F17:
            break;
        case KC_F18:
            break;
        case KC_F19:
            // パワーポイント プレゼンモード 2画面切り替え
            if (record->event.pressed) {
                tap_code16(LSFT(KC_F5)); // プレゼン開始
                tap_code16(LSFT(KC_F10)); // 右クリック
                tap_code16(KC_O);
                tap_code16(KC_L);
                return false;
            }
        default:
            break;
    }

    return true;
}

// -----------------------------------------------------------------------------
// コンボキー(レイヤーによって切り替え)
// -----------------------------------------------------------------------------
enum combo_events {
    MSCOMBO4,
    CTLALTDELCOMBO,
};

const uint16_t PROGMEM mscombo4[] = {KC_BTN1, KC_BTN2, COMBO_END};
const uint16_t PROGMEM ctlaltdelcombo[] = {KC_LCTL, KC_LALT, KC_BSPC, COMBO_END};

combo_t key_combos[] = {
    // COMBO(mscombo4, KC_BTN4),
    // COMBO(mscombo4, LGUI(KC_LEFT)),
    [MSCOMBO4] = COMBO_ACTION(mscombo4),
    [CTLALTDELCOMBO] = COMBO_ACTION(ctlaltdelcombo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case MSCOMBO4:
            if (pressed) {
                if(layer_state_is(_BASE)){
                    tap_code16(LGUI(KC_LEFT));
                }
                else{
                    tap_code16(LALT(KC_LEFT));
                }
            }
            break;
        case CTLALTDELCOMBO:
            if (pressed) {
                if(layer_state_is(_WIN)){
                    tap_code16(LCA(KC_DEL));
                }
            }
            break;
    }
}
