#include "hardware/protocol/SCS0009Protocol.h"

namespace hardware {

    bool SCS0009Protocol::set_torque_enable(uint8_t id, bool enable) const {
        return write8(id, REG_TORQUE_ENABLE, enable ? 1 : 0);
    }

    bool SCS0009Protocol::move_steps(uint8_t id, uint16_t steps) const {
        return write16(id, REG_GOAL_POSITION, steps);
    }

    bool SCS0009Protocol::set_speed(uint8_t id, uint16_t speed) const {
        return write16(id, REG_MOVING_SPEED, speed);
    }

    bool SCS0009Protocol::set_eproom_lock(uint8_t id, bool lock) const {
        return write8(id, REG_LOCK, lock ? 1 : 0);
    }

    int16_t SCS0009Protocol::read_present_position(uint8_t id) const {
        return read16(id, REG_PRESENT_POSITION);
    }

    int16_t SCS0009Protocol::read_present_speed(uint8_t id) const {
        return read16(id, REG_PRESENT_SPEED);
    }

    int16_t SCS0009Protocol::read_present_load(uint8_t id) const {
        return read16(id, REG_PRESENT_LOAD);
    }

    float SCS0009Protocol::read_voltage(uint8_t id) const {
        int8_t v = read8(id, REG_PRESENT_VOLTAGE);
        if (v == -1) return -1.0f;
        return static_cast<float>(v) / 10.0f; // Standard Feetech scaling: 1 unit = 0.1V
    }

    int8_t SCS0009Protocol::read_temperature(uint8_t id) const {
        return read8(id, REG_PRESENT_TEMP);
    }

    int16_t SCS0009Protocol::read_max_torque(uint8_t id) const {
        return read16(id, REG_MAX_TORQUE);
    }

    bool SCS0009Protocol::is_eproom_locked(uint8_t id) const {
        return read8(id, REG_LOCK) == 1;
    }
}