//
// Created by Nagareddy on 08/07/26.
//


#include "builder/HandBuilder.h"
#include "articulation/Joint.h"
#include "anatomy/Finger.h"
#include "articulation/Constants.h"
#include "hardware/HardwareConstants.h"
#include "telemetry/alert/AlertHandler.h"
#include "telemetry/TelemetryManager.h"


using namespace builder;
namespace builder {


     // void HandBuilder::configure_safety_protocols() {
     //     using namespace telemetry::alert;
     //
     //     auto emergency_stop_cb = [this](const Alert& alert) {
     //         if (alert.reading) {
     //             // Direct hardware kill switch for the specific servo ID
     //             hardware_->set_torque_enable(alert.reading->monitorable_id, false);
     //         }
     //     };
     //
     //     // Link safety actions to hardware execution
     //     AlertHandler::getInstance().register_action_handler(SuggestedAction::FREEZE, emergency_stop_cb);
     //     AlertHandler::getInstance().register_action_handler(SuggestedAction::SHUTDOWN, emergency_stop_cb);
     // }

     std::shared_ptr<Joint> HandBuilder::create_monitored_joint(
         const std::string& name, uint8_t id, float no_flex_angle, float full_flex_angle, float default_angle) {
         auto servo = std::make_shared<hardware::SCS0009Servo>(id, name, hardware::constants::DEFAULT_SCS0009_PROTOCOL);
         auto joint = std::make_shared<Joint>(servo, id, no_flex_angle, full_flex_angle, default_angle);
         // Wire into the Telemetry system
         telemetry::TelemetryManager<telemetry::JointReading>::register_monitorable(id, joint.get());
         return joint;
     }

     std::shared_ptr<anatomy::hand::Finger> HandBuilder::build_standard_finger(anatomy::hand::Fingers fingerType, uint8_t id, float no_flex_angle, float full_flex_angle) {
         auto finger = std::make_shared<anatomy::hand::Finger>(fingerType,
             create_monitored_joint(anatomy::hand::to_string(fingerType) + "_Flex",
                 id,
                 no_flex_angle,
                 full_flex_angle,
                 no_flex_angle));
         return finger;
     }

     std::shared_ptr<anatomy::hand::Thumb> HandBuilder::build_thumb() {

         auto oppose_joint = create_monitored_joint(anatomy::hand::to_string(anatomy::hand::Fingers::THUMB) + "_Oppose",
                 1,
                 constants::THUMB_OPPOSITION_NO_FLEX_ANGLE,
                 constants::THUMB_OPPOSITION_FULL_FLEX_ANGLE,
                 constants::THUMB_OPPOSITION_NO_FLEX_ANGLE);

         auto spread_joint = create_monitored_joint(anatomy::hand::to_string(anatomy::hand::Fingers::THUMB) + "_Spread",
                 2,
                 constants::THUMB_SPREAD_NO_FLEX_ANGLE,
                 constants::THUMB_SPREAD_FULL_FLEX_ANGLE,
                 constants::THUMB_SPREAD_NO_FLEX_ANGLE);

         auto flex_joint = create_monitored_joint(anatomy::hand::to_string(anatomy::hand::Fingers::THUMB) + "_Flex",
                 3,
                 constants::THUMB_FLEXION_NO_FLEX_ANGLE,
                 constants::THUMB_FLEXION_FULL_FLEX_ANGLE,
                 constants::THUMB_FLEXION_NO_FLEX_ANGLE);

         auto thumb = std::make_shared<anatomy::hand::Thumb>(flex_joint, spread_joint, oppose_joint);
         return thumb;
     }

     std::shared_ptr<anatomy::hand::Hand> HandBuilder::build() {

         auto pinky_finger = build_standard_finger(anatomy::hand::Fingers::PINKY, 7, constants::PINKY_FINGER_NO_FLEX_ANGLE, constants::PINKY_FINGER_FULL_FLEX_ANGLE);
         auto ring_finger = build_standard_finger(anatomy::hand::Fingers::RING, 6, constants::RING_FINGER_NO_FLEX_ANGLE, constants::RING_FINGER_FULL_FLEX_ANGLE);
         auto middle_finger = build_standard_finger(anatomy::hand::Fingers::MIDDLE, 5, constants::MIDDLE_FINGER_NO_FLEX_ANGLE, constants::MIDDLE_FINGER_FULL_FLEX_ANGLE);
         auto index_finger = build_standard_finger(anatomy::hand::Fingers::INDEX, 4, constants::INDEX_FINGER_NO_FLEX_ANGLE, constants::INDEX_FINGER_FULL_FLEX_ANGLE);
         auto thumb = build_thumb();

         auto right_hand = std::make_shared<anatomy::hand::Hand>(anatomy::hand::Side::RIGHT,
             pinky_finger,
             ring_finger,
             middle_finger,
             index_finger,
             thumb);

         return right_hand;
     }



}