//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_JOINTS_H
#define THING_JOINTS_H
#include "Motion.h"
#include "hardware/Servo.h"
#include "telemetry/IMonitorable.h"
namespace articulation {
    class Joint : public telemetry::IMonitorable<telemetry::JointReading> {
    private:
        hardware::Servo& servo_;
        float default_angle_;
        float current_angle_;
        float no_flex_angle_ ;
        float full_flex_angle_ ;
        float max_speed_;
        int8_t total_steps_;

    public:
        Joint(hardware::Servo &servo,
            float default_angle,
            float no_flex_angle,
            float full_flex_angle,
            int8_t total_steps) :
        servo_(servo),
        default_angle_(default_angle),
        no_flex_angle_(no_flex_angle),
        full_flex_angle_(full_flex_angle),
        current_angle_(default_angle),
        total_steps_(total_steps){}

        movement::Status move_to(float movement_percent, int time_to_execute_ms);
        const telemetry::JointReading poll() override;
    };
}
#endif //THING_JOINTS_H
