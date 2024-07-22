#include "common_event.h"

static EventGroupHandle_t s_main_event_group;

EventGroupHandle_t get_main_event_group(){
    return s_main_event_group;
}

void create_main_eventgroup(){
    s_main_event_group = xEventGroupCreate();
}
