idf_component_register(SRCS "bloodoxygen.c"  "i2c_api.c"  "max30102_api.c" "algorithm.c"
                       INCLUDE_DIRS "include"
		       PRIV_REQUIRES driver freertos bluetick display cmqtt
		       REQUIRES external_bus driver)
