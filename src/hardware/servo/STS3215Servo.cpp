#include "hardware/servo/STS3215Servo.h"
#include <thread>
#include <chrono>

namespace hardware {

    STS3215Servo::~STS3215Servo() {
        freeze();
    }

    void STS3215Servo::move(int target_step, float target_speed_steps_per_sec) {
        if (frozen_.load()) return;

        // Clamp step to valid range [0, max_steps_]
        uint16_t step = static_cast<uint16_t>(
            std::max(0, std::min(target_step, max_steps_)));

        // Clamp and scale speed by the throttle factor
        float raw_speed = target_speed_steps_per_sec * speed_factor_;
        uint16_t speed = static_cast<uint16_t>(
            std::max(0.0f, std::min(raw_speed, max_speed_steps_per_sec_)));

        // Use atomic write to send position and speed in a single packet,
        // avoiding a race between separate set_speed / move_steps calls.
        protocol_->write_pos_speed(id_, step, speed);
    }

    const float STS3215Servo::read_current_angle_degrees() {
        int steps = read_current_step();
        if (steps == -1) return -1.0f;
        return (static_cast<float>(steps) / static_cast<float>(max_steps_)) * static_cast<float>(max_degrees_);
    }

    const int STS3215Servo::read_current_step() {
        return protocol_->read_present_position(id_);
    }

    const float STS3215Servo::read_load_percent() {
        int16_t load = protocol_->read_present_load(id_);
        if (load == -1) return -1.0f;

        // Bits 0-9: load magnitude (0-1000 = 0-100%)
        // Bit 10:   direction (0 = CW, 1 = CCW)
        static constexpr uint16_t LOAD_MAGNITUDE_MASK = 0x03FF;
        static constexpr float MAX_LOAD_VALUE = 1000.0f; // STS3215 uses 1000, not 1023

        return (static_cast<float>(load & LOAD_MAGNITUDE_MASK) / MAX_LOAD_VALUE) * 100.0f;
    }

    const float STS3215Servo::read_temperature_celsius() {
        return static_cast<float>(protocol_->read_temperature(id_));
    }

    const int STS3215Servo::read_voltage_mv() {
        float voltage_v = protocol_->read_voltage(id_);
        if (voltage_v < 0) return -1;
        return static_cast<int>(voltage_v * 1000.0f);
    }

    const float STS3215Servo::read_speed_steps_per_second() {
        int16_t speed = protocol_->read_present_speed(id_);
        if (speed == -1) return -1.0f;
        // Bit 15: direction (1 = reverse), bits 0-14: magnitude
        if (speed & 0x8000) {
            return -static_cast<float>(speed & 0x7FFF);
        }
        return static_cast<float>(speed);
    }

    const int STS3215Servo::read_id() {
        return static_cast<int>(id_);
    }

    void STS3215Servo::freeze() {
        // Only send hardware command on transition: NOT frozen -> frozen
        if (!frozen_.exchange(true)) {
            protocol_->set_torque_enable(id_, false);
        }
    }

    void STS3215Servo::unfreeze() {
        // Only send hardware command on transition: frozen -> NOT frozen
        if (frozen_.exchange(false)) {
            protocol_->set_torque_enable(id_, true);
        }
    }

    const bool STS3215Servo::is_moving(long poll_interval_ms, uint16_t threshold) {
        // Primary check: use the hardware MOVING status register (fast, no delay)
        if (protocol_->read_moving(id_)) return true;

        // Fallback: present speed above threshold
        float speed = read_speed_steps_per_second();
        if (std::abs(speed) > static_cast<float>(threshold)) return true;

        // Last resort: position delta over a short sample window
        int pos1 = read_current_step();
        std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_ms));
        int pos2 = read_current_step();

        if (pos1 == -1 || pos2 == -1) return false;
        return std::abs(pos1 - pos2) > static_cast<int>(threshold);
    }

    void STS3215Servo::throttle() {
        speed_factor_ = 0.5f;
    }

    void STS3215Servo::unthrottle() {
        speed_factor_ = 1.0f;
    }

} // namespace hardware
