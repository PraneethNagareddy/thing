//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_FINGER_H
#define THING_FINGER_H

#include "../articulation/Joint.h"
#include "../articulation/Motion.h"


namespace anatomy::hand {
    class Finger : articulation::IFlexible {
    private:
        articulation::Joint flex_joint_;
    public:
        explicit Finger(const articulation::Joint &joint) : flex_joint_(joint) {}
        ~Finger() override = default;

        articulation::movement::Status flex(float movement_percentage) override;
        articulation::movement::Status extend(float movement_percentage) override;
    };
}

#endif //THING_FINGER_H
