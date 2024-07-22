#ifndef MAIN_SSD1306_H_
#define MAIN_SSD1306_H_
#include "ssd1306_t.h"
void ssd1306_init(SSD1306_t * dev, int width, int height);
int ssd1306_get_width(SSD1306_t * dev);
int ssd1306_get_height(SSD1306_t * dev);
int ssd1306_get_pages(SSD1306_t * dev);
void ssd1306_show_buffer(SSD1306_t * dev);
void ssd1306_set_buffer(SSD1306_t * dev, uint8_t * buffer);
void ssd1306_get_buffer(SSD1306_t * dev, uint8_t * buffer);
void ssd1306_display_image(SSD1306_t * dev, int page, int seg, uint8_t * images, int width);
void ssd1306_display_text(SSD1306_t * dev, int page, char * text, int text_len, bool invert);
void ssd1306_display_text_x3(SSD1306_t * dev, int page, char * text, int text_len, bool invert);
void ssd1306_clear_screen(SSD1306_t * dev, bool invert);
void ssd1306_clear_line(SSD1306_t * dev, int page, bool invert);
void ssd1306_contrast(SSD1306_t * dev, int contrast);
void ssd1306_software_scroll(SSD1306_t * dev, int start, int end);
void ssd1306_scroll_text(SSD1306_t * dev, char * text, int text_len, bool invert);
void ssd1306_scroll_clear(SSD1306_t * dev);
void ssd1306_hardware_scroll(SSD1306_t * dev, ssd1306_scroll_type_t scroll);
void ssd1306_wrap_arround(SSD1306_t * dev, ssd1306_scroll_type_t scroll, int start, int end, int8_t delay);
void ssd1306_bitmaps(SSD1306_t * dev, int xpos, int ypos, uint8_t * bitmap, int width, int height, bool invert);
void _ssd1306_pixel(SSD1306_t * dev, int xpos, int ypos, bool invert);
void _ssd1306_line(SSD1306_t * dev, int x1, int y1, int x2, int y2,  bool invert);
void ssd1306_invert(uint8_t *buf, size_t blen);
void ssd1306_flip(uint8_t *buf, size_t blen);
uint8_t ssd1306_copy_bit(uint8_t src, int srcBits, uint8_t dst, int dstBits);
uint8_t ssd1306_rotate_byte(uint8_t ch1);
void ssd1306_fadeout(SSD1306_t * dev);
void ssd1306_dump(SSD1306_t dev);
void ssd1306_dump_page(SSD1306_t * dev, int page, int seg);

#endif /* MAIN_SSD1306_H_ */

