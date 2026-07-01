//
// Created by Nagareddy on 02/07/26.
//

#include "anatomy/Thumb.h"
#include "articulation/Joint.h"

using namespace articulation;
using namespace articulation::movement;
using namespace telemetry;

namespace articulation {
    const JointReading Joint::poll() {
        const int servo_id = servo_.read_id();
        const float temperature_celsius = servo_.read_temperature_celsius();
        const float load_pct = servo_.read_load_percent();
        const int voltage = servo_.read_voltage_mv();
        const int step = servo_.read_current_step();
        const float angle = servo_.read_current_angle_degrees();
        const float speed_in_steps_per_second = servo_.read_speed_steps_per_second();

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