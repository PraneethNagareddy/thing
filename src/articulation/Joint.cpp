//
// Created by Nagareddy on 02/07/26.
//

#include "anatomy/Thumb.h"
#include "articulation/Joint.h"

using namespace articulation;
using namespace articulation::movement;
using namespace telemetry;

namespace articulation {
    Status Joint::move_to(const float movement_percent, const int time_to_execute_ms) {
        if (movement_percent < 0.0f || movement_percent > 1.0f) {
            throw std::out_of_range("Movement percentage must be between 0.0f and 1.0f");
        }
        float target_angle = no_flex_angle_ + ((full_flex_angle_ - no_flex_angle_) * movement_percent);
        target_angle = std::clamp(target_angle, no_flex_angle_, full_flex_angle_);
        const int target_step = servo_->deg_to_steps(target_angle);
        const int current_step = servo_->read_current_step();
        const float target_speed = static_cast<float>(abs(target_step - current_step)) / (static_cast<float>(time_to_execute_ms)/1000.0f);
        servo_->move(target_step, target_speed);
        //read current step and decide the status to return
        return Status::SUCCESS;
    }

    void Joint::disable() const {
        servo_->freeze();
    }

    void Joint::freeze() const {
        servo_->freeze();
    }

    void Joint::unfreeze() const {
        servo_->unfreeze();
    }

    void Joint::reset() {
        move_to_default();
    }

    void Joint::throttle() const {
        servo_->throttle();
    }

    void Joint::unthrottle() const {
        servo_->unthrottle();
    }

    const JointReading Joint::poll() {
        const int servo_id = servo_->read_id();
        const float temperature_celsius = servo_->read_temperature_celsius();
        const float load_pct = servo_->read_load_percent();
        const int voltage = servo_->read_voltage_mv();
        const int step = servo_->read_current_step();
        const float angle = servo_->read_current_angle_degrees();
        const float speed_in_steps_per_second = servo_->read_speed_steps_per_second();

        return {
                std::chrono::steady_clock::now(),
                servo_id,
                temperature_celsius,
                load_pct,
                voltage,
                step,
                angle,
                speed_in_steps_per_second
            };
    }




}