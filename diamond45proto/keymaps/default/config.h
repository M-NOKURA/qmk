#pragma once

#define TAPPING_TERM 150 // default 200ms

// AZ1UBALL
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
#define PIMORONI_TRACKBALL_SCALE 3
#define POINTING_DEVICE_TASK_THROTTLE_MS 1
#define POINTING_DEVICE_ROTATION_90

// AZ1UBALL with QMK 0.19
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 9

// For Raspberry Pi Pico
// Switch I2C default to I2C0 with halconf.h and mcuconf.h due to assign I2C1 by QMK default
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP12
#define I2C1_SCL_PIN GP13