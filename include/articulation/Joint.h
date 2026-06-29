//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_JOINTS_H
#define THING_JOINTS_H
#include "hardware/Servo.h"
#include "telemetry/IMonitorable.h"
namespace articulation {
    class Joint : public telemetry::IMonitorable<telemetry::JointReading> {
    private:
        hardware::Servo& servo_;
        float default_position_;
        float current_position_;
        float no_flex_position_ ;
        float full_flex_position_ ;
    public:
        Joint(hardware::Servo &servo,
            float default_position,
            float no_flex_position,
            float full_flex_position) :
        servo_(servo),
        default_position_(default_position),
        no_flex_position_(no_flex_position),
        full_flex_position_(full_flex_position),
        current_position_(default_position) {}

        void move(float movement_percent);
        telemetry::JointReading poll() override;
    };
}
#endif //THING_JOINTS_H
