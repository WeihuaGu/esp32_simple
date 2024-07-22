idf_component_register(SRCS "dht11.c"
                       INCLUDE_DIRS "include"
		       REQUIRES driver
		       PRIV_REQUIRES cmqtt esp_timer driver freertos display)
