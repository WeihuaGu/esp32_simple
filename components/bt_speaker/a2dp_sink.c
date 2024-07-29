#include "a2dp_sink.h"
#include "common_event.h"
#include "bluetick.h"
a2dp_sink_state_t a2dp_sink_state;

static void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    uint8_t *bda = NULL;

    switch (event) {
    /* when authentication completed, this event comes */
    case ESP_BT_GAP_AUTH_CMPL_EVT: {
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(A2DPTAG, "authentication success: %s", param->auth_cmpl.device_name);
            ESP_LOG_BUFFER_HEX(A2DPTAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
        } else {
            ESP_LOGE(A2DPTAG, "authentication failed, status: %d", param->auth_cmpl.stat);
        }
        ESP_LOGI(A2DPTAG, "link key type of current link is: %d", param->auth_cmpl.lk_type);
        break;
    }
    case ESP_BT_GAP_ENC_CHG_EVT: {
        char *str_enc[3] = {"OFF", "E0", "AES"};
        bda = (uint8_t *)param->enc_chg.bda;
        ESP_LOGI(A2DPTAG, "Encryption mode to [%02x:%02x:%02x:%02x:%02x:%02x] changed to %s",
                 bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], str_enc[param->enc_chg.enc_mode]);
        break;
    }
    /* when GAP mode changed, this event comes */
    case ESP_BT_GAP_MODE_CHG_EVT:
        ESP_LOGI(A2DPTAG, "ESP_BT_GAP_MODE_CHG_EVT mode: %d", param->mode_chg.mode);
        break;
    /* when ACL connection completed, this event comes */
    case ESP_BT_GAP_ACL_CONN_CMPL_STAT_EVT:
        bda = (uint8_t *)param->acl_conn_cmpl_stat.bda;
        ESP_LOGI(A2DPTAG, "ESP_BT_GAP_ACL_CONN_CMPL_STAT_EVT Connected to [%02x:%02x:%02x:%02x:%02x:%02x], status: 0x%x",bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], param->acl_conn_cmpl_stat.stat);
	//blue_hold_on();
        break;
    /* when ACL disconnection completed, this event comes */
    case ESP_BT_GAP_ACL_DISCONN_CMPL_STAT_EVT:
        bda = (uint8_t *)param->acl_disconn_cmpl_stat.bda;
        ESP_LOGI(A2DPTAG, "ESP_BT_GAP_ACL_DISC_CMPL_STAT_EVT Disconnected from [%02x:%02x:%02x:%02x:%02x:%02x], reason: 0x%x",
                 bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], param->acl_disconn_cmpl_stat.reason);
	//esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
	//blue_hold_off();

        break;
    case ESP_BT_GAP_GET_DEV_NAME_CMPL_EVT:
	if (param->get_dev_name_cmpl.status == ESP_BT_STATUS_SUCCESS) {
                //ESP_LOGI(A2DPTAG, "get bt device name");
	} 
        break;
    case ESP_BT_GAP_CONFIG_EIR_DATA_EVT:
	break;
    /* others */
    default: {
        ESP_LOGI(A2DPTAG, "Unhandled GAP event: %d", event);
        break;
    }
    }
}


void a2dp_sink_event_callback(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param)  
{  
    esp_a2d_cb_param_t *a2d = NULL;
    switch (event) {  
        case ESP_A2D_CONNECTION_STATE_EVT: {  
            esp_a2d_connection_state_t state = param->conn_stat.state;  
            if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {  
                ESP_LOGI("A2DP_SINK", "A2DP audio stream connected");  
            } else if (state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) {  
                ESP_LOGI("A2DP_SINK", "A2DP audio stream disconnected");  
            }  
            break;  
        }  
        case ESP_A2D_AUDIO_STATE_EVT: {  
            esp_a2d_audio_state_t audio_state = param->audio_stat.state;  
            if (audio_state == ESP_A2D_AUDIO_STATE_STARTED) {  
                ESP_LOGI("A2DP_SINK", "A2DP audio stream started");  
            } else if (audio_state == ESP_A2D_AUDIO_STATE_STOPPED) {  
                ESP_LOGI("A2DP_SINK", "A2DP audio stream stopped");  
            }  
            break;  
        }  
        case ESP_A2D_AUDIO_CFG_EVT: {  
            // 在这里处理音频配置更改事件  
            ESP_LOGI("A2DP_SINK", "A2DP audio configuration changed");  
            break;  
        }  
        case ESP_A2D_PROF_STATE_EVT: {  
	    a2d = (esp_a2d_cb_param_t *)(param);
	    if (ESP_A2D_INIT_SUCCESS == a2d->a2d_prof_stat.init_state) {
                ESP_LOGI(A2DPTAG, "A2DP PROF STATE: Init Complete");
            } else {
                ESP_LOGI(A2DPTAG, "A2DP PROF STATE: Deinit Complete");
            }
            break;  
        }  
        // 其他事件处理...  
        default:  
            ESP_LOGI("A2DP_SINK", "Unhandled A2DP event: %d", event);  
            break;  
    }  
}  

void a2dp_sink_data_event_callback(const uint8_t *data, uint32_t len)
{
    write_ringbuf(data, len);
}

// 初始化A2DP Sink
static void bt_a2d_init(){
    esp_err_t ret;
        esp_bt_gap_set_device_name(LOCAL_DEVICE_NAME);
        esp_bt_gap_register_callback(bt_app_gap_cb);

        esp_a2d_register_callback(a2dp_sink_event_callback);
        ret = esp_a2d_sink_init();
        if (ret != ESP_OK) {
             ESP_LOGE(A2DPTAG, "a2d sink init failed: %s", esp_err_to_name(ret));
        }
        esp_a2d_sink_register_data_callback(a2dp_sink_data_event_callback);

        esp_bt_gap_get_device_name();
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

}
void bt_init_task(void *arg){
    xEventGroupWaitBits(get_main_event_group(),
            I2S_RINGBUF_OK,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    esp_err_t ret;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    // 初始化蓝牙控制器
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(A2DPTAG, "esp_bt_controller_init failed: %s", esp_err_to_name(ret));
        xEventGroupSetBits(get_main_event_group(), BT_INIT_ERR);
    }

    // 启用经典蓝牙模式  
    ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);  
    if (ret != ESP_OK) {  
        xEventGroupSetBits(get_main_event_group(), BT_INIT_ERR);
        ESP_LOGE(A2DPTAG, "esp_bt_controller_enable failed: %s", esp_err_to_name(ret));  
    }  
  
    // 初始化bluedroid  
    esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
    ret = esp_bluedroid_init_with_cfg(&bluedroid_cfg);  
    if (ret != ESP_OK) {  
        xEventGroupSetBits(get_main_event_group(), BT_INIT_ERR);
        ESP_LOGE(A2DPTAG, "esp_bluedroid_init_with_cfg failed: %s", esp_err_to_name(ret));  
    }  
  
    // 启用bluedroid
    ret = esp_bluedroid_enable();
    if (ret != ESP_OK) {
        xEventGroupSetBits(get_main_event_group(), BT_INIT_ERR);
        ESP_LOGE(A2DPTAG, "esp_bluedroid_enable failed: %s", esp_err_to_name(ret));
    }

    ret = esp_bredr_tx_power_set(ESP_PWR_LVL_P6,ESP_PWR_LVL_P9);
    if (ret != ESP_OK) {
        ESP_LOGE(A2DPTAG, "Set BR/EDR TX power failed: %s", esp_err_to_name(ret));
    }

    //pin
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code;
    pin_code[0] = '2';
    pin_code[1] = '2';
    pin_code[2] = '3';
    pin_code[3] = '4';
    esp_bt_gap_set_pin(pin_type, 4, pin_code);

    bt_a2d_init();

    xEventGroupSetBits(get_main_event_group(), BT_INIT_OK);
    vTaskDelete(NULL);
}

void bt_init(){
    xTaskCreate(bt_init_task, "bt init", configMINIMAL_STACK_SIZE*2, NULL, 1, NULL);
    //bt_msg_task_start_up();
}

