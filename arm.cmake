include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

include(var.arm.cmake)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER(${TOOL_PATH}/arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${TOOL_PATH}/arm-none-eabi-g++ GNU)

unset(CMAKE_C_FLAGS CACHE)
unset(CMAKE_CXX_FLAGS CACHE)
unset(CMAKE_EXE_LINKER_FLAGS CACHE)
unset(CMAKE_ASM_FLAGS CACHE)

set(COMMON_FLAGS "-mcpu=${MCU_MCPU} -mthumb -mfloat-abi=soft -ffunction-sections -fdata-sections -gdwarf-2 -Wall -O2 -g2  -fno-common -fmessage-length=0")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11 -Wextra --pedantic-errors -fno-exceptions -fno-rtti -fno-threadsafe-statics" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -gdwarf-2 --specs=nano.specs --specs=nosys.specs -Wl,-wrap,__aeabi_unwind_cpp_pr0 -Wl,-wrap,__aeabi_unwind_cpp_pr1 -Wl,-wrap,__aeabi_unwind_cpp_pr2 -lm -T ${LINKER_SCRIPT}" CACHE STRING "" FORCE)
