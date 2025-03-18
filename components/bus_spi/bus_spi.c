#include <string.h>
#include "fram_base.h"
#include "bus_spi.h"

static spi_device_handle_t spi;

esp_err_t bus_spi_init(void) {

    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000, // 20 MHz
        .mode = 0,                          // SPI mode 0
        .spics_io_num = FRAM_PIN_NUM_CS,         // CS pin
        .queue_size = 1                      // Transaction queue size
    };
    // 初始化 SPI 总线
    esp_err_t ret = spi_bus_initialize(USE_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to initialize SPI bus");
        return ret;
    }
    // 添加设备到 SPI 总线
    ret = spi_bus_add_device(USE_SPI_HOST, &devcfg, &spi);
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to add device to SPI bus");
        return ret;
    }

    return ESP_OK;
}


esp_err_t send_command_array(const uint8_t *commands, size_t command_len) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = command_len * 8;  // 每个字节8位
    t.tx_buffer = commands;

    ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to send command array: %s", esp_err_to_name(ret));
    }
    return ret;
}
esp_err_t send_command_array_withreturn(const uint8_t *commands, size_t command_len, uint8_t *rx_data, size_t rx_len) {
    esp_err_t ret;
    spi_transaction_t t;
    uint8_t *tx_buffer_padded = NULL;
    uint8_t *rx_buffer_padded = NULL;
    memset(&t, 0, sizeof(t));
    int dummy_size = rx_len - command_len;
    if(dummy_size == 0){
        t.tx_buffer = commands;
        t.length = command_len * 8;
        t.rxlength = rx_len * 8;
        t.rx_buffer = rx_data;
    }
    if(dummy_size > 0){
        t.length = rx_len * 8;
        tx_buffer_padded = (uint8_t*)malloc(rx_len);
	memcpy(tx_buffer_padded, commands, command_len);
	memset(tx_buffer_padded + command_len,0xFF, dummy_size);
	t.tx_buffer = tx_buffer_padded;
        t.rxlength = rx_len * 8;
        t.rx_buffer = rx_data;
    }
    if(dummy_size < 0){
        t.length = command_len * 8;
        t.tx_buffer = commands;
        t.rxlength = command_len * 8;
        rx_buffer_padded = (uint8_t*)malloc(command_len);
	t.rx_buffer = rx_buffer_padded;
    }
    ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        ESP_LOGE("SPI", "Failed to send command array: %s", esp_err_to_name(ret));
    }
    if(dummy_size > 0){
	free(tx_buffer_padded);
    }
    if(dummy_size < 0){
        memcpy(rx_data,rx_buffer_padded,rx_len);
	free(rx_buffer_padded);
    }
    for(int i=0;i<(t.length)/8;i++){
	     printf("0x%02X ", ((uint8_t*)t.rx_buffer)[i]);
    }


    return ret;
}



