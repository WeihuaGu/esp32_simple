#ifndef lora_t
#define lora_t
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "external_bus.h"

#define MQTT_LORA_TOPIC "/dev/from_esp32/lora_rx"
#define LORA_QUEUE_LENGTH 100


void changeModel();
void lora_start();
QueueHandle_t lora_get_tx_queue();

#endif
