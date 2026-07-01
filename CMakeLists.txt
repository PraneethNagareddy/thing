cmake_minimum_required(VERSION 4.2)
project(thing)

set(CMAKE_CXX_STANDARD 20)

add_executable(thing main.cpp
        include/articulation/Motion.h
        include/articulation/Joint.h
        include/hardware/Servo.h
        include/anatomy/Hand.h
        include/anatomy/Finger.h
        include/anatomy/Thumb.h
        include/telemetry/IMonitorable.h
        include/telemetry/Alert.h
        include/telemetry/Reading.h
        src/anatomy/Finger.cpp
        src/anatomy/Hand.cpp
        src/anatomy/Thumb.cpp
        src/articulation/Joint.cpp)

target_include_directories(thing PRIVATE include)