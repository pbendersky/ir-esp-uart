/*
* Author: Pablo Bendersky
*
* Date: 25th December, 2018
*
* License: MIT
*
* An implementation of the ESP8266 IR hack trough UART described in https://www.analysir.com/blog/2017/01/29/updated-esp8266-nodemcu-backdoor-upwm-hack-for-ir-signals/.
* This code is a port of AnalysIR, provided as an ESP-OPEN-RTOS module.
*
*/

#ifndef __IR_ESP_UART_H__
#define __IR_ESP_UART_H__

/*
* Initializes the uart by setting a default frequency of 38khz.
*/
void ir_esp_uart_init();

/*
* Starts the UART for IR commands. While most IR libraries only provide `mark` and `space` methods,
* this implementations relies on accurate timestamps to work properly. In order to initialize the timestamps,
* we need to call `ir_esp_uart_begin` before any `ir_esp_uart_mark` / `ir_esp_uart_space` combination.
* In addition to that, this method sets the GPIO2 for UART and sets the inversed pin on it.
* We do this everytime since GPIO2 is shared with the built-in led in ESP12 modules. If we were
* to do it only once, the led would remain on.
*/
void ir_esp_uart_begin();

/*
* Ends the sequence of `ir_esp_uart_mark` and `ir_esp_uart_space` commands. This returns GPIO2 to GPIO instead
* of UART, so the built-in LED remains off.
*/
void ir_esp_uart_end();

/*
* Sets the frequency to modulate in. The frequency is expressed in khz (38).
*/
void ir_esp_uart_set_frequency(uint16_t frequency);

/*
* Sets the duty cycle. This defaults to 50%. Supprted values are 10, 20, 30, 40, and 50.
*/
void ir_esp_uart_set_duty_cycle(uint8_t dutyCycle);

/*
* Standard IR mark command.
*/
void ir_esp_uart_mark(uint16_t time);

/*
* Standard IR space command.
*/
void ir_esp_uart_space(uint16_t time);

#endif // __IR_ISP_UART_H__