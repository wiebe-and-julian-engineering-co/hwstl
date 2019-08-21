include(Kvasir.cmake)
project(hwstl)
cmake_minimum_required(VERSION 3.0.0)

# A more pretty solution in the future is to define a CMakeLists.txt for all boards. Then,
# each board is dynamically imported by using a parameter, like $BOARD_TYPE. CMake should
# then import all required build files for that specific target.

set(sources
    target/board/arduino-due/arduino-due.cpp
    target/board/arduino-uno/arduino-uno.cpp
    target/board/arduino-zero/arduino-zero.cpp
    target/board/arduino-micro/arduino-micro.cpp
    target/arm-sam3x8e/core.cpp
    main.cpp
)

include(Target.cmake)