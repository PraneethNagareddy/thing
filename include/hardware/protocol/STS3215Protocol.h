//
// Feetech STS3215 Servo Protocol
//
// The STS3215 is an STS-series smart servo with a 12-bit magnetic encoder
// (4096 steps/rev, 0-360 degrees). Key hardware difference vs SCS0009:
//   - 16-bit registers use LITTLE-ENDIAN byte order (low byte first)
//   - Load register max is 1000 (not 1023)
//   - Has a dedicated hardware MOVING status register
//   - EEPROM Lock register is at 0x37 (not 0x30)
//

#ifndef THING_STS3215PROTOCOL_H
#define THING_STS3215PROTOCOL_H

#include "FeetechServoProtocol.h"

namespace hardware {

    class STS3215Protocol : public FeetechServoProtocol {
    private:
        // --- EEPROM Register Map (persistent, address 0x00-0x36) ---
        static constexpr uint8_t REG_MIN_ANGLE_LIMIT  = 0x09; // 2 bytes LE
        static constexpr uint8_t REG_MAX_ANGLE_LIMIT  = 0x0B; // 2 bytes LE
        static constexpr uint8_t REG_MAX_TORQUE       = 0x10; // 2 bytes LE

        // --- RAM Register Map (volatile, address 0x28-0x...) ---
        static constexpr uint8_t REG_TORQUE_ENABLE    = 0x28; // 1 byte
        static constexpr uint8_t REG_GOAL_POSITION    = 0x2A; // 2 bytes LE
        static constexpr uint8_t REG_GOAL_TIME        = 0x2C; // 2 bytes LE (time-to-target in ms)
        static constexpr uint8_t REG_GOAL_SPEED       = 0x2E; // 2 bytes LE
        static constexpr uint8_t REG_TORQUE_LIMIT     = 0x30; // 2 bytes LE
        static constexpr uint8_t REG_LOCK             = 0x37; // 1 byte  (EEPROM write lock)
        static constexpr uint8_t REG_PRESENT_POSITION = 0x38; // 2 bytes LE
        static constexpr uint8_t REG_PRESENT_SPEED    = 0x3A; // 2 bytes LE (bit15=direction)
        static constexpr uint8_t REG_PRESENT_LOAD     = 0x3C; // 2 bytes LE (bit10=dir, bits0-9=magnitude/1000)
        static constexpr uint8_t REG_PRESENT_VOLTAGE  = 0x3E; // 1 byte (value * 0.1V)
        static constexpr uint8_t REG_PRESENT_TEMP     = 0x3F; // 1 byte (degrees Celsius)
        static constexpr uint8_t REG_MOVING           = 0x42; // 1 byte (1=moving, 0=stationary)

        // Scale constants
        static constexpr float MAX_LOAD_VALUE         = 1000.0f;
        static constexpr uint16_t LOAD_MAGNITUDE_MASK = 0x03FF; // bits 0-9

        // The STS3215 uses little-endian byte order for 16-bit registers.
        // These override the big-endian helpers in FeetechServoProtocol.
        bool write16_le(uint8_t id, uint8_t reg, uint16_t value) const;
        int16_t read16_le(uint8_t id, uint8_t reg) const;

    public:
        using FeetechServoProtocol::FeetechServoProtocol;
        virtual ~STS3215Protocol() = default;

        // --- Command Methods ---
        bool set_torque_enable(uint8_t id, bool enable) const;
        bool move_steps(uint8_t id, uint16_t steps) const;
        bool set_speed(uint8_t id, uint16_t speed) const;
        bool set_eproom_lock(uint8_t id, bool lock) const;

        // --- Read Methods ---
        int16_t read_present_position(uint8_t id) const;
        int16_t read_present_speed(uint8_t id) const;
        int16_t read_present_load(uint8_t id) const;
        float   read_voltage(uint8_t id) const;
        int8_t  read_temperature(uint8_t id) const;
        std::pair<int16_t, int16_t> read_angle_limits(uint8_t id) const;
        int16_t read_max_torque(uint8_t id) const;
        bool    is_eproom_locked(uint8_t id) const;
        bool    read_moving(uint8_t id) const;

        // Atomic write of goal position + speed in one packet
        bool write_pos_speed(uint8_t id, uint16_t pos, uint16_t speed) const;
    };

}

#endif //THING_STS3215PROTOCOL_H
