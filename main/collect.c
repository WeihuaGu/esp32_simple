#include "collect.h"
#include "common_event.h"
#include "external_bus.h"
#include "bluetick.h"
#include "display.h"
#include "bloodoxygen.h"
#include "cwifi.h"
#include "cmqtt.h"
#include "dht11.h"
#include "lora.h"
#include "a2dp_sink.h"

void external_bus_init(void)
{
     i2c_init();
     vTaskDelay(pdMS_TO_TICKS(100));
     uart_init();
     vTaskDelay(pdMS_TO_TICKS(100));
     i2s_init();
     vTaskDelay(pdMS_TO_TICKS(100));
     dac_one_shot_init();
     vTaskDelay(pdMS_TO_TICKS(100));

}
void init_communication(void)
{
    //common
    create_main_eventgroup();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    external_bus_init();
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ESP_ERROR_CHECK(nvs_flash_init());
    }


    display_init();
    init_wifi();
    mqtt_app_start();
}
void init_components(void)
{
    bluetick_set_gpio();
    text_demo();
    bt_init();
    lora_start();
}
void app_main(void)
{
     init_communication();
     init_components();
     blue_hold_on();
     bloodoxygensensor_data_reader_run();
     temsensor_data_reader_run();

}




