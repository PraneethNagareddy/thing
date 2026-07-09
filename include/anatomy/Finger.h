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

    inline std::string to_string(Fingers finger) {
        switch (finger) {
            case Fingers::PINKY:
                return "PINKY";
            case Fingers::RING:
                return "RING";
            case Fingers::INDEX:
                return "INDEX";
            case Fingers::MIDDLE:
                return "MIDDLE";
            case Fingers::THUMB:
                return "THUMB";
            default:
                return "UNKNOWN_FINGER"; // Handle any unlisted values
                return "UNKNOWN_FINGER";
        }
    }

    class Finger : articulation::IFlexible {
    protected:
        Fingers finger_;
        std::shared_ptr<articulation::Joint> flex_joint_;
    public:
        explicit Finger(Fingers finger, std::shared_ptr<articulation::Joint> joint) :
            finger_(finger),
            flex_joint_(std::move(joint)) {}
        ~Finger() override = default;

        [[nodiscard]] articulation::movement::Status flex(articulation::movement::Flexion flexion) override;
        [[nodiscard]] articulation::movement::Status extend(articulation::movement::Extension extension) override;
    };
}

#endif //THING_FINGER_H
