//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_THUMB_H
#define THING_THUMB_H

#include<articulation/Motion.h>
using namespace articulation;
using namespace articulation::movement;

namespace anatomy::hand {
    class Thumb : public Finger, public ISpreadable ,  public IOpposable {
    private:
        Joint spread_joint_;
        Joint oppose_joint_;
    public:
        Thumb(const Joint &flex_joint, const Joint &spread_joint, const Joint &oppose_joint) :
            Finger(flex_joint),
            spread_joint_(spread_joint),
            oppose_joint_(oppose_joint) {}
        ~Thumb() override = default;

        [[nodiscard]] Status flex(Flexion flexion) const override;
        [[nodiscard]] Status extend(Extension extension) const override;
        [[nodiscard]] Status adduct(Adduction adduction) const override;
        [[nodiscard]] Status abduct(Abduction abduction) const override;
        [[nodiscard]] Status oppose(Opposition opposition) const override;
        [[nodiscard]] Status repose(Reposition reposition) const override;
    };
}
#endif //THING_THUMB_H
