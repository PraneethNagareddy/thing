//
// Created by Nagareddy on 29/06/26.
//

#include "anatomy/Finger.h"
#include "articulation/Joint.h"

namespace anatomy::hand {

    articulation::movement::Status Finger::flex(float movement_percentage) {
        // Implementation for flexing the finger
        // This would typically involve calculating the target position for the joint
        return flex_joint_.move(movement_percentage);
    }

    articulation::movement::Status Finger::extend(float movement_percentage) {
        // Implementation for extending the finger
        // Extending is often the inverse of flexing
        return flex_joint_.move(-movement_percentage);
    }

}