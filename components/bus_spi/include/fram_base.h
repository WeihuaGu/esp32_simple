#ifndef __FRAM_BASE_H__
#define __FRAM_BASE_H__
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"
#include "esp_log.h"


#define FRAM_PIN_NUM_CS   5

// 定义操作码
#define WREN 0x06
#define WRDI 0x04
#define RDSR 0x05
#define WRSR 0x01
#define READ 0x03
#define WRITE 0x02

// 读取 FRAM 数据
esp_err_t fram_read(uint16_t addr, uint8_t *data, size_t len);


// 写入 FRAM 数据
esp_err_t fram_write(uint16_t addr, uint8_t *data, size_t len);


void fram_test();





#endif

