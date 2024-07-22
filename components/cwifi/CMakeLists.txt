idf_component_register(SRCS "cwifi.c"
                       INCLUDE_DIRS "include"
		       PRIV_REQUIRES driver freertos
		       REQUIRES esp_wifi nvs_flash lwip freertos common_event)
