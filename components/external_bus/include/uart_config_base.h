#ifndef uart_config_base
#define uart_config_base

#include "driver/uart.h"

#define UART_BAUD_RATE 9600
#define UART_DATA_BITS UART_DATA_8_BITS
#define UART_PARITY UART_PARITY_DISABLE
#define UART_STOP_BITS UART_STOP_BITS_1
#define UART_FLOW_CTRL UART_HW_FLOWCTRL_DISABLE

#define UART_RV_BUFFER_SIZE 1024

#define TXD_PIN 33
#define RXD_PIN 35


#endif
