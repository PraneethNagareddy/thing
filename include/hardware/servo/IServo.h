#ifndef THING_ISERVO_H
#define THING_ISERVO_H

#include <cstdint>
#include <string>

namespace hardware {

    class IServo {
    public:
        virtual ~IServo() = default;
        virtual void move(int target_step, float target_speed_steps_per_sec) = 0;
        virtual const float read_current_angle_degrees() = 0;
        virtual const int read_current_step() = 0;
        virtual const float read_load_percent() = 0;
        virtual const float read_temperature_celsius() = 0;
        virtual const int read_voltage_mv() = 0;
        virtual const float read_speed_steps_per_second() = 0;
        virtual const int read_id() = 0;
        virtual void freeze() = 0;
        virtual void unfreeze() = 0;
        virtual void throttle() = 0;
        virtual void unthrottle() = 0;
        virtual const bool is_moving(long poll_interval_ms = 50, uint16_t threshold = 5) = 0;
        virtual int deg_to_steps(float deg) const = 0;
    };

}

#endif //THING_ISERVO_H
