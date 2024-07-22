#ifndef external_bus
#define external_bus
#include <string.h>
#include "esp_err.h"
#include "driver/i2c.h"
#include "i2c_config_base.h"
#include "uart_config_base.h"
#include "bus_i2s.h"


esp_err_t i2c_init(void);
esp_err_t uart_init(void);
int uart_send(const char *text);
int uart_read(char* data);
esp_err_t uart_flush_data();
void i2s_init();

#endif
