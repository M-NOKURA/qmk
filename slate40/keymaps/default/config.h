#pragma once

#define TAPPING_TERM 200 // default 200ms
#define RETRO_TAPPING // 入れておかないと CTRL/CMD等とLT (LayerTap)のコンビネーションが効かない(もしくは判定シビア)
// #define PERMISSIVE_HOLD

// AZ1UBALL
// #define DYNAMIC_KEYMAP_LAYER_COUNT 5
// #define PIMORONI_TRACKBALL_SCALE 2
// #define POINTING_DEVICE_TASK_THROTTLE_MS 1 // default: 1
// #define POINTING_DEVICE_ROTATION_90
// #define POINTING_DEVICE_AUTO_MOUSE_ENABLE // Auto Mouse Layer w/ QMK 0.19
// #define AUTO_MOUSE_DEFAULT_LAYER 9

// For Raspberry Pi Pico
// Switch I2C default to I2C0 with halconf.h and mcuconf.h due to assign I2C1 by QMK default
// #define I2C_DRIVER I2CD1
// #define I2C1_SDA_PIN GP12
// #define I2C1_SCL_PIN GP13

// For Promicro Arduino
// #define I2C_DRIVER I2CD0
// #define I2C0_SDA_PIN D1
// #define I2C0_SCL_PIN D0

// Unused Old Functions
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define NO_MUSIC_MODE

// RGB
#define WS2812_DI_PIN B4
#ifdef RGBLIGHT_ENABLE
    #undef RGBLED_NUM
    #define RGBLED_NUM 2
    #define RGBLIGHT_LIMIT_VAL 64 // maximum light brightness default 255
    #define RGBLIGHT_HUE_STEP 8
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 17
    #define RGBLIGHT_SLEEP 300
    #define RGBLIGHT_DEFAULT_HUE 0 // red
    #define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT
    // #define RGBLIGHT_EFFECT_BREATHING
    // #define RGBLIGHT_EFFECT_SNAKE
#endif
