#include <esp8266.h>
#include <espressif/esp_common.h>
#include <esp/uart.h>
#include <esp/wdt_regs.h>

#include "ir_esp_uart.h"

static char carrierFrequency = 0; // Default carrier frequency
static char dutyCycleChar = 0xF0;

static unsigned long sigTime = 0; //used in mark & space functions to keep track of time

void ir_esp_uart_init() {
    ir_esp_uart_set_frequency(38);
}

void ir_esp_uart_begin() {
    /* Activate UART for GPIO2 */
    gpio_set_iomux_function(2, IOMUX_GPIO2_FUNC_UART1_TXD);

    UART(1).CONF0 = SET_MASK_BITS(UART(1).CONF0, UART_CONF0_TXD_INVERTED);

    sigTime = sdk_system_get_time();
}

void ir_esp_uart_end() {
    /* Activate UART for GPIO2 */
    gpio_set_iomux_function(2, IOMUX_GPIO2_FUNC_GPIO);

    UART(1).CONF0 = SET_MASK_BITS(UART(1).CONF0, UART_CONF0_TXD_INVERTED);
}

void ir_esp_uart_set_frequency(uint16_t frequency) {
    carrierFrequency = frequency;
    uart_set_baud(1, carrierFrequency * 10000);
}

void ir_esp_uart_set_duty_cycle(uint8_t dutyCycle) {
    switch (dutyCycle) {
        case 40: // 40%
            dutyCycleChar = 0xF8;
            break;
        case 30: // 30%
            dutyCycleChar = 0xFC;
            break;
        case 20: // 20%
            dutyCycleChar = 0xFE;
            break;
        case 10: // 10%
            dutyCycleChar = 0xFF;
            break;
        case 50: // 50%, default value
        default:
            dutyCycleChar = 0xF0;
            break;
    }
}

void ir_esp_uart_mark(uint16_t time) {
    sigTime += time; //mark ends at new sigTime
    unsigned long startTime = sdk_system_get_time();
    long dur = sigTime - startTime; //allows for rolling time adjustment due to code execution delays

    if (dur <= 0) return;

    unsigned int cycleCount = dur / ((1000 + carrierFrequency / 2) / carrierFrequency); // get number of cycles & do rounding with integer maths
    
    // ESP.wdtFeed(); //avoid watchdog issues
    WDT.FEED = WDT_FEED_MAGIC;
    while (cycleCount) {
      // while (true) { //send continuous carrier, for testing, signal generator or just generic PWM
        uart_putc(1, dutyCycleChar);
        // uart_flush_txfifo(1);
        --cycleCount;
        if ((cycleCount ^ 0x1F) == 0)WDT.FEED = WDT_FEED_MAGIC; //avoid watchdog issues
    }

    while ((sdk_system_get_time() - startTime) < dur) {
    } //just wait here until time is up
}

void ir_esp_uart_space(uint16_t time) {
    sigTime += time; //space ends at new sigTime

    unsigned long startTime = sdk_system_get_time();
    long dur = sigTime - startTime; //allows for rolling time adjustment due to code execution delays
    if (dur <= 0) return;
    unsigned int cycleCount = 0; //
    // ESP.wdtFeed();  //avoid watchdog issues
    WDT.FEED = WDT_FEED_MAGIC;
    while ((sdk_system_get_time() - startTime) < dur) { //just wait here until time is up
      if ((cycleCount++ ^ 0x1f) == 0)WDT.FEED = WDT_FEED_MAGIC; //avoid watchdog issues
    }
}
