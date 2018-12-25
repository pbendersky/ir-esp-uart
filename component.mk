# Component makefile for ir_esp_uart

# include this library as 'ir_esp_uart/ir_esp_uart.h'
INC_DIRS += $(ir_esp_uart_ROOT)..

# args for passing into compile rule generation
ir_esp_uart_SRC_DIR = $(ir_esp_uart_ROOT)

$(eval $(call component_compile_rules,ir_esp_uart))
