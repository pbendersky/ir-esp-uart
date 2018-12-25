# IR ESP UART - Reliable IR library for ESP-OPEN-RTOS

This library provides an implementation of the hack described by [AnalysIR] to reliably send IR codes using
the secondary UART in an ESP8266 module.

IR codes are sent on GPIO2, which is also shared by the built-in LED.

## Usage

While most IR libraries provide only `mark` and `space` methods, to use this library you must wrap your `mark` / `space`
combination within `begin`, `end` calls.

Typical usage would be:

```
// Common initialization (can be done only once)
ir_esp_uart_init();
ir_esp_set_frequency(38);

// Command begin
ir_esp_uart_begin();

// Command
ir_esp_uart_mark(9000);
ir_esp_uart_space(500);
ir_esp_uart_mark(9000);
ir_esp_uart_space(500);

// Command end
ir_esp_uart_end();
```

If you are migrating from another IR library it's typically easy to wrap the `mark` / `space` calls in a method to add the 
required `begin` / `end` calls.

`ir_esp_uart_begin` initializes `GPIO2` for UART usage, and initializes a timestamp required for better timings in `mark` / 
`space` calls.

`ir_esp_uart_end` ends a command and returns `GPIO2` to GPIO.

[AnalysIR]: https://www.analysir.com/blog/2017/01/29/updated-esp8266-nodemcu-backdoor-upwm-hack-for-ir-signals/