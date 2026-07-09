//
// Created by Nagareddy on 10/07/26.
//

#ifndef THING_DUMMYSCS0009CONSOLEPROTOCOL_H
#define THING_DUMMYSCS0009CONSOLEPROTOCOL_H
#include "SCS0009Protocol.h"

namespace hardware {
    class DummySCS0009ConsoleProtocol : public SCS0009Protocol {
    public:
        using SCS0009Protocol::SCS0009Protocol;

        bool move_steps(uint8_t id, uint16_t steps) const override {
            std::cout << "[MOCK] Moving Servo " << (int)id << " to " << steps << " steps." << std::endl;
            return true;
        }

        bool set_torque_enable(uint8_t id, bool enable) const override {
            std::cout << "[MOCK] Servo " << (int)id << " Torque: " << (enable ? "ENABLED" : "DISABLED") << std::endl;
            return true;
        }

        bool set_speed(uint8_t id, uint16_t speed) const override {
            std::cout << "[MOCK] Setting Servo " << (int)id << " Speed to " << speed << " steps/s." << std::endl;
            return true;
        }

        bool set_eeprom_lock(uint8_t id, bool lock) const override {
            std::cout << "[MOCK] Servo " << (int)id << " EEPROM: " << (lock ? "LOCKED" : "UNLOCKED") << std::endl;
            return true;
        }

        int16_t read_present_position(uint8_t id) const override {
            return 512; // Return middle position as default
        }

        int16_t read_present_speed(uint8_t id) const override {
            return 0; // Return stationary
        }

        int16_t read_present_load(uint8_t id) const override {
            return 0; // Return no load
        }

        float read_voltage(uint8_t id) const override {
            return 5.0f; // Return nominal 5V
        }

        int8_t read_temperature(uint8_t id) const override {
            return 25; // Return 25C room temp
        }

        std::pair<int16_t, int16_t> read_angle_limits(uint8_t id) const override {
            return {0, 1023}; // Return full range
        }

        int16_t read_max_torque(uint8_t id) const override {
            return 1023; // Return max possible torque cap
        }

        bool is_eeprom_locked(uint8_t id) const override {
            return true;
        }

    };
}


#endif //THING_DUMMYSCS0009CONSOLEPROTOCOL_H
