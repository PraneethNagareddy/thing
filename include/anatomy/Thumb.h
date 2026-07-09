//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_THUMB_H
#define THING_THUMB_H

#include<articulation/Motion.h>

#include "Finger.h"
using namespace articulation;
using namespace articulation::movement;

namespace anatomy::hand {
    class Thumb : public Finger, public ISpreadable ,  public IOpposable {
    private:
        std::shared_ptr<Joint> spread_joint_;
        std::shared_ptr<Joint> oppose_joint_;
    public:
        Thumb(std::shared_ptr<Joint> flex_joint, std::shared_ptr<Joint> spread_joint, std::shared_ptr<Joint> oppose_joint) :
            Finger(Fingers::THUMB, std::move(flex_joint)),
            spread_joint_(std::move(spread_joint)),
            oppose_joint_(std::move(oppose_joint)) {}
        ~Thumb() override = default;

        [[nodiscard]] Status flex(Flexion flexion) override;
        [[nodiscard]] Status extend(Extension extension) override;
        [[nodiscard]] Status adduct(Adduction adduction) override;
        [[nodiscard]] Status abduct(Abduction abduction) override;
        [[nodiscard]] Status oppose(Opposition opposition) override;
        [[nodiscard]] Status repose(Reposition reposition) override;
    };
}
#endif //THING_THUMB_H
