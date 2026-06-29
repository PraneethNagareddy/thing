//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_MOTION_H
#define THING_MOTION_H

namespace articulation {
    namespace movement {
        enum class Status {
            SUCCESS,
            STOPPED_BOUNDS_REACHED,
            STOPPED_FROZEN,
            ERROR_HARDWARE,
            ERROR_UNKNOWN,
        };
    }

    class IFlexible {
    public:
        virtual ~IFlexible() = default;
        virtual movement::Status flex(float movement_percentage) = 0;
        virtual movement::Status extend(float movement_percentage) = 0;
    };

    class ISpreadable {
    public:
        virtual ~ISpreadable() = default;
        virtual movement::Status adduct(float movement_percentage) = 0;
        virtual movement::Status abduct(float movement_percentage) = 0;
    };

    class IOpposable {
    public:
        virtual ~IOpposable() = default;
        virtual movement::Status oppose(float movement_percentage) = 0;
        virtual movement::Status repose(float movement_percentage) = 0;
    };
}


#endif //THING_MOTION_H
