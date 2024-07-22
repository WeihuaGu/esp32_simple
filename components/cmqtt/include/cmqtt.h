#ifndef cmqtt
#define cmqtt
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"

#define MQTT_URL "mqtt://192.168.9.2:1883"
#define SUBSCRIBE_TOPIC "/dev/to_esp32"
#define PUBLISH_TOPIC "/dev/from_esp32"
#define MQTT_QUEUE_LENGTH 5

void mqtt_app_start(void);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
QueueHandle_t mqtt_get_common_queue();
esp_err_t mqtt_publish(const char *data, size_t data_len);
esp_err_t mqtt_publish_bytopic(const char *topic, const char *data, size_t data_len);

#endif
