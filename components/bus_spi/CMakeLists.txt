idf_component_register(SRCS "bus_spi.c" "flash_base.c" "psram_base.c"
                       INCLUDE_DIRS "include"
                       REQUIRES driver freertos)

