#include "hardware/protocol/STS3215Protocol.h"
#include <thread>
#include <chrono>

namespace hardware {

    // -------------------------------------------------------------------------
    // Little-Endian 16-bit helpers
    //
    // The STS3215 (STS series) transmits 16-bit values LOW byte first, HIGH
    // byte second — the opposite of the SCS0009 (SCS series) which is big-
    // endian. We cannot use the base-class write16/read16 here.
    // -------------------------------------------------------------------------

    bool STS3215Protocol::write16_le(uint8_t id, uint8_t reg, uint16_t value) const {
        bool ok;
        {
            std::lock_guard<std::mutex> lock(transaction_mutex_);
            // Little-endian: low byte first, high byte second
            ok = send_packet(id, INST_WRITE, {
                reg,
                static_cast<uint8_t>(value & 0xFF),        // low byte
                static_cast<uint8_t>((value >> 8) & 0xFF)  // high byte
            });
        }
        // Recovery time for the servo MCU (mirroring base class timing)
        std::this_thread::sleep_for(std::chrono::microseconds(200));
        return ok;
    }

    int16_t STS3215Protocol::read16_le(uint8_t id, uint8_t reg) const {
        std::vector<uint8_t> res;
        {
            std::lock_guard<std::mutex> lock(transaction_mutex_);
            if (!send_packet(id, INST_READ, {reg, 2})) return -1;
            res = receive_packet(id, 2);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(200));

        if (res.size() != 2) return -1;
        // Little-endian: res[0] = low byte, res[1] = high byte
        return static_cast<int16_t>(res[0] | (res[1] << 8));
    }

    // -------------------------------------------------------------------------
    // Command Methods
    // -------------------------------------------------------------------------

    bool STS3215Protocol::set_torque_enable(uint8_t id, bool enable) const {
        return write8(id, REG_TORQUE_ENABLE, enable ? 1 : 0);
    }

    bool STS3215Protocol::move_steps(uint8_t id, uint16_t steps) const {
        return write16_le(id, REG_GOAL_POSITION, steps);
    }

    bool STS3215Protocol::set_speed(uint8_t id, uint16_t speed) const {
        return write16_le(id, REG_GOAL_SPEED, speed);
    }

    bool STS3215Protocol::set_eproom_lock(uint8_t id, bool lock) const {
        return write8(id, REG_LOCK, lock ? 1 : 0);
    }

    // -------------------------------------------------------------------------
    // Read Methods
    // -------------------------------------------------------------------------

    int16_t STS3215Protocol::read_present_position(uint8_t id) const {
        return read16_le(id, REG_PRESENT_POSITION);
    }

    int16_t STS3215Protocol::read_present_speed(uint8_t id) const {
        return read16_le(id, REG_PRESENT_SPEED);
    }

    int16_t STS3215Protocol::read_present_load(uint8_t id) const {
        return read16_le(id, REG_PRESENT_LOAD);
    }

    std::pair<int16_t, int16_t> STS3215Protocol::read_angle_limits(uint8_t id) const {
        return {read16_le(id, REG_MIN_ANGLE_LIMIT), read16_le(id, REG_MAX_ANGLE_LIMIT)};
    }

    float STS3215Protocol::read_voltage(uint8_t id) const {
        int8_t v = read8(id, REG_PRESENT_VOLTAGE);
        if (v == -1) return -1.0f;
        return static_cast<float>(static_cast<uint8_t>(v)) / 10.0f; // 1 unit = 0.1V
    }

    int8_t STS3215Protocol::read_temperature(uint8_t id) const {
        return read8(id, REG_PRESENT_TEMP);
    }

    int16_t STS3215Protocol::read_max_torque(uint8_t id) const {
        return read16_le(id, REG_MAX_TORQUE);
    }

    bool STS3215Protocol::is_eproom_locked(uint8_t id) const {
        return read8(id, REG_LOCK) == 1;
    }

    bool STS3215Protocol::read_moving(uint8_t id) const {
        return read8(id, REG_MOVING) == 1;
    }

    // -------------------------------------------------------------------------
    // Atomic Position + Speed Write
    //
    // Writes goal position, goal time (0 = ASAP), and goal speed in a single
    // packet starting at REG_GOAL_POSITION (0x2A) — all in little-endian order.
    // -------------------------------------------------------------------------
    bool STS3215Protocol::write_pos_speed(uint8_t id, uint16_t pos, uint16_t speed) const {
        std::vector<uint8_t> params = {
            REG_GOAL_POSITION,
            static_cast<uint8_t>(pos   & 0xFF), static_cast<uint8_t>((pos   >> 8) & 0xFF), // goal pos LE
            0x00, 0x00,                                                                      // goal time = 0 LE
            static_cast<uint8_t>(speed & 0xFF), static_cast<uint8_t>((speed >> 8) & 0xFF)  // goal speed LE
        };
        std::lock_guard<std::mutex> lock(transaction_mutex_);
        return send_packet(id, INST_WRITE, params);
    }

} // namespace hardware
