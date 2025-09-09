#include <string.h>
#include "bus_spi.h"
#include "spi_cs.h"
#include "psram_base.h"

uint8_t device_id[8] = {0};
uint8_t capacity_id = 0x17;
// 读取设备 ID
esp_err_t read_device_id() {
    esp_err_t ret;
    uint8_t tx_buffer[4];
    tx_buffer[0] = PSRAM_READ_ID_CMD;
    tx_buffer[1] = 0xff;
    tx_buffer[2] = 0xff;
    tx_buffer[3] = 0xff;
    ret = send_command_array_withreturn(SPI_DEVICE_PSRAM, tx_buffer, sizeof(tx_buffer), device_id, sizeof(device_id));
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to read data: %s", esp_err_to_name(ret));
	return ret;
    }
    // 制造商ID
    ESP_LOGI("PSRAM", "Manufacturer ID: 0x%02X", device_id[0]);
    ESP_LOGI("PSRAM", "KGD: 0x%02X", device_id[1]);
    /*
    ESP_LOGI("PSRAM", "EID[47:0]:");
    ESP_LOGI("PSRAM", "  EID[47:40]: 0x%02X (Byte 2)", device_id[2]);
    ESP_LOGI("PSRAM", "  EID[39:32]: 0x%02X (Byte 3)", device_id[3]);
    ESP_LOGI("PSRAM", "  EID[31:24]: 0x%02X (Byte 4)", device_id[4]);
    ESP_LOGI("PSRAM", "  EID[23:16]: 0x%02X (Byte 5)", device_id[5]);
    ESP_LOGI("PSRAM", "  EID[15:8]:  0x%02X (Byte 6)", device_id[6]);
    ESP_LOGI("PSRAM", "  EID[7:0]:   0x%02X (Byte 7)", device_id[7]);
    */
    return ESP_OK;
}
esp_err_t psram_read(uint32_t addr,uint8_t *data){
    if(!is_address_valid(addr,capacity_id))
	    return ESP_FAIL;
    esp_err_t ret;
    uint8_t tx_buffer[4];
    tx_buffer[0] = PSRAM_READ_CMD;
    tx_buffer[1] = (addr >> 16) & 0xFF;
    tx_buffer[2] = (addr >> 8) & 0xFF;
    tx_buffer[3] = addr & 0xFF;
    ret = send_command_array_withreturn(SPI_DEVICE_PSRAM, tx_buffer, sizeof(tx_buffer), data, 1);
    return ret;
}
esp_err_t psram_write(uint32_t addr,uint8_t *data){
    if(!is_address_valid(addr,capacity_id))
	    return ESP_FAIL;
    esp_err_t ret;
    uint8_t tx_buffer[5];
    tx_buffer[0] = PSRAM_WRITE_CMD;
    tx_buffer[1] = (addr >> 16) & 0xFF;
    tx_buffer[2] = (addr >> 8) & 0xFF;
    tx_buffer[3] = addr & 0xFF;
    tx_buffer[4] = *data;
    ret = send_command_array_noreturn(SPI_DEVICE_PSRAM, tx_buffer, sizeof(tx_buffer));
    return ret;
}

esp_err_t psram_read_bytes(uint32_t addr,uint8_t *data,size_t len){
    esp_err_t ret;
    for(size_t i=0;i<len;i++){
	ret = psram_read(addr+i,data+i);
	if(ret!=ESP_OK)
		return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t psram_write_bytes(uint32_t addr,uint8_t *data,size_t len){
    esp_err_t ret;
    for(size_t i=0;i<len;i++){
	ret = psram_write(addr+i,data+i);
	if(ret!=ESP_OK)
		return ESP_FAIL;
    }
    return ESP_OK;
}
void psram_test() {
    read_device_id();
    uint8_t a[5];
    uint8_t b[5];
    a[0] = (uint8_t)'p';
    a[1] = (uint8_t)'s';
    a[2] = (uint8_t)'r';
    a[3] = (uint8_t)'a';
    a[4] = (uint8_t)'m';
    uint32_t addr = 0x23;
    psram_write_bytes(addr,&a,sizeof(a));
    psram_read_bytes(addr,&b,sizeof(b));
    for(size_t i=0;i<sizeof(b);i++)
        printf("%c",b[i]);
    printf("\n");
}

