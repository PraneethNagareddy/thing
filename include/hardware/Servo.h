//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_SERVO_H
#define THING_SERVO_H

#include <cassert>
#include <string>
#include <stdexcept>

namespace hardware {

    enum class ServoType {
        PWM,
        SERIAL_BUS
    };

    enum class CommunicationProtocol {
        PWM,
        TTL,
        RS485,
    };

    class Servo {
    private:
        std::string name_;
        ServoType type_;
        CommunicationProtocol protocol_;
        uint8_t id_;
        std::atomic<bool> frozen_{false};
        float max_speed_steps_per_sec_;
        float default_speed_factor_;
        int max_degrees_;
        int max_steps_;
    public:
        Servo(uint8_t id,
            int max_degrees = 300,
            int max_steps = 1023,
            ServoType type = ServoType::SERIAL_BUS,
            CommunicationProtocol protocol = CommunicationProtocol::TTL,
            float max_speed_steps_per_sec = 2929.0f,
            float default_speed_factor = 1.0f) :
            type_(type),
            protocol_(protocol),
            id_(id),
            max_speed_steps_per_sec_(max_speed_steps_per_sec),
            default_speed_factor_(default_speed_factor),
            max_degrees_(max_degrees),
            max_steps_(max_steps){

            assert(id >= 1 && id <= 253 && "Servo ID must be between 1 and 253!");
        }
        ~Servo();
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
        const bool is_moving(long poll_interval_ms = 50, uint16_t threshold = 5);
        int deg_to_steps(float deg) {
            return int(round(std::max(0.0f, std::min(static_cast<float>(max_degrees_), deg)) / max_degrees_ * max_steps_));
        }
    };

}
#endif //THING_SERVO_H