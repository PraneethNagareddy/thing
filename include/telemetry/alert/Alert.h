//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_ALERT_H
#define THING_ALERT_H

#include <chrono>
#include <concepts>

#include "../IMonitorable.h"


namespace telemetry::alert {

    enum class SuggestedAction {
        LOG,
        SHUTDOWN,
        FREEZE,
        RESET,
        THROTTLE,
        UNTHROTTLE,
        UNFREEZE
    };

    enum class SeverityLevel {
        CRITICAL,
        HIGH,
        MEDIUM,
        NORMAL,
    };

    struct Alert {
        const BaseReading* reading;
        std::string message;
        SeverityLevel level = SeverityLevel::CRITICAL;
        std::vector<SuggestedAction> actions = {SuggestedAction::FREEZE, SuggestedAction::LOG};
        std::chrono::steady_clock::time_point timestamp = std::chrono::steady_clock::now();
    };
}
#endif //THING_ALERT_H
