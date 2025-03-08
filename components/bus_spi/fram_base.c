#include <string.h>
#include "bus_spi.h"
#include "fram_base.h"

// 写入 FRAM 数据
esp_err_t fram_write(uint16_t addr, uint8_t *data, size_t len) {
    esp_err_t ret;
    // 先使能写操作
    uint8_t wren_cmd = WREN;
    ret = send_command_array(&wren_cmd, 1);
    if (ret != ESP_OK) {
        return ret;
    }

    uint8_t *tx_buffer = malloc(3 + len); // 命令 + 地址 + 数据
    tx_buffer[0] = WRITE;                 // 写命令
    tx_buffer[1] = (addr >> 8) & 0xFF;  // 地址高位
    tx_buffer[2] = addr & 0xFF;          // 地址低位
    memcpy(&tx_buffer[3], data, len);    // 数据
    // 发送命令和数据
    ret = send_command_array(tx_buffer, len + 3);
    free(tx_buffer);
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to write data: %s", esp_err_to_name(ret));
    }
    return ret;
}

// 读取 FRAM 数据
esp_err_t fram_read(uint16_t addr, uint8_t *data, size_t len) {
    esp_err_t ret;

    uint8_t tx_buffer[3];
    tx_buffer[0] = READ;                 // 读命令
    tx_buffer[1] = (addr >> 8) & 0xFF;  // 地址高位
    tx_buffer[2] = addr & 0xFF;          // 地址低位
					 //
    ret = send_command_array_withreturn(tx_buffer, sizeof(tx_buffer), data, len);
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to read data: %s", esp_err_to_name(ret));
    }
    return ret;
}


void fram_test(){
     // 测试地址
    uint16_t addr = 0x0000;

    // 写入数据到 FRAM
    uint8_t write_data = 0xAB; // 要写入的数据
    if (fram_write(addr, &write_data, 1) == ESP_OK) {
        ESP_LOGI("SPI", "Data written to FRAM at address 0x%hu: 0x%02X", addr, write_data);
    } else {
        ESP_LOGE("SPI", "Failed to write to FRAM");
        return;
    }
    // 从 PSRAM 读取数据
    uint8_t read_data;
    if (fram_read(addr, &read_data, 1) == ESP_OK) {
        ESP_LOGI("SPI", "Data read from FRAM at address 0x%hu: 0x%02X", addr, read_data);
    } else {
        ESP_LOGE("SPI", "Failed to read from FRAM");
        return;
    }
}

