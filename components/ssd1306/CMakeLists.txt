idf_component_register(SRCS "ssd1306.c"  "ssd1306_i2c.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES external_bus driver freertos)
