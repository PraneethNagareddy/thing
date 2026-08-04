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
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 300},
                FingerMovement{Fingers::MIDDLE, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::RING, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::PINKY, Flexion{0.75f, 1000}, 100},
                ThumbMovement{
                    Flexion{1.0f, 1000}, // Full extend
                    Abduction{0.5f, 1000}, // Full extend
                    Opposition{1.0f, 1000},// Full Oppose
                    200,
                    300,
                    400
                }
            };
        }

        static std::vector<Movement> ring_pinch() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 300},
                FingerMovement{Fingers::MIDDLE, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::PINKY, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::RING, Flexion{0.8f, 1000}, 100},
                ThumbMovement{
                    Flexion{0.8f, 1000}, // Full extend
                    Abduction{0.4f, 1000}, // Full extend
                    Opposition{1.0f, 1000},// Full Oppose
                    200,
                    300,
                    400
                }
            };
        }

        static std::vector<Movement> middle_pinch() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 300},
                FingerMovement{Fingers::RING, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::PINKY, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::MIDDLE, Flexion{0.8f, 1000}, 100},
                ThumbMovement{
                    Flexion{0.63f, 1000}, // Full extend
                    Abduction{0.47f, 1000}, // Full extend
                    Opposition{1.0f, 1000},// Full Oppose
                    200,
                    300,
                    400
                }
            };
        }

        static std::vector<Movement> index_pinch() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::MIDDLE, Extension{1.0f, 1000}, 300},
                FingerMovement{Fingers::RING, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::PINKY, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::INDEX, Flexion{0.865f, 1000}, 100},
                ThumbMovement{
                    Flexion{0.63f, 1000}, // Full extend
                    Abduction{0.53f, 1000}, // Full extend
                    Opposition{0.8f, 1000},// Full Oppose
                    200,
                    300,
                    400
                }
            };
        }

        static std::vector<Movement> finger_gun() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 1000}, 300},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 500},
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 500},
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{1.0f, 1000}, // Full extend
                    Abduction{1.0f, 1000}, // Full extend
                    Reposition{1.0f, 1000}, // Full repose
                    200,
                    300,
                    400
                }
            };
        }

        static std::vector<Movement> rock_on() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{0.2f, 1000}, // Full extend
                    Abduction{0.4f, 1000}, // Full extend
                    Opposition{1.0f, 1000}, // Full repose
                    500,
                    300,
                    400
                }
            };
        }

        static std::vector<Movement> peace() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::MIDDLE, Extension{1.0f, 1000}, 100},
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{0.2f, 1000}, // Full extend
                    Abduction{0.2f, 1000}, // Full extend
                    Opposition{1.0f, 1000}, // Full repose
                    500,
                    300,
                    400
                }
            };
        }

        /**
         * Thumbs Up: Curls all fingers and extends/reposes the thumb.
         */
        static std::vector<Movement> thumbs_up() {
            return {
                FingerMovement{Fingers::INDEX, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{1.0f, 1000}, // Full extend
                    Abduction{1.0f, 1000}, // Full extend
                    Reposition{1.0f, 1000}, // Full repose
                    200,
                    300,
                    400
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
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 100},
                FingerMovement{Fingers::MIDDLE, Extension{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Extension{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Extension{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{1.0f, 1000}, // Full extend
                    Abduction{1.0f, 1000}, // Full extend
                    Reposition{1.0f, 1000}, // Full repose
                    200,
                    300,
                    400
                }
            };
        }
    };
}

#endif //THING_GESTURES_H