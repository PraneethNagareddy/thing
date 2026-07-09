#ifndef THING_EASING_H
#define THING_EASING_H

#include <cmath>
#include <numbers>
#include "../anatomy/Hand.h"

namespace articulation::easing {

    /**
     * Returns an interpolated t based on the smoothing algorithm.
     * @param t Progress from 0.0 to 1.0
     */
    inline float apply(float t, anatomy::hand::Easing type) {
        using namespace anatomy::hand;
        if (t <= 0.0f) return 0.0f;
        if (t >= 1.0f) return 1.0f;

        switch (type) {
            case Easing::LINEAR:
                return t;
            case Easing::QUADRATIC:
                return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
            case Easing::CUBIC:
                return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
            case Easing::SINE:
                return -(std::cos(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
            case Easing::EXPONENTIAL:
                return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
            case Easing::CIRCULAR:
                return t < 0.5f ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f : (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
            default:
                return t;
        }
    }
}

#endif //THING_EASING_H