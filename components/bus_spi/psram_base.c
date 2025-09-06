#include <string.h>
#include "bus_spi.h"
#include "spi_cs.h"
#include "psram_base.h"

uint8_t device_id[8] = {0};
// 读取设备 ID
void read_device_id() {
    esp_err_t ret;
    uint8_t tx_buffer[4];
    tx_buffer[0] = PSRAM_READ_ID_CMD;
    tx_buffer[1] = 0xff;
    tx_buffer[2] = 0xff;
    tx_buffer[3] = 0xff;
    ret = send_command_array_withreturn(SPI_DEVICE_PSRAM, tx_buffer, sizeof(tx_buffer), device_id, sizeof(device_id));
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to read data: %s", esp_err_to_name(ret));
    }
    // 制造商ID
    ESP_LOGI("PSRAM", "Manufacturer ID: 0x%02X", device_id[0]);
    ESP_LOGI("PSRAM", "KGD: 0x%02X", device_id[1]);
    ESP_LOGI("PSRAM", "EID[47:0]:");
    ESP_LOGI("PSRAM", "  EID[47:40]: 0x%02X (Byte 2)", device_id[2]);
    ESP_LOGI("PSRAM", "  EID[39:32]: 0x%02X (Byte 3)", device_id[3]);
    ESP_LOGI("PSRAM", "  EID[31:24]: 0x%02X (Byte 4)", device_id[4]);
    ESP_LOGI("PSRAM", "  EID[23:16]: 0x%02X (Byte 5)", device_id[5]);
    ESP_LOGI("PSRAM", "  EID[15:8]:  0x%02X (Byte 6)", device_id[6]);
    ESP_LOGI("PSRAM", "  EID[7:0]:   0x%02X (Byte 7)", device_id[7]);
}

void psram_test() {
    read_device_id();
}

