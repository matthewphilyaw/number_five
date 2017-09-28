cmake_minimum_required(VERSION 3.8)
enable_language(C ASM)

project(N5)

# Project Variables
string(TOLOWER ${MCU_FAMILY} MCU_FAMILY_LOWER)
string(TOLOWER ${MCU_PART} MCU_PART_LOWER)

set(DRIVER_DIR drivers)
set(FREE_RTOS_DIR FreeRTOS)

# Definitions
add_definitions(-D${MCU_PART} -DUSE_FULL_LL_DRIVER)

# Includes
include_directories(
        include
        ${DRIVER_DIR}/CMSIS/Include
        ${DRIVER_DIR}/CMSIS/Device/ST/${MCU_FAMILY}/Include
        ${DRIVER_DIR}/${MCU_FAMILY}_HAL_Driver/Inc
        ${DRIVER_DIR}/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
        ${FREE_RTOS_DIR}/Source/include
        ${FREE_RTOS_DIR}/Source/portable/GCC/${RTOS_TARGET}
)

# User source
set(USER_SOURCE
        src/main.c
        src/bsp/board.c
        src/display/ssd1306.c
        src/display/gfx.c
        src/task/blinky.c
        src/task/menu.c
        src/task/runtime_stats.c)

# Libraries
set(HAL_DIR_PREFIX ${DRIVER_DIR}/${MCU_FAMILY}_HAL_Driver/Src/${MCU_FAMILY_LOWER}_ll_)
add_library(HAL
        ${HAL_DIR_PREFIX}adc.c
        ${HAL_DIR_PREFIX}comp.c
        ${HAL_DIR_PREFIX}crc.c
        ${HAL_DIR_PREFIX}crs.c
        ${HAL_DIR_PREFIX}dac.c
        ${HAL_DIR_PREFIX}dma.c
        ${HAL_DIR_PREFIX}exti.c
        ${HAL_DIR_PREFIX}gpio.c
        ${HAL_DIR_PREFIX}i2c.c
        ${HAL_DIR_PREFIX}lptim.c
        ${HAL_DIR_PREFIX}lpuart.c
        ${HAL_DIR_PREFIX}pwr.c
        ${HAL_DIR_PREFIX}rcc.c
        ${HAL_DIR_PREFIX}rng.c
        ${HAL_DIR_PREFIX}rtc.c
        ${HAL_DIR_PREFIX}spi.c
        ${HAL_DIR_PREFIX}tim.c
        ${HAL_DIR_PREFIX}usart.c
        ${HAL_DIR_PREFIX}utils.c
)

add_library(CMSIS
        ${DRIVER_DIR}/CMSIS/Device/ST/${MCU_FAMILY}/Source/Templates/gcc/startup_${MCU_PART_LOWER}.s
        ${DRIVER_DIR}/CMSIS/Device/ST/${MCU_FAMILY}/Source/Templates/system_${MCU_FAMILY_LOWER}.c)

add_library(FREE_RTOS
        ${FREE_RTOS_DIR}/Source/portable/GCC/${RTOS_TARGET}/port.c
        ${FREE_RTOS_DIR}/Source/portable/MemMang/heap_1.c
        ${FREE_RTOS_DIR}/Source/croutine.c
        ${FREE_RTOS_DIR}/Source/event_groups.c
        ${FREE_RTOS_DIR}/Source/list.c
        ${FREE_RTOS_DIR}/Source/queue.c
        ${FREE_RTOS_DIR}/Source/tasks.c
        ${FREE_RTOS_DIR}/Source/timers.c)

add_executable(${PROJECT_NAME}.elf ${USER_SOURCE} ${LINKER_SCRIPT} )

target_link_libraries(${PROJECT_NAME}.elf HAL CMSIS FREE_RTOS)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")

set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)

set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${TOOL_PATH}/arm-none-eabi-size --format=berkeley "${PROJECT_NAME}.elf"
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")
