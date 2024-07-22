#include <string.h>
#include "esp_system.h"
#include "ssd1306.h"
#include "display.h"
static SSD1306_t dev;
static char * time_str;

void display_init(){
	ssd1306_init(&dev, 128, 64);
}

void get_time(){

}
void text_demo(){
	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
	ssd1306_display_text_x3(&dev, 0, "Hello", 5, false);
}
void dis_temperature(int temperature,int humidity){
	char *tem;
	char *hum;
	asprintf(&tem, "T: %u", temperature);
	asprintf(&hum, "H: %u", humidity);
        get_time();
	ssd1306_clear_screen(&dev, false);
	ssd1306_display_text_x3(&dev, 0, tem, strlen(tem), false);
	ssd1306_display_text_x3(&dev, 4, hum, strlen(hum), false);
	//ssd1306_display_text(&dev, 7, time_str, strlen(time_str), false);

}
void dis_bloodoxygen(double spo2,int heart){
	char *spo2str;
	char *heartstr;
	asprintf(&spo2str, "%.2f", spo2);
	asprintf(&heartstr, "H %d", heart);
	ssd1306_clear_screen(&dev, false);
	ssd1306_display_text_x3(&dev, 0, spo2str, strlen(spo2str), false);
	ssd1306_display_text_x3(&dev, 4, heartstr, strlen(heartstr), false);

}
