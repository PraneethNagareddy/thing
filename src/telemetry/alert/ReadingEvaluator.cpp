#include <iostream>

#include "telemetry/alert/ReadingEvaluator.h"
#include "telemetry/alert/AlertRulesFactory.h"

namespace telemetry::alert {
    std::vector<Alert> ReadingEvaluator::process_reading(const BaseReading& reading) {
        std::vector<Alert> triggered_alerts;
        for (const auto& rule : AlertRulesFactory::get()) {
            auto result = rule.evaluate(reading);
            if (result) {
                triggered_alerts.push_back(*result);
            }
        }
        return triggered_alerts;
    }

    void ReadingEvaluator::evaluate(const BaseReading& reading) {
        auto alerts = process_reading(reading);
        for (const auto& alert : alerts) {
            // Example Sink: Log to console
            std::cout << "[ALERT] " << alert.message << std::endl;
        }
    }
}
