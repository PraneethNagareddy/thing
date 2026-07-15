//
// Created by Nagareddy on 06/07/26.
//

#ifndef THING_SCS0009_H
#define THING_SCS0009_H

#include "FeetechServoProtocol.h"

namespace hardware {

    class SCS0009Protocol : public FeetechServoProtocol {
    private:
        // Register Map
        static constexpr uint8_t REG_MIN_ANGLE_LIMIT  = 0x09;
        static constexpr uint8_t REG_MAX_ANGLE_LIMIT  = 0x0B;
        static constexpr uint8_t REG_MAX_TORQUE       = 0x10;
        static constexpr uint8_t REG_TORQUE_ENABLE    = 0x28;
        static constexpr uint8_t REG_GOAL_POSITION    = 0x2A;
        static constexpr uint8_t REG_MOVING_SPEED     = 0x2E;
        static constexpr uint8_t REG_LOCK             = 0x30;
        static constexpr uint8_t REG_PRESENT_POSITION = 0x38;
        static constexpr uint8_t REG_PRESENT_SPEED    = 0x3A;
        static constexpr uint8_t REG_PRESENT_LOAD     = 0x3C;
        static constexpr uint8_t REG_PRESENT_VOLTAGE  = 0x3E;
        static constexpr uint8_t REG_PRESENT_TEMP     = 0x3F;

    public:
        using FeetechServoProtocol::FeetechServoProtocol;
        virtual ~SCS0009Protocol() = default;

        // --- Command Methods ---
        virtual bool set_torque_enable(uint8_t id, bool enable) const;

        virtual bool move_steps(uint8_t id, uint16_t steps) const;

        virtual bool set_speed(uint8_t id, uint16_t speed) const;

        virtual bool set_eproom_lock(uint8_t id, bool lock) const;

        // --- Read Methods ---
        virtual int16_t read_present_position(uint8_t id) const;
        virtual int16_t read_present_speed(uint8_t id) const;
        virtual int16_t read_present_load(uint8_t id) const;

        virtual float read_voltage(uint8_t id) const;

        virtual int8_t read_temperature(uint8_t id) const;

        virtual std::pair<int16_t, int16_t> read_angle_limits(uint8_t id) const;

        virtual int16_t read_max_torque(uint8_t id) const;

        virtual bool is_eproom_locked(uint8_t id) const;

        bool write_pos_speed(uint8_t id, uint16_t pos, uint16_t speed) const;
    };
}

#endif //THING_SCS0009_H