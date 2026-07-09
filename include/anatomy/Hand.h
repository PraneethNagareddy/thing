//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_HAND_H
#define THING_HAND_H

#include "Finger.h"
#include "Thumb.h"
#include <memory>
#include <span>
#include <utility>

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
        std::shared_ptr<Finger> pinky_finger_;
        std::shared_ptr<Finger> ring_finger_;
        std::shared_ptr<Finger> middle_finger_;
        std::shared_ptr<Finger> index_finger_;
        std::shared_ptr<Thumb> thumb_;

        void apply_single_movement_(std::variant<FingerMovement,ThumbMovement> &movement);
        void apply_finger_movement_(FingerMovement &movement);
        void apply_thumb_movement_(ThumbMovement &movement);

        static void execute_flex(Finger &finger, FingerMovement movement);
        static void execute_spread(Thumb &thumb, ThumbMovement movement);
        static void execute_oppose(Thumb &thumb, ThumbMovement movement);

        [[nodiscard]] Finger* get_finger_(const Fingers finger) const {
            switch (finger) {
                case Fingers::INDEX:
                    return index_finger_.get();
                case Fingers::RING:
                    return ring_finger_.get();
                case Fingers::MIDDLE:
                    return middle_finger_.get();
                case Fingers::PINKY:
                    return pinky_finger_.get();
                case Fingers::THUMB:
                    return thumb_.get();
            }
            return nullptr;
        }

        [[nodiscard]] Thumb* get_thumb_() const {
            return thumb_.get();
        }

    public:
        explicit Hand(Side side,
            std::shared_ptr<Finger> pinky,
            std::shared_ptr<Finger> ring_finger,
            std::shared_ptr<Finger> middle_finger,
            std::shared_ptr<Finger> index_finger,
            std::shared_ptr<Thumb> thumb) :
        side_(side),
        pinky_finger_(std::move(pinky)),
        ring_finger_(std::move(ring_finger)),
        middle_finger_(std::move(middle_finger)),
        index_finger_(std::move(index_finger)),
        thumb_(std::move(thumb)) {}
        ~Hand() = default;
        void apply(std::span<std::variant<FingerMovement, ThumbMovement>> movements);
    };
}



#endif //THING_HAND_H