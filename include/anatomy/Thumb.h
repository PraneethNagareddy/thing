//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_THUMB_H
#define THING_THUMB_H

namespace anatomy::hand {
    class Thumb : public Finger, public articulation::ISpreadable ,  public articulation::IOpposable {
    private:
        articulation::Joint spread_joint_;
        articulation::Joint oppose_joint_;
    public:
        Thumb(const articulation::Joint &flex_joint, const articulation::Joint &spread_joint, const articulation::Joint &oppose_joint) :
            Finger(flex_joint),
            spread_joint_(spread_joint),
            oppose_joint_(oppose_joint) {}
        ~Thumb() override = default;

        articulation::movement::Status flex(float movement_percentage) override;
        articulation::movement::Status extend(float movement_percentage) override;
        articulation::movement::Status adduct(float movement_percentage) override;
        articulation::movement::Status abduct(float movement_percentage) override;
        articulation::movement::Status oppose(float movement_percentage) override;
        articulation::movement::Status repose(float movement_percentage) override;
    };
}
#endif //THING_THUMB_H
