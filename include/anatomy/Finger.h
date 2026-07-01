//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_FINGER_H
#define THING_FINGER_H

#include "../articulation/Joint.h"
#include "../articulation/Motion.h"


namespace anatomy::hand {

    enum class Fingers {
        INDEX,
        MIDDLE,
        RING,
        PINKY,
        THUMB
    };

    inline std::ostream& operator<<(std::ostream& os, Fingers finger) {
        switch (finger) {
            case Fingers::PINKY:
                os << "PINKY";
                break;
            case Fingers::RING:
                os << "RING";
                break;
            case Fingers::INDEX:
                os << "INDEX";
                break;
            case Fingers::MIDDLE:
                os << "MIDDLE";
                break;
            case Fingers::THUMB:
                os << "THUMB";
                break;
            default:
                os << "UNKNOWN_FINGER"; // Handle any unlisted values
                break;
        }
        return os;
    }

    class Finger : articulation::IFlexible {
    protected:
        articulation::Joint flex_joint_;
    public:
        explicit Finger(const articulation::Joint &joint) : flex_joint_(joint) {}
        ~Finger() override = default;

        [[nodiscard]] articulation::movement::Status flex(articulation::movement::Flexion flexion) override;
        [[nodiscard]] articulation::movement::Status extend(articulation::movement::Extension extension) override;
    };
}

#endif //THING_FINGER_H
