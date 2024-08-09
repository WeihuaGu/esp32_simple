#include "push_i2s.h"
size_t write_ringbuf(const uint8_t *data, size_t size){
    return write_speaker_ringbuf(data,size);
}
