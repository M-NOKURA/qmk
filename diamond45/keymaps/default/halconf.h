// Switch I2C default to I2C0 with halconf.h and mcuconf.h due to assign I2C1 by QMK default

#pragma once

#define HAL_USE_I2C TRUE

#include_next <halconf.h>
