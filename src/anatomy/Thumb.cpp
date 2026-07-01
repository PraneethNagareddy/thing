//
// Created by Nagareddy on 02/07/26.
//

#include "anatomy/Thumb.h"
#include "articulation/Joint.h"

using namespace articulation;
using namespace articulation::movement;

namespace anatomy::hand {

    Status Thumb::flex(Flexion flexion) {
        return flex_joint_.move_to(flexion.percentage);
    }
    Status Thumb::extend(Extension extension) {
        return flex_joint_.move_to(1.0f-extension.percentage);
    }
    Status Thumb::adduct(Adduction adduction) {
        return spread_joint_.move_to(adduction.percentage);
    }
    Status Thumb::abduct(Abduction abduction) {
        return spread_joint_.move_to(1.0f-abduction.percentage);
    }
    Status Thumb::oppose(Opposition opposition) {
        return oppose_joint_.move_to(opposition.percentage);
    }
    Status Thumb::repose(Reposition reposition) {
        return oppose_joint_.move_to(1.0f-reposition.percentage);
    }

}