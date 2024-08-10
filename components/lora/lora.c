#include "lora.h"
#include "cmqtt.h"
SemaphoreHandle_t sem_uart_tx;
static QueueHandle_t lora_tx_queue;

void changeModel(){



}
QueueHandle_t lora_get_tx_queue(){
   return lora_tx_queue;
}
void lora_test_loop(void *pvParameters){
    for (;;){
	  char* test_str = "lora rx test hold";
	  xQueueSend(lora_tx_queue,&test_str,0);
          vTaskDelay(pdMS_TO_TICKS(50000));
    }

}
void lora_rx_task(void *arg)
{
    char * lora_r_data = (char *) malloc(UART_RV_BUFFER_SIZE + 1);
    char * send_mqtt_str;
    while (1) {
	   const int read_num = uart_read(lora_r_data);
	   if(read_num>0){
	       asprintf(&send_mqtt_str, "{\"intent\":\"lora_rx\",\"content\":%s}", lora_r_data);
               xQueueSend(mqtt_get_common_queue(),&send_mqtt_str,0);

	       uart_flush_data();
	   }
           vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
void task_tx(void *arg){
    char *receivedJson;
    while(1){
        if(xQueueReceive(lora_tx_queue, &receivedJson, portMAX_DELAY) == pdTRUE)
        {
             if(receivedJson!=NULL){
	  	//xSemaphoreTake(sem_uart_tx,portMAX_DELAY);
	  	uart_send(receivedJson);
          	//xSemaphoreGive(sem_uart_tx);
             }else{
             }
        }

    }

}
void lora_start(){
    sem_uart_tx = xSemaphoreCreateBinary();
    lora_tx_queue = xQueueCreate(LORA_QUEUE_LENGTH,sizeof(char *));
    xSemaphoreGive(sem_uart_tx);
    //xTaskCreate(lora_test_loop, "lora send test", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    //xTaskCreate(task_tx, "lora send to other", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(lora_rx_task, "lora rx", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

