#ifndef __PSRAM_BASE_H__
#define __PSRAM_BASE_H__
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

// ESP-PSRAM64(H) 命令定义 (SPI Mode)
#define PSRAM_READ_CMD                  0x03  // 读数据
#define PSRAM_FAST_READ_CMD             0x0B  // 快速读
#define PSRAM_WRITE_CMD                 0x02  // 写数据
#define PSRAM_QUAD_READ_CMD             0xEB  // 四线读
#define PSRAM_QUAD_WRITE_CMD            0x38  // 四线写
					      //
// PSRAM 模式设置命令
#define PSRAM_SET_BURST_LENGTH_CMD      0xC0  // 设置突发长度
#define PSRAM_READ_ID_CMD               0x9F  // 读ID
#define PSRAM_RESET_ENABLE_CMD          0x66  // 使能复位
#define PSRAM_RESET_CMD                 0x99  // 执行复位

static const char *TAG = "Tflash";


void psram_test();


#endif

