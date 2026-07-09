//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_JOINTS_H
#define THING_JOINTS_H
#include <memory>
#include "Motion.h"
#include "hardware/SCS0009Servo.h"
#include "telemetry/IMonitorable.h"
namespace articulation {
    class Joint : public telemetry::IMonitorable<telemetry::JointReading> {
    private:
        std::shared_ptr<hardware::SCS0009Servo> servo_;
        float default_angle_;
        float current_angle_;
        float no_flex_angle_ ;
        float full_flex_angle_ ;
        float max_speed_;

    public:
        Joint(std::shared_ptr<hardware::SCS0009Servo> servo,
            float default_angle,
            float no_flex_angle,
            float full_flex_angle,
            float max_speed) :
        servo_(std::move(servo)),
        default_angle_(default_angle),
        no_flex_angle_(no_flex_angle),
        full_flex_angle_(full_flex_angle),
        current_angle_(default_angle),
        max_speed_(max_speed){}

        movement::Status move_to(float movement_percent, int time_to_execute_ms);
        const telemetry::JointReading poll() override;
    };
}
#endif //THING_JOINTS_H
