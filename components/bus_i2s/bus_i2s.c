#include "bus_i2s.h"
#include "common_event.h"
static i2s_chan_handle_t tx_handle;

static RingbufHandle_t s_ringbuf_i2s = NULL;
uint16_t ringbuffer_mode = RINGBUFFER_MODE_PROCESSING;
int bus_isdown = 1;
static TaskHandle_t s_i2s_task_handle = NULL;
SemaphoreHandle_t s_i2s_write_semaphore = NULL;


RingbufHandle_t get_i2s_ringbuf(){
    return s_ringbuf_i2s;
}


void do_i2s_driver_install(void)
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    i2s_new_channel(&chan_cfg, &tx_handle, NULL);

    i2s_std_config_t std_cfg = {
      .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(44100),
      .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_MONO),
      .gpio_cfg = {
        .mclk = I2S_GPIO_UNUSED,
        .bclk = I2S_BCLK_NUM,
        .ws = I2S_LRCK_NUM,
        .dout = I2S_DOUT_NUM,
        .din = I2S_GPIO_UNUSED,
        .invert_flags = {
            .mclk_inv = false,
            .bclk_inv = false,
            .ws_inv = false,
        },
      },
    };
    i2s_channel_init_std_mode(tx_handle, &std_cfg);
    i2s_channel_enable(tx_handle);


}
void re_enable_i2s_channel(){
    i2s_channel_disable(tx_handle);
    i2s_channel_enable(tx_handle);
}

static void i2s_task(void *arg)
{
    uint8_t *data = NULL;
    size_t item_size = 0;
    const size_t item_size_upto = 240 * 6 * 2;
    size_t bytes_written = 0;

    for (;;) {
        if (pdTRUE == xSemaphoreTake(s_i2s_write_semaphore, portMAX_DELAY)) {
            for (;;) {
                item_size = 0;
                data = (uint8_t *)xRingbufferReceiveUpTo(s_ringbuf_i2s, &item_size, (TickType_t)pdMS_TO_TICKS(20), item_size_upto);
                if (item_size == 0) {
                    //ESP_LOGI(I2S_CORE_TAG, "ringbuffer underflowed! mode changed: RINGBUFFER_MODE_PREFETCHING");
                    ringbuffer_mode = RINGBUFFER_MODE_PREFETCHING;
                    break;
                }
		i2s_channel_write(tx_handle, data, item_size, &bytes_written, portMAX_DELAY);
                vRingbufferReturnItem(s_ringbuf_i2s, (void *)data);
            }
        }
    }
}

void i2s_task_start_up(void)
{
    ESP_LOGI(I2S_CORE_TAG, "ringbuffer data empty! first mode: RINGBUFFER_MODE_PREFETCHING");
    ringbuffer_mode = RINGBUFFER_MODE_PREFETCHING;
    s_i2s_write_semaphore = xSemaphoreCreateBinary();
    //建立字节环形队列
    s_ringbuf_i2s = xRingbufferCreate(RINGBUF_HIGHEST_WATER_LEVEL, RINGBUF_TYPE_BYTEBUF);
    if(s_ringbuf_i2s!=NULL){
        ESP_LOGI(I2S_CORE_TAG, "ringbuffer list create ok");
        xEventGroupSetBits(get_main_event_group(), I2S_RINGBUF_OK);
        xTaskCreate(i2s_task, "I2STask",configMINIMAL_STACK_SIZE*2, NULL, 4, &s_i2s_task_handle );
    }

    bus_isdown = 0;
}

void i2s_task_shut_down(void)
{
    bus_isdown = 1;
    if (s_i2s_task_handle) {
        vTaskDelete(s_i2s_task_handle);
        s_i2s_task_handle = NULL;
    }
    if (s_ringbuf_i2s) {
        vRingbufferDelete(s_ringbuf_i2s);
        s_ringbuf_i2s = NULL;
    }
    if (s_i2s_write_semaphore) {
        vSemaphoreDelete(s_i2s_write_semaphore);
        s_i2s_write_semaphore = NULL;
    }
}
