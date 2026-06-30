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
        PINKY
    };

    class Finger : articulation::IFlexible {
    private:
        articulation::Joint flex_joint_;
    public:
        explicit Finger(const articulation::Joint &joint) : flex_joint_(joint) {}
        ~Finger() override = default;

        [[nodiscard]] articulation::movement::Status flex(articulation::movement::Flexion flexion) const override;
        [[nodiscard]] articulation::movement::Status extend(articulation::movement::Extension extension) const override;
    };
}

#endif //THING_FINGER_H
