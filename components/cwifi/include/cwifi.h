#ifndef cwifi
#define cwifi
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define MAXIMUM_RETRY  10
#define SSID "you wifi ssid"
#define PASSWD "you wifi passpd"


void init_wifi(void);
void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_sta(void *pvParameters);

#endif
