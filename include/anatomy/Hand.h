//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_HAND_H
#define THING_HAND_H

#include "Finger.h"
#include "Thumb.h"
#include <span>

namespace anatomy::hand {
    struct Movement {
        int servo_id;
        float to_location;
        float speed;
        float acceleration;
        float start_delay;
    };

    enum class Side {
        LEFT,
        RIGHT
    };

    class Hand {
    private:
        Side side_;
        Finger pinky_finger_;
        Finger ring_finger_;
        Finger middle_finger_;
        Finger index_finger_;
        Thumb thumb_;
    public:
        explicit Hand(const Side side,
                      const Finger &pinky,
                      const Finger &ring_finger,
                      const Finger &middle_finger,
                      const Finger &index_finger,
                      const Thumb &thumb) :
        side_(side),
        pinky_finger_(pinky),
        ring_finger_(ring_finger),
        middle_finger_(middle_finger),
        index_finger_(index_finger),
        thumb_(thumb){}
        ~Hand() = default;
        void apply(std::span<Movement> movements);
    };
}



#endif //THING_HAND_H