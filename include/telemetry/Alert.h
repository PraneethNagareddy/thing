//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_ALERT_H
#define THING_ALERT_H

#include <chrono>
#include <concepts>

#include "IMonitorable.h"


namespace telemetry {

    enum class SuggestedAction {
        LOG,
        FREEZE,
        RESET
    };

    enum class Level {
        HIGH,
        MEDIUM,
        LOW
    };

    template <typename M>
    requires std::derived_from<M, BaseReading>

    class Alert {
    private:
        IMonitorable<M> * monitorable_;
        SuggestedAction action_;
        Level level_;
        std::chrono::steady_clock::time_point timestamp;
        M reading_;
    public:
        Alert(IMonitorable<M> &monitorable,
            M reading,
            SuggestedAction action = SuggestedAction::LOG,
            Level level = Level::LOW,
            std::chrono::steady_clock::time_point timestamp = std::chrono::steady_clock::now()) :
            monitorable_(&monitorable),
            action_(action),
            level_(level),
            timestamp(timestamp),
            reading_(reading) { }
    };
}
#endif //THING_ALERT_H
