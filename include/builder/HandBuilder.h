//
// Created by Nagareddy on 08/07/26.
//
#ifndef THING_HANDBUILDER_H
#define THING_HANDBUILDER_H

#include <memory>
#include "anatomy/Hand.h"
#include "anatomy/Finger.h"
#include "anatomy/Thumb.h"

namespace builder {

    /**
     * HandBuilder is responsible for the complex orchestration of instantiating
     * the hand anatomy, configuring servo constraints, and wiring telemetry/safety.
     */
    class HandBuilder {
    public:
        explicit HandBuilder() = default;

        /**
         * Assembles the joints and fingers into a complete Hand object.
         * Automatically registers all joints for telemetry monitoring.
         */
        static std::unique_ptr<anatomy::hand::Hand> build();

    private:
        // Helper to create a joint and register it for monitoring
        static std::shared_ptr<articulation::Joint> create_monitored_joint(
            const std::string& name, uint8_t id, float no_flex_angle, float full_flex_angle, float default_angle);

        // Finger specific construction helpers
        static std::shared_ptr<anatomy::hand::Finger> build_standard_finger(
            const anatomy::hand::Fingers finger, uint8_t id, float no_flex_angle, float full_flex_angle);

        static std::shared_ptr<anatomy::hand::Thumb> build_thumb();
    };

}

#endif //THING_HANDBUILDER_H