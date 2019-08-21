set(CMAKE_SYSTEM_NAME Generic)
set(toolchain $ENV{KVASIR_TOOLCHAIN})
set(CMAKE_TOOLCHAIN_FILE ${toolchain}/compilers/arm-none-eabi.cmake)