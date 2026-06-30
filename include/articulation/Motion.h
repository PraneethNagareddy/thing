//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_MOTION_H
#define THING_MOTION_H
#include <cassert>

namespace articulation {
    namespace movement {
        enum class Status {
            SUCCESS,
            STOPPED_BOUNDS_REACHED,
            STOPPED_FROZEN,
            ERROR_HARDWARE,
            ERROR_UNKNOWN,
        };

        struct MovementValue {
        protected:
            explicit MovementValue(float percentage) : percentage(percentage) {
                assert(percentage >= 0.0f && percentage <= 1.0f && "Move value should be a percentage value between 0.0f anf 1.0");
            }
        public:
            float percentage;
        private:
            MovementValue() = default;
        };

        struct Flexion : MovementValue {
            explicit Flexion(float percent) : MovementValue(percent) {}
        };
        struct Extension : MovementValue {
            explicit Extension(float percent) : MovementValue(percent) {}
        };
        struct Adduction : MovementValue {
            explicit Adduction(float percent) : MovementValue(percent) {}
        };
        struct Abduction : MovementValue {
            explicit Abduction(float percent) : MovementValue(percent) {}
        };
        struct Opposition : MovementValue {
            explicit Opposition(float percent) : MovementValue(percent) {}
        };
        struct Reposition : MovementValue {
            explicit Reposition(float percent) : MovementValue(percent) {}
        };
    }

    class IFlexible {
    public:
        virtual ~IFlexible() = default;
        [[nodiscard]] virtual movement::Status flex(movement::Flexion flexion) const = 0;
        [[nodiscard]] virtual movement::Status extend(movement::Extension extension) const = 0;
    };

    class ISpreadable {
    public:
        virtual ~ISpreadable() = default;
        [[nodiscard]] virtual movement::Status adduct(movement::Adduction adduction) const = 0;
        [[nodiscard]] virtual movement::Status abduct(movement::Abduction abduction) const  = 0;
    };

    class IOpposable {
    public:
        virtual ~IOpposable() = default;
        [[nodiscard]] virtual movement::Status oppose(movement::Opposition opposition) const = 0;
        [[nodiscard]] virtual movement::Status repose(movement::Reposition reposition) const = 0;
    };
}


#endif //THING_MOTION_H
