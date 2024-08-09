#include <stdint.h>  
#include <stdbool.h>
#include "push_speaker.h"

#define SAMPLE_BYTES 4
#define SAMPLE_RATE 44100
static const uint8_t silence_frame[SAMPLE_BYTES] = {0, 0, 0, 0}; // 32位全零的静音样本

size_t write_speaker_ringbuf(const uint8_t *data, size_t size){
    size_t item_size = 0;
    BaseType_t done = pdFALSE;

    if (ringbuffer_mode == RINGBUFFER_MODE_DROPPING) {
        ESP_LOGW(PUSH_SPEAKER_TAG, "ringbuffer is full, drop this packet!");
        vRingbufferGetInfo(get_i2s_ringbuf(), NULL, NULL, NULL, NULL, &item_size);
        if (item_size <= RINGBUF_PREFETCH_WATER_LEVEL) {
            //ESP_LOGI(PUSH_SPEAKER_TAG, "ringbuffer data decreased! mode changed: RINGBUFFER_MODE_PROCESSING");
            ringbuffer_mode = RINGBUFFER_MODE_PROCESSING;
        }
        return 0;
    }

    done = xRingbufferSend(get_i2s_ringbuf(), (void *)data, size, (TickType_t)20);

    if (!done) {
        //ESP_LOGW(PUSH_SPEAKER_TAG, "ringbuffer overflowed, ready to decrease data! mode changed: RINGBUFFER_MODE_DROPPING");
        ringbuffer_mode = RINGBUFFER_MODE_DROPPING;
    }

    if (ringbuffer_mode == RINGBUFFER_MODE_PREFETCHING) {
        vRingbufferGetInfo(get_i2s_ringbuf(), NULL, NULL, NULL, NULL, &item_size);
        if (item_size >= RINGBUF_PREFETCH_WATER_LEVEL) {
            //ESP_LOGI(PUSH_SPEAKER_TAG, "ringbuffer data increased! mode changed: RINGBUFFER_MODE_PROCESSING");
            ringbuffer_mode = RINGBUFFER_MODE_PROCESSING;
            if (pdFALSE == xSemaphoreGive(s_i2s_write_semaphore)) {
                ESP_LOGE(PUSH_SPEAKER_TAG, "semphore give failed");
            }
        }
    }

    return done ? size : 0;

}

void play_silence(size_t seconds){
    size_t frames_to_write = seconds * SAMPLE_RATE;
    for (size_t i = 0; i < frames_to_write; i++) {
        write_speaker_ringbuf(&silence_frame, SAMPLE_BYTES);

    }


}
