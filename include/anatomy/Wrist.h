//
// Created for Robotic Hand Wrist Anatomy
//

#ifndef THING_WRIST_H
#define THING_WRIST_H

#include <memory>
#include <utility>
#include "articulation/Joint.h"
#include "articulation/Motion.h"

namespace anatomy::hand {

    class Wrist : public articulation::IPitchable, public articulation::IYawable {
    private:
        std::shared_ptr<articulation::Joint> pitch_joint_; // ID 11 (Pitch)
        std::shared_ptr<articulation::Joint> yaw_joint_;   // ID 12 (Yaw)

    public:
        Wrist(std::shared_ptr<articulation::Joint> pitch_joint,
              std::shared_ptr<articulation::Joint> yaw_joint)
            : pitch_joint_(std::move(pitch_joint)),
              yaw_joint_(std::move(yaw_joint)) {}

        ~Wrist() override = default;

        [[nodiscard]] articulation::movement::Status pitch(articulation::movement::Pitch pitch) override;
        [[nodiscard]] articulation::movement::Status yaw(articulation::movement::Yaw yaw) override;

        [[nodiscard]] articulation::Joint* get_pitch_joint() const { return pitch_joint_.get(); }
        [[nodiscard]] articulation::Joint* get_yaw_joint() const { return yaw_joint_.get(); }
    };

}

#endif //THING_WRIST_H
