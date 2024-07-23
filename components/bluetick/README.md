# bluetick
bluetick只是简单的点亮一个LED灯。
## 使用GPIO口
你只需将led灯的正极与esp32的gpio 21口相连，负极接GND。

```
#define GPIO_LED_OUT 21
```
## 其它组件通过调用
- void blue_hold_on(); 点亮
- void blud_hold_off(); 关闭


