#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"

#include "bt_base_v.h"
#include "push_i2s.h"
#include "bt_msg.h"

/* device name */
#define LOCAL_DEVICE_NAME    "Gu Of Wind"

/* event for stack up */
enum {
    BT_APP_EVT_STACK_UP = 0,
};




void bt_init();
