#ifndef __BT_BASE_V_H__
#define __BT_BASE_V_H__
#include <stdint.h>
#define A2DPTAG "A2DP_SINK"


typedef void (* bt_app_cb_t) (uint16_t event, void *param);
typedef void (* bt_app_copy_cb_t) (void *p_dest, void *p_src, int len);



#endif
