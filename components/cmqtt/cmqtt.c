#include "cmqtt.h"
#include "cwifi.h"
#include "common_event.h"
static char *expected_data = NULL;
static char *actual_data = NULL;
static size_t expected_size = 0;
static size_t expected_published = 0;
static size_t actual_published = 0;
static int qos_test = 0;

static const char *TAG = "mqtt";
static esp_mqtt_client_handle_t mqtt_client = NULL;
static TaskHandle_t mqtt_task_handle;
static QueueHandle_t common_queue;
static int wificonnected = 1;

QueueHandle_t mqtt_get_common_queue(){
     return common_queue;
}
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    esp_mqtt_client_handle_t client = event->client;
    static int msg_id = 0;
    static int actual_len = 0;
    // your_context_t *context = event->context;
    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        xEventGroupSetBits(get_main_event_group(), MQTT_CONNECTED_BIT);
        msg_id = esp_mqtt_client_subscribe(client, SUBSCRIBE_TOPIC, qos_test);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        printf("ID=%d, total_len=%d, data_len=%d, current_data_offset=%d\n", event->msg_id, event->total_data_len, event->data_len, event->current_data_offset);
        if (event->topic) {
            actual_len = event->data_len;
            msg_id = event->msg_id;
        } else {
            actual_len += event->data_len;
            // check consisency with msg_id across multiple data events for single msg
            if (msg_id != event->msg_id) {
                ESP_LOGI(TAG, "Wrong msg_id in chunked message %d != %d", msg_id, event->msg_id);
                abort();
            }
        }
        memcpy(actual_data + event->current_data_offset, event->data, event->data_len);
        if (actual_len == event->total_data_len) {
            if (0 == memcmp(actual_data, expected_data, expected_size)) {
                printf("OK!");
                memset(actual_data, 0, expected_size);
                actual_published ++;
                if (actual_published == expected_published) {
                    printf("Correct pattern received exactly x times\n");
                    ESP_LOGI(TAG, "Test finished correctly!");
                }
            } else {
                printf("FAILED!");
                abort();
            }
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}
void mqtt_init(){
    EventBits_t bits = xEventGroupWaitBits(get_main_event_group(),
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        wificonnected = 0;
        ESP_LOGI(TAG, "connected to ap SSID and start mqtt");
        const esp_mqtt_client_config_t mqtt_cfg = {
	   .broker.address.uri = MQTT_URL,
        };
        mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
        esp_mqtt_client_register_event(mqtt_client,ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);
        esp_mqtt_client_start(mqtt_client);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID and mqtt make flag");
        wificonnected = 1;

    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        wificonnected = 1;
    }
    vTaskDelete(NULL);

}
void task_common(void *arg){
    xEventGroupWaitBits(get_main_event_group(),
            MQTT_CONNECTED_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);
    char *receivedJson;
    while(1){
        if(xQueueReceive(common_queue, &receivedJson, portMAX_DELAY) == pdTRUE)
        {
        // 数据已成功从队列中取出
        // 发送到mqtt server
        // 释放队列项占用的内存（如果是由队列管理函数分配的）
	     if(receivedJson!=NULL){
	       //printf("%s\n", receivedJson); 
	       mqtt_publish((const char *)receivedJson,strlen(receivedJson));
               vPortFree(receivedJson);
	     }else{
               ESP_LOGE(TAG, "get a null receive from common_queue");
	     }
        }

    }

}

void mqtt_app_start(void){
	common_queue = xQueueCreate(MQTT_QUEUE_LENGTH,sizeof(char *));
	xTaskCreate(mqtt_init, "mqtt_start", configMINIMAL_STACK_SIZE*1.5, NULL, 1, &mqtt_task_handle);
	xTaskCreate(task_common, "mqtt_common_queue", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

}

esp_err_t mqtt_publish_base(const char *topic,const char *data, size_t data_len)
{
    if(wificonnected == 0){
	esp_mqtt_client_publish(mqtt_client, topic, data, data_len, 0, 0);
	return ESP_OK;
    }else{
	return ESP_FAIL;
    }
}
esp_err_t mqtt_publish(const char *data, size_t data_len)
{
        return mqtt_publish_base(PUBLISH_TOPIC, data, data_len);
}
esp_err_t mqtt_publish_bytopic(const char *topic, const char *data, size_t data_len)
{
        return mqtt_publish_base(topic, data, data_len);
}
