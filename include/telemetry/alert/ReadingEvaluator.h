//
// Created by Nagareddy on 05/07/26.
//

#ifndef THING_READINGEVALUATOR_H
#define THING_READINGEVALUATOR_H
#include "AlertRule.h"
#include "telemetry/Reading.h"

#include <vector>
 #include <memory>

 #include "telemetry/Reading.h"
 #include "telemetry/alert/AlertRule.h"

 namespace telemetry::alert {

     class ReadingEvaluator {
     public:
         static std::vector<Alert> process_reading(const BaseReading& reading);
         static void evaluate(const BaseReading& reading);
     };
 }
#endif //THING_READINGEVALUATOR_H
