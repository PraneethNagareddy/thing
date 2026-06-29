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
    public:
        Servo(uint8_t id, ServoType type = ServoType::SERIAL_BUS, CommunicationProtocol protocol = CommunicationProtocol::TTL) :
            type_(type),
            protocol_(protocol),
            id_(id) {

            assert(id >= 1 && id <= 253 && "Servo ID must be between 1 and 253!");
        }
        ~Servo();
        void move(float target_angle);
        void move(int target_step);
        float currentAngle();
        int currentStep();
        float readLoad();
        float readTemperature();
        void freeze();
        void unfreeze();
        bool isMoving(long poll_interval_ms = 50, uint16_t threshold = 5);
    };

}
#endif //THING_SERVO_H