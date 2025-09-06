#ifndef __SPI_CS_H__
#define __SPI_CS_H__

#define SPI_DEVICE_MAX 3

#define FRAM_PIN_NUM_CS 7
#define FLASH_PIN_NUM_CS 10
#define PSRAM_PIN_NUM_CS 5
// 定义SPI设备的枚举
typedef enum {
    SPI_DEVICE_FRAM,
    SPI_DEVICE_FLASH,
    SPI_DEVICE_PSRAM
} spi_device_type_t;

#endif

