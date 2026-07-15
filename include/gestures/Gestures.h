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
                FingerMovement{Fingers::PINKY, Flexion{0.0f, 1000}, 100},
                //FingerMovement{Fingers::INDEX, Flexion{0.0f, 1000}},
                FingerMovement{Fingers::MIDDLE, Flexion{0.0f, 1000}, 500},
                FingerMovement{Fingers::RING, Flexion{0.0f, 1000}, 2000}
            };
        }

        /**
         * Thumbs Up: Curls all fingers and extends/reposes the thumb.
         */
        static std::vector<Movement> thumbs_up() {
            return {
                //FingerMovement{Fingers::INDEX, Flexion{1.0f, 200}},
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},
                ThumbMovement{
                    Flexion{0.0f, 200}, // Full extend
                    Abduction{0.0f, 200}, // Full extend
                    Opposition{0.0f, 200}, // Full repose
                }
            };
        }

        /**
         * Reset All: Returns all fingers and the thumb to their default (0% flexion) positions.
         */
        static std::vector<Movement> reset_all() {
            using namespace anatomy::hand;
            using namespace articulation::movement;
            return {
                //FingerMovement{Fingers::INDEX,  Flexion{0.0f, 1000}},
                FingerMovement{Fingers::MIDDLE, Flexion{0.0f, 1000}},
                FingerMovement{Fingers::RING,   Flexion{0.0f, 1000}},
                FingerMovement{Fingers::PINKY,  Flexion{0.0f, 1000}},
                ThumbMovement{Flexion{0.0f, 1000}, Adduction{0.0f, 1000}, Opposition{0.0f, 1000}}
            };
        }
    };

}

#endif //THING_GESTURES_H