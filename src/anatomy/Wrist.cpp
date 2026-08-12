#include "anatomy/Wrist.h"

using namespace articulation;
using namespace articulation::movement;

namespace anatomy::hand {

    Status Wrist::pitch(Pitch pitch) {
        return pitch_joint_->move_to(pitch.percentage, pitch.time_to_execute_ms);
    }

    Status Wrist::yaw(Yaw yaw) {
        return yaw_joint_->move_to(yaw.percentage, yaw.time_to_execute_ms);
    }

}
