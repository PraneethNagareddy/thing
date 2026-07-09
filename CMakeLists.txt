cmake_minimum_required(VERSION 4.2)
project(thing)

set(CMAKE_CXX_STANDARD 20)

add_executable(thing main.cpp
        include/articulation/Motion.h
        include/articulation/Joint.h
        include/hardware/SCS0009Servo.h
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
        include/hardware/bus/SerialBus.h
        include/hardware/protocol/FeetechServoProtocol.h
        include/hardware/protocol/SCS0009Protocol.h
        src/hardware/protocol/SCS0009Protocol.cpp
        src/hardware/bus/MacSerialBus.cpp
        include/hardware/bus/LinuxSerialBus.h
        include/builder/HandBuilder.h
        src/builder/HandBuilder.cpp
        src/hardware/protocol/FeetechServoProtocol.cpp
        src/hardware/SCS0009Servo.cpp
        include/telemetry/TelemetryManager.h
        include/articulation/Constants.h
)

target_include_directories(thing PRIVATE include)