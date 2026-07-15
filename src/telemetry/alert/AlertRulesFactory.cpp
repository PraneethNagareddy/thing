//
// Created by Nagareddy on 03/07/26.
//

#include "telemetry/alert/AlertRulesFactory.h"

#include "telemetry/alert/AlertRule.h"

namespace telemetry::alert {

    static AlertRule getCriticalTemperatureAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.temperature_celsius >= 70.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at critical temperature: " + std::to_string(r.temperature_celsius),
                    .level = SeverityLevel::CRITICAL,
                    .actions = {SuggestedAction::FREEZE,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getHighTemperatureAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.temperature_celsius > 60.0 && r.temperature_celsius < 70.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at High temperature: " + std::to_string(r.temperature_celsius),
                    .level = SeverityLevel::HIGH,
                    .actions = {SuggestedAction::THROTTLE,SuggestedAction::LOG},
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getMediumTemperatureAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.temperature_celsius > 50.0 && r.temperature_celsius <= 60.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo temperature raising - : " + std::to_string(r.temperature_celsius),
                    .level = SeverityLevel::MEDIUM,
                    .actions = {SuggestedAction::UNFREEZE,SuggestedAction::LOG},
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getNormalTemperatureAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.temperature_celsius <= 50.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo temperature back to normal - : " + std::to_string(r.temperature_celsius),
                    .level = SeverityLevel::NORMAL,
                    .actions = {SuggestedAction::UNFREEZE,SuggestedAction::UNTHROTTLE,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getCriticalLoadAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.load_percentage > 95.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at critical load: " + std::to_string(r.load_percentage),
                    .level = SeverityLevel::CRITICAL,
                    .actions = {SuggestedAction::RESET,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getHighLoadAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.load_percentage > 85.0 && r.load_percentage <= 95.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at high load: " + std::to_string(r.load_percentage),
                    .level = SeverityLevel::HIGH,
                    .actions = {SuggestedAction::THROTTLE,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getMediumLoadAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.load_percentage > 70.0 && r.load_percentage <= 85.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at high load: " + std::to_string(r.load_percentage),
                    .level = SeverityLevel::MEDIUM,
                    .actions = {SuggestedAction::UNTHROTTLE,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getNormalLoadAlertRule() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.load_percentage < 70.0) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at high load: " + std::to_string(r.load_percentage),
                    .level = SeverityLevel::HIGH,
                    .actions = {SuggestedAction::UNFREEZE,SuggestedAction::UNTHROTTLE,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getCriticalLowVoltageAlert() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.voltage_mv <= 4900) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at critical low voltage: " + std::to_string(r.voltage_mv),
                    .level = SeverityLevel::CRITICAL,
                    .actions = {SuggestedAction::SHUTDOWN,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }
    static AlertRule getCriticalHighVoltageAlert() {
        return AlertRule::Builder<JointReading>()
        .with_evaluator([](const JointReading& r) -> std::optional<Alert> {
            if (r.voltage_mv >= 7000) {
                return Alert{
                    .reading = &r,
                    .message = "Servo at critical high voltage: " + std::to_string(r.voltage_mv),
                    .level = SeverityLevel::CRITICAL,
                    .actions = {SuggestedAction::SHUTDOWN,SuggestedAction::LOG}
                };
            }
            return std::nullopt;
        })
        .build();
    }


    std::vector<AlertRule > AlertRulesFactory::get() {
        return {
            getCriticalTemperatureAlertRule(),
            getHighTemperatureAlertRule(),
            getMediumTemperatureAlertRule(),
            getNormalTemperatureAlertRule(),

            getCriticalLoadAlertRule(),
            getHighLoadAlertRule(),
            getMediumLoadAlertRule(),
            getNormalLoadAlertRule(),

            getCriticalHighVoltageAlert(),
            getCriticalLowVoltageAlert()
        };
    }
}
