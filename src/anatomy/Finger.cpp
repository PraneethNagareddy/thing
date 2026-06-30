//
// Created by Nagareddy on 29/06/26.
//

#include "anatomy/Finger.h"
#include "articulation/Joint.h"

namespace anatomy::hand {

    articulation::movement::Status Finger::flex(float movement_percentage) {
        return flex_joint_.move(movement_percentage);
    }

    articulation::movement::Status Finger::extend(float movement_percentage) {
        return flex_joint_.move(-movement_percentage);
    }

}