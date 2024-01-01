// Switch I2C default to I2C0 with halconf.h and mcuconf.h due to assign I2C1 by QMK default

#pragma once

#include_next "mcuconf.h"

#undef RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 TRUE

#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 FALSE
