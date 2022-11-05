
cmake_minimum_required(VERSION 3.13) # CMAKE (heading)
include(pico_sdk_import.cmake) # CMAKE -> PICO SDK (include)

project(repl C CXX ASM) # CMAKE ("project")
pico_sdk_init() # PICO SDK (initialization)

add_executable(repl) # CMAKE (target)

pico_generate_pio_header(repl ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}) # PICO SDK (PIO)

target_sources(repl PRIVATE REPL_console.c pixel.c sio_registers.c button_capture.c data_transfer.c) # CMAKE -> C (compiler)
target_link_libraries(repl PRIVATE pico_stdlib hardware_pio) # CMAKE -> C (linker)

pico_enable_stdio_usb(repl 1) # PICO SDK (USB)
pico_enable_stdio_uart(repl 0) # PICO SDK (USB)

pico_add_extra_outputs(repl) # PICO SDK (UF2)