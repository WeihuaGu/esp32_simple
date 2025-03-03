#ifndef __BUS_SPI_H__
#define __BUS_SPI_H__
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"
#include "esp_log.h"

#define PSRAM_HOST    SPI3_HOST
#define PIN_NUM_CLK  18
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23

#define PIN_NUM_WP 32
#define PIN_NUM_HD 27

#define PSRAM_PIN_NUM_CS   5

// 初始化 VSPI 总线
esp_err_t bus_spi_init(void);

// 读取 PSRAM 数据
esp_err_t psram_read(uint32_t addr, uint8_t *data, size_t len);

void psram_test_read();

// 写入 PSRAM 数据
esp_err_t psram_write(uint32_t addr, uint8_t *data, size_t len);

// 读取 PSRAM ID
esp_err_t psram_read_id(uint8_t *id);






#endif // __BUS_SPI_H__

