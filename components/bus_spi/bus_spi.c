#include <string.h>
#include "spi_cs.h"
#include "bus_spi.h"

// 存储每个设备的句柄
spi_device_handle_t spi_handles[SPI_DEVICE_MAX];
int cs_nums[SPI_DEVICE_MAX];

esp_err_t bus_initialize(void){
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092,
	.flags = 0
    };
    // 初始化 SPI 总线
    esp_err_t ret = spi_bus_initialize(USE_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    return ret;
}

esp_err_t add_device_flash(void){
    cs_nums[SPI_DEVICE_FLASH] = FLASH_PIN_NUM_CS;
    //add W25Q128JV
    spi_device_interface_config_t devcfg_flash = {
        .clock_speed_hz = 20 * 1000 * 1000, // 1 MHz
        .mode = 0,                          // SPI mode 0
        .spics_io_num = FLASH_PIN_NUM_CS,         // CS pin
        .queue_size = 1                      // Transaction queue size
    };
    esp_err_t ret;
    ret = spi_bus_add_device(USE_SPI_HOST, &devcfg_flash,&spi_handles[SPI_DEVICE_FLASH]);
    if (ret != ESP_OK) {
        return ret;
    }
    return ESP_OK;

}
esp_err_t add_device_fram(void){
    cs_nums[SPI_DEVICE_FRAM] = FRAM_PIN_NUM_CS;
    return ESP_OK;
}
esp_err_t add_device_psram(void){
    cs_nums[SPI_DEVICE_PSRAM] = PSRAM_PIN_NUM_CS;
    spi_device_interface_config_t devcfg_flash = {
        .clock_speed_hz = 20 * 1000 * 1000, // 1 MHz
        .mode = 0,                          // SPI mode 0
        .spics_io_num = PSRAM_PIN_NUM_CS,         // CS pin
        .queue_size = 1                      // Transaction queue size
    };
    esp_err_t ret;
    ret = spi_bus_add_device(USE_SPI_HOST, &devcfg_flash,&spi_handles[SPI_DEVICE_PSRAM]);
    if (ret != ESP_OK) {
        return ret;
    }

    return ESP_OK;
}
esp_err_t devices_add(void){

    esp_err_t ret;
    /*
    ret = add_device_flash();
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to add flash device to SPI bus");
        return ret;
    }
    ret = add_device_fram();
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to add flash device to SPI bus");
        return ret;
    }
    */
    ret = add_device_psram();
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to add flash device to SPI bus");
        return ret;
    }
    return ESP_OK;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
esp_err_t bus_spi_init(void) {
    esp_err_t ret;
    ret = bus_initialize();
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to initialize SPI bus");
        return ret;
    }
    ret = devices_add();
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to add device to SPI bus");
        return ret;
    }

    return ESP_OK;
}

esp_err_t send_command_array_withreturn(int spidevice, const uint8_t *commands, size_t command_len, uint8_t *rx_data, size_t rx_len) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    size_t sum_size;
    if(rx_data == NULL){
	sum_size = command_len;
    }
    else{
	sum_size = command_len + rx_len;
    }
    // 在栈上创建联合缓冲区
    uint8_t tx_buffer[sum_size];
    uint8_t rx_buffer[sum_size];

     // 准备发送缓冲区
    memcpy(tx_buffer, commands, command_len);      // 拷贝命令部分
    if(rx_data != NULL)
    	memset(tx_buffer + command_len, 0x00, rx_len); // 填充Dummy数据部分

    t.user = &cs_nums[spidevice];
    t.tx_buffer = tx_buffer;
    if(rx_data != NULL)
    	t.rx_buffer = rx_buffer;
    t.length = sum_size * 8; // 计算总位数
			     
    printf("commands: ");
    for(int i=0;i<command_len;i++){
             printf("0x%02X ", tx_buffer[i]);
    }
    printf("\n");

    ret = spi_device_polling_transmit(spi_handles[spidevice], &t);
    //ret = spi_device_transmit(spi_handles[spidevice], &t);
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to send command array: %s", esp_err_to_name(ret));
    }

    if(rx_data != NULL){
    memcpy(rx_data,rx_buffer+command_len,rx_len);

    printf("returned rx_data: ");
    for(int i=0;i<command_len+rx_len;i++){
        printf("0x%02X ", rx_buffer[i]);
    }
    printf("\n");
    }

    return ret;
}
esp_err_t send_command_array_noreturn(int spidevice, const uint8_t *commands, size_t command_len) {
    return send_command_array_withreturn(spidevice, commands, command_len ,NULL ,0);
}
//comon use fun
bool is_address_valid(uint32_t address,uint8_t capacity_id) {
    uint32_t max_address;
    // 检查是否已经读取过Device ID
    if (capacity_id == 0) {
        ESP_LOGW("TAG", "Device ID not read yet, please call read_device_id() first");
        return false;
    }
    // 根据容量ID计算最大寻址空间
    max_address = (uint32_t)1 << capacity_id;

    ESP_LOGI("TAG", "Capacity ID: 0x%02X, Max address: 0x%06lX (%lu bytes)",
             capacity_id, max_address, max_address);

    // 检查地址是否在有效范围内
    if (address < max_address) {
        ESP_LOGD("TAG", "Address 0x%06lX is valid", address);
        return true;
    } else {
        ESP_LOGW("TAG", "Address 0x%06lX exceeds maximum 0x%06lX", address, max_address - 1);
        return false;
    }
}


