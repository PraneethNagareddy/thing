//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_MOTION_H
#define THING_MOTION_H
#include <cassert>
#include <iostream>

namespace articulation {
    namespace movement {
        enum class Status {
            SUCCESS,
            STOPPED_BOUNDS_REACHED,
            STOPPED_FROZEN,
            ERROR_HARDWARE,
            ERROR_UNKNOWN,
        };

        inline std::ostream& operator<<(std::ostream& os, Status status) {
            switch (status) {
                case Status::SUCCESS:
                    os << "SUCCESS";
                    break;
                case Status::STOPPED_BOUNDS_REACHED:
                    os << "STOPPED_BOUNDS_REACHED";
                    break;
                case Status::STOPPED_FROZEN:
                    os << "STOPPED_FROZEN";
                    break;
                case Status::ERROR_HARDWARE:
                    os << "ERROR_HARDWARE";
                    break;
                case Status::ERROR_UNKNOWN:
                    os << "ERROR_UNKNOWN";
                    break;
                default:
                    os << "UNKNOWN_STATUS"; // Handle any unlisted values
                    break;
            }
            return os;
        }

        struct MovementValue {
        protected:
            explicit MovementValue(float percentage, int time_ms) : percentage(percentage), time_to_execute_ms(time_ms) {
                assert(percentage >= 0.0f && percentage <= 1.0f && "Move value should be a percentage value between 0.0f anf 1.0");
            }
        public:
            float percentage;
            int time_to_execute_ms;
        private:
            MovementValue() = default;
        };

        struct Flexion : MovementValue {
            explicit Flexion(float percentage, int time_ms) : MovementValue(percentage, time_ms) {}
        };
        struct Extension : MovementValue {
            explicit Extension(float percentage, int time_ms) : MovementValue(percentage, time_ms) {}
        };
        struct Adduction : MovementValue {
            explicit Adduction(float percentage, int time_ms) : MovementValue(percentage, time_ms) {}
        };
        struct Abduction : MovementValue {
            explicit Abduction(float percentage, int time_ms) : MovementValue(percentage, time_ms) {}
        };
        struct Opposition : MovementValue {
            explicit Opposition(float percentage, int time_ms) : MovementValue(percentage, time_ms) {}
        };
        struct Reposition : MovementValue {
            explicit Reposition(float percentage, int time_ms) : MovementValue(percentage, time_ms) {}
        };
    }

    class IFlexible {
    public:
        virtual ~IFlexible() = default;
        [[nodiscard]] virtual movement::Status flex(movement::Flexion flexion) = 0;
        [[nodiscard]] virtual movement::Status extend(movement::Extension extension) = 0;
    };

    class ISpreadable {
    public:
        virtual ~ISpreadable() = default;
        [[nodiscard]] virtual movement::Status adduct(movement::Adduction adduction) = 0;
        [[nodiscard]] virtual movement::Status abduct(movement::Abduction abduction)  = 0;
    };

    class IOpposable {
    public:
        virtual ~IOpposable() = default;
        [[nodiscard]] virtual movement::Status oppose(movement::Opposition opposition) = 0;
        [[nodiscard]] virtual movement::Status repose(movement::Reposition reposition) = 0;
    };
}


#endif //THING_MOTION_H
