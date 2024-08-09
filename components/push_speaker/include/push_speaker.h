#include "bus_i2s.h"

#define PUSH_SPEAKER_TAG "PUSH_SPEAKER_TAG"

size_t write_speaker_ringbuf(const uint8_t *data, size_t size);
void play_silence(size_t seconds);

