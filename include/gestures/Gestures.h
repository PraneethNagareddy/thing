#ifndef THING_GESTURES_H
#define THING_GESTURES_H

#include <vector>
#include <variant>
#include <cmath>
#include "anatomy/Finger.h"
#include "articulation/Motion.h"
#include "anatomy/Hand.h"

using namespace anatomy::hand;
namespace gestures {

    using Movement = std::variant<FingerMovement, ThumbMovement, WristMovement>;

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

        /**
         * Grab Object: Pinch-grabs an object of specified size (in mm).
         * Calculates index finger and thumb joint positions so that the pinch leaves a gap
         * matching the object size (with a slight grip margin to securely hold the object).
         *
         * @param size_mm Target object size/width in millimeters.
         * @param grip_margin_mm Margin subtracted from size_mm for firm grip contact (default: 2.0mm).
         * @param time_ms Movement execution time in milliseconds (default: 1000ms).
         * @return Vector of Movements for pinching an object of the specified size.
         */
        static std::vector<Movement> grab_object(const float size_mm, const float grip_margin_mm = 2.0f, const int time_ms = 1000) {
            constexpr float MIN_SIZE_MM = 0.0f;
            constexpr float MAX_SIZE_MM = 60.0f; // Maximum pinch aperture in mm

            // Baseline values at 0mm object size (0mm pinch gap)
            constexpr float INDEX_FLEX_0MM = 0.865f;
            constexpr float THUMB_FLEX_0MM = 0.630f;
            constexpr float THUMB_ABD_0MM  = 0.530f;
            constexpr float THUMB_OPP_0MM  = 0.800f;

            // Target values at max object size (MAX_SIZE_MM opening)
            constexpr float INDEX_FLEX_MAX = 0.200f;
            constexpr float THUMB_FLEX_MAX = 0.150f;
            constexpr float THUMB_ABD_MAX  = 0.250f;
            constexpr float THUMB_OPP_MAX  = 0.500f;

            // Apply grip margin so the hand grips the object slightly tighter than its exact size (unless size is 0mm)
            float effective_gap_mm = size_mm > 0.0f ? std::max(0.0f, size_mm - grip_margin_mm) : 0.0f;

            // Clamp effective gap between MIN_SIZE_MM and MAX_SIZE_MM
            effective_gap_mm = std::clamp(effective_gap_mm, MIN_SIZE_MM, MAX_SIZE_MM);

            // Calculate interpolation factor t in [0.0, 1.0]
            float t = (effective_gap_mm - MIN_SIZE_MM) / (MAX_SIZE_MM - MIN_SIZE_MM);

            // Interpolate finger and thumb flexion/position parameters
            float index_flex  = INDEX_FLEX_0MM + t * (INDEX_FLEX_MAX - INDEX_FLEX_0MM);
            float thumb_flex  = THUMB_FLEX_0MM + t * (THUMB_FLEX_MAX - THUMB_FLEX_0MM);
            float thumb_abd   = THUMB_ABD_0MM  + t * (THUMB_ABD_MAX  - THUMB_ABD_0MM);
            float thumb_opp   = THUMB_OPP_0MM  + t * (THUMB_OPP_MAX  - THUMB_OPP_0MM);

            return {
                FingerMovement{Fingers::MIDDLE, Extension{1.0f, time_ms}, 300},
                FingerMovement{Fingers::RING, Extension{1.0f, time_ms}, 500},
                FingerMovement{Fingers::PINKY, Extension{1.0f, time_ms}, 500},
                FingerMovement{Fingers::INDEX, Flexion{index_flex, time_ms}, 100},
                ThumbMovement{
                    Flexion{thumb_flex, time_ms},
                    Abduction{thumb_abd, time_ms},
                    Opposition{thumb_opp, time_ms},
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
                },
                WristMovement{
                    Pitch{0.5f, 1000},
                    Yaw{0.5f, 1000},
                    100
                }
            };
        }

        static std::vector<Movement> spidey_web() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Extension{1.0f, 1000}, 500},
                FingerMovement{Fingers::INDEX, Extension{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{1.0f, 1000}, // Full extend
                    Abduction{1.0f, 1000}, // Full extend
                    Reposition{1.0f, 1000}, // Full repose
                    200,
                    300,
                    400
                },
                WristMovement{
                    Pitch{1.0f, 1000},
                    Yaw{0.5f, 1000},
                    100
                }
            };
        }

        static std::vector<Movement> fist_rotate() {
            return {
                /*FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 100},*/
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 500},
                FingerMovement{Fingers::INDEX, Flexion{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{0.2f, 1000}, // Full extend
                    Abduction{0.2f, 1000}, // Full extend
                    Opposition{1.0f, 1000}, // Full repose
                    500,
                    300,
                    400
                },
                WristMovement{
                    Pitch{0.5f, 1000},
                    Yaw{0.0f, 1000},
                    1000
                },
                WristMovement{
                    Pitch{0.0f, 500},
                    Yaw{0.5f, 500},
                    1000
                },
                WristMovement{
                    Pitch{0.5f, 500},
                    Yaw{1.0f, 500},
                    1500
                },
                WristMovement{
                    Pitch{1.0f, 500},
                    Yaw{0.5f, 500},
                    2000
                },
                WristMovement{
                    Pitch{0.5f, 500},
                    Yaw{0.0f, 500},
                    2500
                },
                WristMovement{
                    Pitch{0.5f, 500},
                    Yaw{0.0f, 500},
                    3000
                }
            };
        }

        /**
         * Streams a continuous rotation of the wrist in 2DOF space (Pitch and Yaw)
         * over 360 degrees, completing a full circle.
         */
        static anatomy::hand::MovementStream rotate_wrist_stream() {
            using namespace anatomy::hand;
            using namespace articulation::movement;
            
            anatomy::hand::MovementStream stream;

            std::vector<MovementVariant> fistGroup = {
                FingerMovement{Fingers::MIDDLE, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::RING, Flexion{1.0f, 1000}, 100},
                FingerMovement{Fingers::PINKY, Flexion{1.0f, 1000}, 500},
                FingerMovement{Fingers::INDEX, Flexion{1.0f, 1000}, 100},
                ThumbMovement{
                    Extension{0.2f, 1000}, // Full extend
                    Abduction{0.2f, 1000}, // Full extend
                    Opposition{1.0f, 1000}, // Full repose
                    200,
                    300,
                    400
                }
            };

            stream.push_back(fistGroup);
            
            // 36 steps of 10 degrees each to make a full 360 circle
            const int num_steps = 36;
            const int time_per_step_ms = 10; // fast fluid motion

            for (int i = 0; i <= num_steps; ++i) {
                float angle_rad = (static_cast<float>(i) / num_steps) * 2.0f * M_PI;
                
                // sin and cos give -1.0 to 1.0, map to 0.0 to 1.0 range
                float pitch_pct = (std::sin(angle_rad) + 1.0f) / 2.0f;
                float yaw_pct   = (std::cos(angle_rad) + 1.0f) / 2.0f;

                std::vector<MovementVariant> group = {
                    WristMovement{
                        Pitch{pitch_pct, time_per_step_ms},
                        Yaw{yaw_pct, time_per_step_ms},
                        0 // no delay
                    }
                };
                
                stream.push_back(group);
            }
            
            return stream;
        }
    };
}

#endif //THING_GESTURES_H