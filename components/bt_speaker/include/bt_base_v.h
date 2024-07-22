#ifndef __BT_BASE_V_H__
#define __BT_BASE_V_H__

#define A2DPTAG "A2DP_SINK"


typedef struct {
    bool is_connected;  // 指示是否已建立A2DP连接
    uint8_t audio_codec; // 使用的音频编解码器（如果需要）
} a2dp_sink_state_t;


typedef void (* bt_app_cb_t) (uint16_t event, void *param);
typedef void (* bt_app_copy_cb_t) (void *p_dest, void *p_src, int len);



#endif
