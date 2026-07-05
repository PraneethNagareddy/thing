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
        include/telemetry/alert/Alert.h
        include/telemetry/Reading.h
        src/anatomy/Finger.cpp
        src/anatomy/Hand.cpp
        src/anatomy/Thumb.cpp
        src/articulation/Joint.cpp
        src/telemetry/TelemetryManager.cpp
        src/telemetry/alert/ReadingEvaluator.cpp
        include/telemetry/alert/AlertRule.h
        include/telemetry/alert/AlertRulesFactory.h
        src/telemetry/alert/AlertRulesFactory.cpp
        include/telemetry/alert/ReadingEvaluator.h
        include/telemetry/logging/LiveLogger.h
        src/telemetry/logger/LiveLogger.cpp
        include/utility/Utils.h
        include/telemetry/alert/AlertHandler.h
        src/telemetry/alert/AlertHandler.cpp
        include/hardware/SerialBus.h
        include/hardware/FeetechServo.h
        include/hardware/SCS0009.h
        src/hardware/SCS0009.cpp
        src/hardware/MacSerialBus.cpp
        include/hardware/LinuxSerialBus.h
)

target_include_directories(thing PRIVATE include)