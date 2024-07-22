#ifndef __BT_MSG_H__
#define __BT_MSG_H__

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "bt_base_v.h"


/* signal for `bt_app_work_dispatch` */
#define BT_APP_SIG_WORK_DISPATCH    (0x01)

typedef struct {
    uint16_t       sig;      /*!< signal to bt_app_task */
    uint16_t       event;    /*!< message event id */
    bt_app_cb_t    cb;       /*!< context switch callback */
    void           *param;   /*!< parameter area needs to be last */
} bt_app_msg_t;

bool bt_app_send_msg(bt_app_msg_t *msg);
bool bt_app_work_dispatch(bt_app_cb_t p_cback, uint16_t event, void *p_params, int param_len, bt_app_copy_cb_t p_copy_cback);
void bt_msg_task_start_up(void);
void bt_msg_task_shut_down(void);




#endif
