#include "freertos/FreeRTOS.h"  
#include "freertos/task.h"  
#include "max98357.h"
  
static dac_oneshot_handle_t chan1_handle;
  
void dac_oneshot_install(){
    dac_oneshot_config_t chan1_cfg = {
        .chan_id = DAC_CHAN_1,
    };
    dac_oneshot_new_channel(&chan1_cfg, &chan1_handle);
}
  
// mode: 0 - 关闭, 1 - 立体声平均, 2 - 右声道, 3 - 左声道  
void set_sd_mode(int mode) {  
    if (mode == 0) {  
        // 关闭模式  
	// If SD is connected to ground directly (voltage is under 0.16V) then the amp is shut down
	dac_oneshot_output_voltage(chan1_handle,0);
    } else {  
	//If the voltage on SD is between 0.16V and 0.77V then the output is (Left + Right)/2, that is the stereo average.
        //If the voltage on SD is between 0.77V and 1.4V then the output is just the Right channel
        //If the voltage on SD is higher than 1.4V then the output is the Left channel.
        dac_oneshot_output_voltage(chan1_handle,25);	
    }  
}  
void re_on_max98357(){
    set_sd_mode(0);  // 关闭
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延时1秒
    set_sd_mode(1);  // 注意：这里仅为了示例，实际上可能需要修改代码来适配硬件
    vTaskDelay(1000 / portTICK_PERIOD_MS);

}
