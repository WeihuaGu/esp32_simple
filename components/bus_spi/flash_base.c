#include <string.h>
#include "bus_spi.h"
#include "spi_cs.h"
#include "flash_base.h"

uint8_t device_id[3] = {0};
// 读取设备 ID
void read_device_id() {
    esp_err_t ret;
    uint8_t tx_buffer[1];
    tx_buffer[0] = W25Q128JV_READ_ID;
    ret = send_command_array_withreturn(SPI_DEVICE_FLASH, tx_buffer, sizeof(tx_buffer), device_id, sizeof(device_id));
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to read data: %s", esp_err_to_name(ret));
    }
    ESP_LOGI(TAG, "Manufacturer ID: 0x%02X", device_id[0]);
    ESP_LOGI(TAG, "Memory Type: 0x%02X", device_id[1]);
    ESP_LOGI(TAG, "Device ID: 0x%02X", device_id[2]);
}

void flash_test() {
    read_device_id();
}

