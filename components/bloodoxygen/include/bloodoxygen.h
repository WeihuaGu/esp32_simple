#ifndef MAIN_H
#define MAIN_H

#include "esp_err.h"
#include "max30102_api.h"


void bloodoxygensensor_data_reader();
void bloodoxygensensor_data_reader_run();
void fill_buffers_data();

#define BUFFER_SIZE 128

#endif
