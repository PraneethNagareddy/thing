#ifndef THING_ALERTRULESFACTORY_H
#define THING_ALERTRULESFACTORY_H

#include <vector>
#include <concepts>

#include "AlertRule.h"
#include "telemetry/Reading.h"

namespace telemetry::alert {

    class AlertRulesFactory {
    public:
        static std::vector<AlertRule> get();
    };

}

#endif //THING_ALERTRULESFACTORY_H