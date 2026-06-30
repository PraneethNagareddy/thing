//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_READING_H
#define THING_READING_H

#include <chrono>

namespace telemetry {
    struct BaseReading {
    protected:
        explicit BaseReading(std::chrono::steady_clock::time_point timestamp) : timestamp(timestamp) {}
    public:
        std::chrono::steady_clock::time_point timestamp;
    private:
    };

    struct JointReading : BaseReading {
        int servo_id{};
        float temperature{};
        float load_percentage{};
        int voltage{};
        int current_step{};
        int current_angle{};
        int current_speed{};
    };
}

#endif //THING_READING_H
