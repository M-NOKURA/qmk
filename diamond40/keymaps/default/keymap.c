#include QMK_KEYBOARD_H
#include "keymap_japanese.h"

// For Layers
enum {
    _BASE = 0,
    _WIN = 1,
    _FUNC = 2,
    _NUM = 3
};

// For Tap Dance
enum {
    TD_ESC = 0,
    TD_F2 = 1,
    TD_CAD = 2
};

// trigger for keys without double tapped in normal use
// qk_tap_dance_action_t tap_dance_actions[] = {
tap_dance_action_t tap_dance_actions[] = { // QMK 0.21から
    [TD_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC),
    [TD_F2] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_F2),
    [TD_CAD] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, LCA(KC_DEL))
};

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        TD(TD_ESC),    TD(TD_F2), KC_E,    KC_R,   KC_T,                KC_Y,         KC_U,          KC_I,       KC_O,       KC_P,
        LT(2,KC_A),    KC_S,      KC_D,    KC_F,   KC_G,                KC_H,         LT(2,KC_J),    KC_K,       KC_L,       KC_MINUS,
        LSFT_T(KC_Z),  KC_X,      KC_C,    KC_V,   KC_B,                KC_N,         KC_M,          KC_COMM,    KC_DOT,     RSFT_T(KC_ENT),
        LCTL_T(KC_F2), KC_LALT,   KC_LGUI, KC_ESC, LT(2,KC_BSPC),       LT(2,KC_SPC), LT(3,KC_RGUI), KC_SLSH,    KC_LBRC,    LSFT(KC_INT1)
    ),
    [_WIN] = LAYOUT(
        KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS, KC_TRNS,      KC_TRNS,    KC_TRNS,         KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS, KC_TRNS,      KC_TRNS,    KC_TRNS,         KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS, KC_TRNS,      KC_TRNS,    KC_TRNS,         KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_LGUI,    LALT_T(JP_MHEN), KC_TRNS, KC_TRNS,      KC_TRNS,    RALT_T(JP_HENK), KC_TRNS,    TD(TD_CAD), KC_TRNS
    ),
    [_FUNC] = LAYOUT(
        KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F10,         KC_F12,     LCTL(KC_U), KC_F7,         KC_F8,         LCTL(KC_T),
        KC_HOME,    KC_BSPC,    KC_DEL,     KC_END,     LSG(KC_S),      KC_LEFT,    KC_DOWN,    KC_UP,         KC_RGHT,       KC_SCLN,
        KC_LSFT,    XXXXXXX,    RGB_RMOD,   RGB_MOD,    RGB_TOG,        KC_RBRC,    KC_NUHS,    LSFT(KC_RBRC), LSFT(KC_NUHS), KC_QUOT,
        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_F2,          KC_TAB,     XXXXXXX,    XXXXXXX,       LSFT(KC_LBRC), KC_INT3
    ),
    [_NUM] = LAYOUT(
        LSFT(KC_1), LSFT(KC_2), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5),     LSFT(KC_6),    LSFT(KC_7),    LSFT(KC_8), LSFT(KC_9),    KC_EQL,
        KC_1,       KC_2,       KC_3,       KC_4,       KC_5,           KC_6,          KC_7,          KC_8,       KC_9,          KC_0,
        KC_LSFT,    XXXXXXX,    RGB_RMOD,   RGB_MOD,    RGB_TOG,        LSFT(KC_MINS), LSFT(KC_SCLN), KC_MINUS,   LSFT(KC_QUOT), KC_SLSH,
        QK_BOOT,    DF(0),      DF(1),      XXXXXXX,    KC_F2,          KC_TAB,        XXXXXXX,       XXXXXXX,    KC_VOLD,       KC_VOLU
    )
};
