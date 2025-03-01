#include "external_bus.h"

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
void i2s_init(void){
	do_i2s_driver_install();
	i2s_task_start_up();
}
void dac_one_shot_init(){
	dac_oneshot_install();
	set_sd_mode(1);
}
