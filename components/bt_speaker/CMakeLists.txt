idf_component_register(SRCS "a2dp_sink.c" "push_i2s.c" "bt_msg.c"
                       INCLUDE_DIRS "include"
	               REQUIRES bus_i2s common_event bt nvs_flash esp_ringbuf driver freertos)
