include(Kvasir.cmake)
project(hwstl)
cmake_minimum_required(VERSION 3.0.0)

set(sources
    main.cpp
    target/arduino-due.cpp
)

include(Target.cmake)