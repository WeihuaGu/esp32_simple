#ifndef external_bus
#define external_bus
#include <string.h>
#include "esp_err.h"
#include "driver/i2c.h"
#include "i2c_config_base.h"
#include "bus_i2s.h"
#include "max98357.h"


esp_err_t i2c_init(void);
void i2s_init();
void dac_one_shot_init();

#endif
