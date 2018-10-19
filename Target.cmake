# Set compilation traits
set(warnings
"-Wall"
"-Wextra"
#"-pedantic-errors"
"-Wfatal-errors"
"-Wcast-align"
"-Wmissing-declarations"
"-Wredundant-decls"
"-Wuninitialized"
"-Wno-unused-parameter"
"-Wno-missing-field-initializers")

set(cxxflags
"-std=c++17"
"-Os"
${warnings})

string(REGEX REPLACE ";" " " cxxflags "${cxxflags}")
string(REGEX REPLACE ";" " " ldflags "${ldflags}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxxflags}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ldflags}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

add_definitions (-DTARGET=arduino_due)

# Set kvasir target
set (cxxflags
    "-Os"
    "-ffunction-sections"
    "-fdata-sections"
    "-fno-exceptions"
    "-fno-rtti"
    "-fno-threadsafe-statics"
    # "-nostdlib"
    "-std=c++17")

string (REGEX REPLACE ";" " " cxxflags "${cxxflags}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxxflags}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--defsym=STACK_SIZE=92160")

# The target chip that is compiled for:
include (${toolchain}/targets/arm32/cm3/atmel/sam3x/sam3x8e/sam3x8e.cmake)

# The flashscript used to flash the binary (optional):
include (${toolchain}/targets/arm32/cm3/atmel/sam3x/sam3x8e/flashscripts/arduino_due.cmake)