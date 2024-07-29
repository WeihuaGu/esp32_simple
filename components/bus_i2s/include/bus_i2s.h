#ifndef bus_i2s_v
#define bus_i2s_v
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/ringbuf.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"


#define I2S_CORE_TAG "i2s core tag"
#define I2S_BCLK_NUM GPIO_NUM_23
#define I2S_DOUT_NUM GPIO_NUM_26
#define I2S_LRCK_NUM GPIO_NUM_18
#define RINGBUF_HIGHEST_WATER_LEVEL    (25 * 1024)
#define RINGBUF_PREFETCH_WATER_LEVEL (3 * 1024)  /*waiting to be retrieved limited*/

enum {
    RINGBUFFER_MODE_PROCESSING,    /* ringbuffer is buffering incoming audio data, I2S is working */
    RINGBUFFER_MODE_PREFETCHING,   /* ringbuffer is buffering incoming audio data, I2S is waiting */
    RINGBUFFER_MODE_DROPPING       /* ringbuffer is not buffering (dropping) incoming audio data, I2S is working */
};
extern uint16_t ringbuffer_mode;
extern SemaphoreHandle_t s_i2s_write_semaphore;
void do_i2s_driver_install(void);
RingbufHandle_t get_i2s_ringbuf();
void i2s_task_start_up();
void i2s_task_shut_down();

#endif
