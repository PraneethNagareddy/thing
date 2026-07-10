#include "../../../include/hardware/servo/SCS0009Servo.h"
#include <thread>
#include <chrono>

namespace hardware {

    SCS0009Servo::~SCS0009Servo() {
        freeze();
    }

    void SCS0009Servo::move(int target_step, float target_speed_steps_per_sec) {
        if (frozen_.load()) return;

        // Set speed first, then move
        protocol_->set_speed(id_, static_cast<uint16_t>(target_speed_steps_per_sec * speed_factor_));
        protocol_->move_steps(id_, static_cast<uint16_t>(target_step));
    }

    const float SCS0009Servo::read_current_angle_degrees() {
        int steps = read_current_step();
        if (steps == -1) return -1.0f;
        return (static_cast<float>(steps) / max_steps_) * max_degrees_;
    }

    const int SCS0009Servo::read_current_step() {
        return protocol_->read_present_position(id_);
    }

    const float SCS0009Servo::read_load_percent() {
        int16_t load = protocol_->read_present_load(id_);
        if (load == -1) return -1.0f;
        // Bits 0-9 are magnitude (0-1023), bit 10 is direction
        return (static_cast<float>(load & 0x3FF) / 1023.0f) * 100.0f;
    }

    const float SCS0009Servo::read_temperature_celsius() {
        return static_cast<float>(protocol_->read_temperature(id_));
    }

    const int SCS0009Servo::read_voltage_mv() {
        float voltage_v = protocol_->read_voltage(id_);
        if (voltage_v < 0) return -1;
        return static_cast<int>(voltage_v * 1000.0f);
    }

    const float SCS0009Servo::read_speed_steps_per_second() {
        int16_t speed = protocol_->read_present_speed(id_);
        if (speed == -1) return -1.0f;
        // SCS speed is signed (bit 15 is direction)
        if (speed & 0x8000) {
            return -static_cast<float>(speed & 0x7FFF);
        }
        return static_cast<float>(speed);
    }

    const int SCS0009Servo::read_id() {
        return static_cast<int>(id_);
    }

    void SCS0009Servo::freeze() {
        // Only send hardware command if we are transitioning from NOT frozen to frozen
        if (!frozen_.exchange(true)) {
            protocol_->set_torque_enable(id_, false);
        }
    }

    void SCS0009Servo::unfreeze() {
        // Only send hardware command if we are transitioning from frozen to NOT frozen
        if (frozen_.exchange(false)) {
            protocol_->set_torque_enable(id_, true);
        }
    }

    const bool SCS0009Servo::is_moving(long poll_interval_ms, uint16_t threshold) {
        // First check: Present Speed
        float speed = read_speed_steps_per_second();
        if (std::abs(speed) > threshold) return true;

        // Second check: Position delta over time
        int pos1 = read_current_step();
        std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_ms));
        int pos2 = read_current_step();

        if (pos1 == -1 || pos2 == -1) return false;
        return std::abs(pos1 - pos2) > threshold;
    }

    void SCS0009Servo::throttle() {
        speed_factor_ = 0.5f;
    }

    void SCS0009Servo::unthrottle() {
        speed_factor_ = 1.0f;
    }
}
