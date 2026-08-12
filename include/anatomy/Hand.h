//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_HAND_H
#define THING_HAND_H

#include "Finger.h"
#include "Thumb.h"
#include "Wrist.h"
#include <memory>
#include <span>
#include <utility>

namespace anatomy::hand {

    enum class Easing {
        LINEAR,
        SINE,
        QUADRATIC,
        CUBIC,
        EXPONENTIAL,
        CIRCULAR
    };

    struct FingerMovement {
        Fingers finger;
        std::variant<Flexion, Extension> flex_value;
        float start_delay_ms = 0.0f;
        Easing easing = Easing::LINEAR;
    };

    struct ThumbMovement : FingerMovement {
        ThumbMovement(std::variant<Flexion, Extension> flex_value,
            std::variant<Adduction, Abduction> spread_value,
            std::variant<Opposition, Reposition> oppose_value,
            float start_delay_ms = 0.0f,
            float start_spread_delay_ms = 0.0f,
            float start_oppose_delay_ms = 0.0f,
            Easing easing = Easing::LINEAR) :
                    FingerMovement(Fingers::THUMB, flex_value, start_delay_ms, easing),
                    spread_value(spread_value),
                    oppose_value(oppose_value),
                    start_spread_delay_ms(start_spread_delay_ms),
                    start_oppose_delay_ms(start_oppose_delay_ms){
        }

        std::variant<Adduction, Abduction>  spread_value;
        std::variant<Opposition, Reposition> oppose_value;
        float start_spread_delay_ms = 0.0f;
        float start_oppose_delay_ms = 0.0f;
    };

    struct WristMovement {
        WristMovement(std::variant<Pitch, std::monostate> pitch_value = std::monostate{},
                      std::variant<Yaw, std::monostate> yaw_value = std::monostate{},
                      float start_delay_ms = 0.0f,
                      Easing easing = Easing::LINEAR)
            : pitch_value(pitch_value),
              yaw_value(yaw_value),
              start_delay_ms(start_delay_ms),
              easing(easing) {}

        std::variant<Pitch, std::monostate> pitch_value;
        std::variant<Yaw, std::monostate> yaw_value;
        float start_delay_ms = 0.0f;
        Easing easing = Easing::LINEAR;
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
        std::shared_ptr<Wrist> wrist_;

        void apply_single_movement_(std::variant<FingerMovement, ThumbMovement, WristMovement> movement);
        void apply_finger_movement_(FingerMovement &movement);
        void apply_thumb_movement_(ThumbMovement &movement);
        void apply_wrist_movement_(WristMovement &movement);

        static void execute_flex(Finger &finger, FingerMovement movement);
        static void execute_spread(Thumb &thumb, ThumbMovement movement);
        static void execute_oppose(Thumb &thumb, ThumbMovement movement);
        static void execute_pitch(Wrist &wrist, WristMovement movement);
        static void execute_yaw(Wrist &wrist, WristMovement movement);

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

        [[nodiscard]] Wrist* get_wrist_() const {
            return wrist_.get();
        }

    public:
        explicit Hand(Side side,
            std::shared_ptr<Finger> pinky,
            std::shared_ptr<Finger> ring_finger,
            std::shared_ptr<Finger> middle_finger,
            std::shared_ptr<Finger> index_finger,
            std::shared_ptr<Thumb> thumb,
            std::shared_ptr<Wrist> wrist = nullptr) :
        side_(side),
        pinky_finger_(std::move(pinky)),
        ring_finger_(std::move(ring_finger)),
        middle_finger_(std::move(middle_finger)),
        index_finger_(std::move(index_finger)),
        thumb_(std::move(thumb)),
        wrist_(std::move(wrist)) {}
        ~Hand() = default;
        void apply(std::span<std::variant<FingerMovement, ThumbMovement, WristMovement>> movements);
    };
}



#endif //THING_HAND_H