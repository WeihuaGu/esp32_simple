#include "ssd1306_t.h"
#define I2CAddress 0x3C
void i2c_init_ssd1306(SSD1306_t * dev, int width, int height);
void i2c_display_image(SSD1306_t * dev, int page, int seg, uint8_t * images, int width);
void i2c_contrast(SSD1306_t * dev, int contrast);
void i2c_hardware_scroll(SSD1306_t * dev, ssd1306_scroll_type_t scroll);
