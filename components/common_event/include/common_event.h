#ifndef common_event
#define common_event
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define configUSE_16_BIT_TICKS 0
/* The main event*/
#define BT_INIT_OK BIT0
#define BT_INIT_ERR BIT1

#define WIFI_CONNECTED_BIT BIT2
#define WIFI_FAIL_BIT      BIT3

#define MQTT_CONNECTED_BIT BIT4

#define I2S_RINGBUF_OK BIT5


void create_main_eventgroup();
EventGroupHandle_t get_main_event_group();

#endif

