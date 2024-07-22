# esp_simple
简明，却完整的esp32 使用esp-idf 例子。
## 简介
使用一个esp32,实现包括：
- **温度湿度测定**
- **血氧测量**
- **oled屏幕显示**
- **蓝牙音响**
- **使用mqtt发送收集的数据到 mqtt server**

## 组件协同
上面功能通过编写独立的组件实现，但组件之间会有同步互斥问题，例如要使用mqtt发送信息，必须等待wifi连接的组件连入wifi。再比如，初始化蓝牙时，会出现内存峰值占用过高，为了保证初始化正常，就需要先初始化蓝牙组件，然后再初始化wifi连接。
为此，定义common_event组件，用于全部组件之间的同步。
common_event组件的头文件定义如下：
```
#ifndef common_event
#define common_event
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define configUSE_16_BIT_TICKS 0
/* The main event*/
#define BT_INIT_OK BIT0
#define BT_INIT_ERR BIT1

#define WIFI_CONNECTED_BIT BIT2
#define WIFI_FAIL_BIT      BIT3

#define MQTT_CONNECTED_BIT BIT4

#define I2S_RINGBUF_OK BIT5


void create_main_eventgroup();
EventGroupHandle_t get_main_event_group();

#endif
```
其只定义事件组的各BIT宏，创建事件组函数create_main_eventgroup，获得事件组函数get_main_event_group。create_main_eventgroup函数由main组件调用，创建事件组，其余所有组件皆通过引入该头文件，调用get_main_event_group()获取全局组件同步事件组。
## 其它组件说明
其它组件说明，请查看components下的各组件自己的说明README.md文件。

