//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_SERVO_H
#define THING_SERVO_H

#include <cassert>
#include <string>
#include <stdexcept>
#include <utility>

#include "../protocol/SCS0009Protocol.h"

namespace hardware {

    enum class ServoType {
        PWM,
        SERIAL_BUS
    };

    class SCS0009Servo {
    private:
        std::string name_;
        ServoType type_;
        uint8_t id_;
        std::shared_ptr<SCS0009Protocol> protocol_;
        std::atomic<bool> frozen_{false};
        float max_speed_steps_per_sec_;
        float speed_factor_;
        int max_degrees_;
        int max_steps_;
    public:
        SCS0009Servo(uint8_t id,
            std::string name,
            std::shared_ptr<SCS0009Protocol> protocol,
            int max_degrees = 300,
            int max_steps = 1023,
            ServoType type = ServoType::SERIAL_BUS,
            float max_speed_steps_per_sec = 2929.0f,
            float speed_factor = 1.0f) : type_(type),
                                                 name_(std::move(name)),
                                                 protocol_(std::move(protocol)),
                                                 id_(id),
                                                 max_speed_steps_per_sec_(max_speed_steps_per_sec),
                                                 speed_factor_(speed_factor),
                                                 max_degrees_(max_degrees),
                                                 max_steps_(max_steps) {
            assert(id >= 1 && id <= 253 && "Servo ID must be between 1 and 253!");
        }

        ~SCS0009Servo();
        void move(int target_step, float target_speed_steps_per_sec);
        const float read_current_angle_degrees();
        const int read_current_step();
        const float read_load_percent();
        const float read_temperature_celsius();
        const int read_voltage_mv();
        const float read_speed_steps_per_second();
        const int read_id();
        void freeze();
        void unfreeze();
        void throttle();
        void unthrottle();
        const bool is_moving(long poll_interval_ms = 50, uint16_t threshold = 5);
        int deg_to_steps(float deg) const {
            return int(round(std::max(0.0f, std::min(static_cast<float>(max_degrees_), deg)) / max_degrees_ * max_steps_));
        }
    };

}
#endif //THING_SERVO_H