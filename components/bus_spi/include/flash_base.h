#ifndef __FLASH_BASE_H__
#define __FLASH_BASE_H__
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

// 定义 W25Q128JV 指令集
#define W25Q128JV_WRITE_ENABLE 0x06
#define W25Q128JV_WRITE_DISABLE 0x04
#define W25Q128JV_READ_ID 0x9F
#define W25Q128JV_READ_DATA 0x03
#define W25Q128JV_PAGE_PROGRAM 0x02
#define W25Q128JV_SECTOR_ERASE 0x20

static const char *TAG = "Tflash";


void flash_test();


#endif

