#include "external_bus.h"

const uart_port_t uart_num = UART_NUM_2;
QueueHandle_t uart_queue;

esp_err_t i2c_init(void)
{
        i2c_config_t i2c_configuration = {
                        .mode              = I2C_MODE_MASTER, //I2C COMO MASTER
                        .sda_io_num        = SDA_PIN,
                        .sda_pullup_en     = 1,
                        .scl_io_num        = SCL_PIN,
                        .scl_pullup_en     = 1,
                        .master.clk_speed  = 200000           //SCL CLOCK SPEED 200KHZ
        };
        i2c_param_config(I2C_MASTER_PORT, &i2c_configuration);
        esp_err_t err = i2c_driver_install(I2C_MASTER_PORT, i2c_configuration.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
        return err;
}
esp_err_t uart_init(void)
{
	uart_config_t uart_config = {
    			.baud_rate = 9600,
    			.data_bits = UART_DATA_8_BITS,
    			.parity = UART_PARITY_DISABLE,
    			.stop_bits = UART_STOP_BITS_1,
    			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        };
	uart_param_config(uart_num, &uart_config);
	uart_set_pin(uart_num, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	const int uart_rv_buffer_size = (UART_RV_BUFFER_SIZE);
	// We won't use a buffer for sending data.
	esp_err_t err = uart_driver_install(uart_num, uart_rv_buffer_size, uart_rv_buffer_size, 20, &uart_queue, 0);
        return err;
}
int uart_send(const char *text){
        const int txBytes  = uart_write_bytes(uart_num, text, strlen(text));
	return txBytes;
}
int uart_read(char* data){
    	int length = 0;
    	ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
    	const int rxBytes = uart_read_bytes(uart_num, data, length, 1000 / portTICK_PERIOD_MS);
	return rxBytes;
}
esp_err_t uart_flush_data(){
	return uart_flush(uart_num);
}
void i2s_init(void){
	do_i2s_driver_install();
	i2s_task_start_up();
}
void dac_one_shot_init(){
	dac_oneshot_install();
	set_sd_mode(1);
}
