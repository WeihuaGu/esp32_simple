#include <driver/gpio.h>
#include "bluetick.h"

void bluetick_set_gpio(){
    gpio_config_t my_io_config = {
        .pin_bit_mask = 1 << GPIO_LED_OUT,
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&my_io_config);
}

void blue_hold_on() {
    gpio_set_level(GPIO_LED_OUT, 0);
}
void blue_hold_off() {
    gpio_set_level(GPIO_LED_OUT, 1);
    
}
