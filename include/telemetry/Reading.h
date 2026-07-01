//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_READING_H
#define THING_READING_H

#include <chrono>

namespace telemetry {
    struct BaseReading {
    protected:
        explicit BaseReading(std::chrono::steady_clock::time_point timestamp) : timestamp(timestamp) {}
    public:
        std::chrono::steady_clock::time_point timestamp;
    private:
        BaseReading() = default;
    };

    struct JointReading : BaseReading {
        int servo_id{};
        float temperature_celsius{};
        float load_percentage{};
        int voltage_mv{};
        int current_step{};
        float current_angle_degrees{};
        float current_speed_steps_per_second{};

        JointReading(std::chrono::steady_clock::time_point timestamp,
                      int servo_id,
                      float temperature_celsius,
                      float load_percentage,
                      int voltage_mv,
                      int current_step,
                      float current_angle_degrees,
                      float current_speed_steps_per_second)
             : BaseReading(timestamp),
               servo_id(servo_id),
               temperature_celsius(temperature_celsius),
               load_percentage(load_percentage),
               voltage_mv(voltage_mv),
               current_step(current_step),
               current_angle_degrees(current_angle_degrees),
               current_speed_steps_per_second(current_speed_steps_per_second) {}
    };
}

#endif //THING_READING_H
