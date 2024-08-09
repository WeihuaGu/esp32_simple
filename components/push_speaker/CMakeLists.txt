idf_component_register(SRCS "push_speaker.c"
                       INCLUDE_DIRS "include"
	               REQUIRES bus_i2s esp_ringbuf driver freertos)
