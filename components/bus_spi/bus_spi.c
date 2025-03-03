#include "bus_spi.h"
#include <string.h>

static spi_device_handle_t spi;
gpio_config_t io_conf_cs = {
        .pin_bit_mask = (1ULL << PSRAM_PIN_NUM_CS),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
};

esp_err_t bus_spi_init(void) {

    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = PIN_NUM_WP,
        .quadhd_io_num = PIN_NUM_HD,
        .max_transfer_sz = 4096
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1, // 40 MHz
        .mode = 0,                          // SPI mode 0
        .spics_io_num = PSRAM_PIN_NUM_CS,         // CS pin
        .queue_size = 3                      // Transaction queue size
    };
    // 初始化 SPI 总线
    esp_err_t ret = spi_bus_initialize(PSRAM_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to initialize SPI bus");
        return ret;
    }
    // 添加设备到 SPI 总线
    ret = spi_bus_add_device(PSRAM_HOST, &devcfg, &spi);
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to add device to SPI bus");
        return ret;
    }

    // 等待 150μs，确保 PSRAM 完成自初始化
    ets_delay_us(150);
    // 初始化psram
    gpio_config(&io_conf_cs);
    gpio_set_level(PSRAM_PIN_NUM_CS, 1); // 拉高 CE#

    uint8_t reset_cmd[] = {0x66, 0x99}; // 复位命令
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = sizeof(reset_cmd) * 8;    // 命令长度
    t.tx_buffer = reset_cmd;             // 命令数据

    ret = spi_device_transmit(spi, &t);  // 发送复位命令
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to send reset command: 0x%X", ret);
        return ret;
    }
    ets_delay_us(100);


    // 进入四线模式
    uint8_t enter_quad_mode_cmd = 0x35;  // 进入四线模式命令
    memset(&t, 0, sizeof(t));
    t.length = 8;                        // 命令长度
    t.tx_buffer = &enter_quad_mode_cmd;  // 命令数据

    ret = spi_device_transmit(spi, &t);  // 发送进入四线模式命令
    if (ret != ESP_OK) {
        ESP_LOGE("BUS_SPI", "Failed to enter quad mode: 0x%X", ret);
        return ret;
    }
    ets_delay_us(100);

    ESP_LOGI("BUS_SPI", "PSRAM initialized successfully");
    return ESP_OK;
}

// 读取 PSRAM 数据
esp_err_t psram_read(uint32_t addr, uint8_t *data, size_t len) {
    gpio_set_level(PSRAM_PIN_NUM_CS, 0); // 拉高 CE#
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t tx_buffer[5];
    tx_buffer[0] = 0xEB;                 // 读命令
    tx_buffer[1] = (addr >> 16) & 0xFF;  // 地址高位
    tx_buffer[2] = (addr >> 8) & 0xFF;   // 地址中位
    tx_buffer[3] = addr & 0xFF;          // 地址低位
    tx_buffer[4] = 0x00;                 // Dummy 字节

    t.length = 5 * 8;                    // 命令 + 地址
    t.tx_buffer = tx_buffer;
    t.rxlength = len * 8;                // 读取的数据长度
    t.rx_buffer = data;
    esp_err_t ret = spi_device_transmit(spi, &t); // 发送命令并读取数据
    gpio_set_level(PSRAM_PIN_NUM_CS, 1); // 拉高 CE#
    return ret;
}


// 写入 PSRAM 数据
esp_err_t psram_write(uint32_t addr, uint8_t *data, size_t len) {
    gpio_set_level(PSRAM_PIN_NUM_CS, 0); // 拉高 CE#
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t *tx_buffer = malloc(len + 4); // 命令 + 地址 + 数据
    tx_buffer[0] = 0x38;                 // 写命令
    tx_buffer[1] = (addr >> 16) & 0xFF;  // 地址高位
    tx_buffer[2] = (addr >> 8) & 0xFF;   // 地址中位
    tx_buffer[3] = addr & 0xFF;          // 地址低位
    memcpy(&tx_buffer[4], data, len);    // 数据

    t.length = (len + 4) * 8;            // 总位数
    t.tx_buffer = tx_buffer;

    esp_err_t ret = spi_device_transmit(spi, &t); // 发送数据
    free(tx_buffer);
    gpio_set_level(PSRAM_PIN_NUM_CS, 1); // 拉高 CE#
    return ret;
}

// 读取 PSRAM ID
esp_err_t psram_read_id(uint8_t *id) {
    gpio_set_level(PSRAM_PIN_NUM_CS, 0); // 拉高 CE#
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t tx_buffer = 0x9F;            // 读取 ID 命令
    t.length = 8;                        // 命令长度
    t.tx_buffer = &tx_buffer;
    t.rxlength = 8;                      // ID 长度
    t.rx_buffer = id;

    return spi_device_transmit(spi, &t); // 发送命令并读取 ID
}

void psram_test_read(){
    uint8_t id;
    psram_read_id(&id);
    ESP_LOGI("APP", "PSRAM id 0x%02X", id);
     // 测试地址
    uint32_t addr = 0x123456;

    // 写入数据到 PSRAM
    uint8_t write_data = 0xAB; // 要写入的数据
    if (psram_write(addr, &write_data, 1) == ESP_OK) {
        ESP_LOGI("APP", "Data written to PSRAM at address 0x%06lX: 0x%02X", addr, write_data);
    } else {
        ESP_LOGE("APP", "Failed to write to PSRAM");
        return;
    }
    // 从 PSRAM 读取数据
    uint8_t read_data;
    if (psram_read(addr, &read_data, 1) == ESP_OK) {
        ESP_LOGI("APP", "Data read from PSRAM at address 0x%06lX: 0x%02X", addr, read_data);
    } else {
        ESP_LOGE("APP", "Failed to read from PSRAM");
        return;
    }

}
