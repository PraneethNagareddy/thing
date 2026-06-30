//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_HAND_H
#define THING_HAND_H

#include "Finger.h"
#include "Thumb.h"
#include <span>

namespace anatomy::hand {
    struct FingerMovement {
        Fingers finger;
        std::variant<Flexion, Extension> flex_value;
        float speed;
        float acceleration;
        float start_delay_ms;
    };

    struct ThumbMovement : FingerMovement {
        std::variant<Adduction, Abduction>  spread_value;
        std::variant<Opposition, Reposition> oppose_value;
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

        void apply_single_movement_(std::variant<FingerMovement,ThumbMovement> &movement);
        void execute_finger_movement_(FingerMovement &movement);
        void execute_thumb_movement_(ThumbMovement &movement);

        Finger& get_finger_(const Fingers finger) {
            switch (finger) {
                case Fingers::INDEX:
                    return index_finger_;
                case Fingers::RING:
                    return ring_finger_;
                case Fingers::MIDDLE:
                    return middle_finger_;
                case Fingers::PINKY:
                    return pinky_finger_;
            }
        }

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
        void apply(std::span<std::variant<FingerMovement, ThumbMovement>> movements);
    };
}



#endif //THING_HAND_H