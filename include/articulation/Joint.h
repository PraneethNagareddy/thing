//
// Created by Nagareddy on 28/06/26.
//

#ifndef THING_JOINTS_H
#define THING_JOINTS_H
#include "../hardware/servo/IServo.h"
#include "Motion.h"
#include "telemetry/IMonitorable.h"
#include <memory>
namespace articulation {
class Joint : public telemetry::IMonitorable<telemetry::JointReading> {
private:
  std::shared_ptr<hardware::IServo> servo_;
  float default_angle_;
  float current_angle_;
  float no_flex_angle_;
  float full_flex_angle_;
  float max_speed_;

public:
  Joint(std::shared_ptr<hardware::IServo> servo, float default_angle,
        float no_flex_angle, float full_flex_angle, float max_speed)
      : servo_(std::move(servo)), default_angle_(default_angle),
        no_flex_angle_(no_flex_angle), full_flex_angle_(full_flex_angle),
        current_angle_(default_angle), max_speed_(max_speed) {}

  movement::Status move_to(float movement_percent, int time_to_execute_ms);
  const telemetry::JointReading poll() override;

  void freeze() const;
  void unfreeze() const;
  void throttle() const;
  void unthrottle() const;
  void reset();
  void disable() const;
  void move_to_default() { move_to(0.0f, 1000); }
};
} // namespace articulation
#endif // THING_JOINTS_H
