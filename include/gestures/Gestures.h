#ifndef THING_GESTURES_H
#define THING_GESTURES_H

#include <vector>
#include <variant>
#include "anatomy/Finger.h"
#include "articulation/Motion.h"
#include "anatomy/Hand.h"

using namespace anatomy::hand;
namespace gestures {

    using Movement = std::variant<FingerMovement, ThumbMovement>;

    class Gestures {
    public:
        /**
         * Pinky Pinch: Curls the pinky to 100% while keeping others open.
         */
        static std::vector<Movement> pinky_pinch() {
            return {
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 100}},
                FingerMovement{Fingers::INDEX, Flexion{0.0f, 200}},
                FingerMovement{Fingers::MIDDLE, Flexion{0.0f, 200}},
                FingerMovement{Fingers::RING, Flexion{0.0f, 200}}
            };
        }

        /**
         * Thumbs Up: Curls all fingers and extends/reposes the thumb.
         */
        static std::vector<Movement> thumbs_up() {
            return {
                FingerMovement{Fingers::INDEX, Flexion{1.0f, 200}},
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 200}},
                FingerMovement{Fingers::RING, Flexion{1.0f, 200}},
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 200}},
                ThumbMovement{
                    Flexion{0.0f, 200}, // Full extend
                    Abduction{0.0f, 200}, // Full extend
                    Opposition{0.0f, 200}, // Full repose
                }
            };
        }
    };

}

#endif //THING_GESTURES_H