idf_component_register(SRCS "lora.c"
                       INCLUDE_DIRS "include"
	               REQUIRES external_bus cmqtt driver freertos)
