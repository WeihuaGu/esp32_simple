#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c_api.h"
#include "max30102_api.h"
#include "algorithm.h"
#include "bloodoxygen.h"
#include "bluetick.h"
#include "cmqtt.h"
#include "display.h"

#define DELAY_AMOSTRAGEM 40
int32_t red_data = 0;
int32_t ir_data = 0;
int32_t red_data_buffer[BUFFER_SIZE];
int32_t ir_data_buffer[BUFFER_SIZE];
double auto_correlationated_data[BUFFER_SIZE];
static TaskHandle_t spo2_task_handle;
max_config max30102_configuration = {

                .INT_EN_1.A_FULL_EN         = 1,
                .INT_EN_1.PPG_RDY_EN        = 1,
                .INT_EN_1.ALC_OVF_EN        = 0,
                .INT_EN_1.PROX_INT_EN       = 0,

                .INT_EN_2.DIE_TEMP_RDY_EN   = 0,

                .FIFO_WRITE_PTR.FIFO_WR_PTR = 0,

                .OVEF_COUNTER.OVF_COUNTER   = 0,

                .FIFO_READ_PTR.FIFO_RD_PTR  = 0,

                .FIFO_CONF.SMP_AVE          = 0b010,  //média de 4 valores
                .FIFO_CONF.FIFO_ROLLOVER_EN = 1,      //fifo rollover enable
                .FIFO_CONF.FIFO_A_FULL      = 0,      //0

                .MODE_CONF.SHDN             = 0,
                .MODE_CONF.RESET            = 0,
                .MODE_CONF.MODE             = 0b011,  //SPO2 mode

                .SPO2_CONF.SPO2_ADC_RGE     = 0b01,   //16384 nA(Escala do DAC)
                .SPO2_CONF.SPO2_SR          = 0b001,  //200 samples per second
                .SPO2_CONF.LED_PW           = 0b10,   //pulso de 215 uS do led.

                .LED1_PULSE_AMP.LED1_PA     = 0x24,   //CORRENTE DO LED1 25.4mA
                .LED2_PULSE_AMP.LED2_PA     = 0x24,   //CORRENTE DO LED2 25.4mA

                .PROX_LED_PULS_AMP.PILOT_PA = 0X7F,

                .MULTI_LED_CONTROL1.SLOT2   = 0,      //Desabilitado
                .MULTI_LED_CONTROL1.SLOT1   = 0,      //Desabilitado

                .MULTI_LED_CONTROL2.SLOT4   = 0,      //Desabilitado
                .MULTI_LED_CONTROL2.SLOT3   = 0,      //Desabilitado
};
void bloodoxygensensor_data_reader()
{
	max30102_init(&max30102_configuration);
	init_time_array();
	uint64_t ir_mean;
	uint64_t red_mean;
	float temperature;
	double r0_autocorrelation;
        double pearson_correlation;
        int heart_rate;
        char *mqttdata;
        double spo2;


	for(;;){
		fill_buffers_data();
		temperature = get_max30102_temp();
		remove_dc_part(ir_data_buffer, red_data_buffer, &ir_mean, &red_mean);
		remove_trend_line(ir_data_buffer);
		remove_trend_line(red_data_buffer);
		pearson_correlation = correlation_datay_datax(red_data_buffer, ir_data_buffer);
		heart_rate = calculate_heart_rate(ir_data_buffer, &r0_autocorrelation, auto_correlationated_data);

		if(pearson_correlation >= 0.7){
			spo2 = spo2_measurement(ir_data_buffer, red_data_buffer, ir_mean, red_mean);
			dis_bloodoxygen(spo2,heart_rate);
			asprintf(&mqttdata, "{\"intent\":\"spo2\",\"spo2\":%.2f, \"heart_rate\":%d}", spo2, heart_rate);
			xQueueSend(mqtt_get_common_queue(),&mqttdata,0);

		}

	}
}
void bloodoxygensensor_data_reader_run(){
	 xTaskCreate(bloodoxygensensor_data_reader, "spo2_task", configMINIMAL_STACK_SIZE*2, NULL, 2, &spo2_task_handle);
}

void fill_buffers_data()
{
	for(int i = 0; i < BUFFER_SIZE; i++){
		read_max30102_fifo(&red_data, &ir_data);
		ir_data_buffer[i] = ir_data;
		red_data_buffer[i] = red_data;
		//printf("%d\n", red_data);
		ir_data = 0;
		red_data = 0;
		vTaskDelay(pdMS_TO_TICKS(DELAY_AMOSTRAGEM));
	}
}
