idf_component_register(SRCS "cmqtt.c"
                       INCLUDE_DIRS "include"
		       PRIV_REQUIRES driver freertos
		       REQUIRES mqtt esp_event freertos common_event cwifi)
