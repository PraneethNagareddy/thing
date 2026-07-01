//
// Created by Nagareddy on 29/06/26.
//

#include "anatomy/Finger.h"
#include "articulation/Joint.h"

using namespace articulation;
using namespace articulation::movement;

namespace anatomy::hand {

    Status Finger::flex(Flexion flexion) {
        return flex_joint_.move_to(flexion.percentage, flexion.time_to_execute_ms);
    }

    Status Finger::extend(Extension extension) {
        return flex_joint_.move_to(1.0f-extension.percentage, extension.time_to_execute_ms);
    }

}