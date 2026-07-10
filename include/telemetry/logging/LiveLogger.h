//
// Created by Nagareddy on 05/07/26.
//

#ifndef THING_LIVELOGGER_H
#define THING_LIVELOGGER_H

#include <string>
#include <map>
#include <mutex>
#include <vector>
#include "telemetry/alert/Alert.h"
#include "telemetry/Reading.h"

namespace telemetry::logging {

    /**
     * LiveLogger maintains the most recent state of all monitorables.
     * It categorizes data by Type and Sensor ID.
     */
    class LiveLogger {
    private:
        // Map structure: <TypeName, <SensorID, FormattedValue>>
        std::map<std::string, std::map<int, std::string>> registry_;
        std::vector<alert::Alert> recent_alerts_;
        std::mutex log_mutex_;

        static constexpr size_t MAX_ALERTS = 15;
        LiveLogger() = default;

    public:
        static LiveLogger& get_instance() {
            static LiveLogger instance;
            return instance;
        }

        // Delete copy/move for singleton integrity
        LiveLogger(const LiveLogger&) = delete;
        LiveLogger& operator=(const LiveLogger&) = delete;

        /**
         * Updates the logger with the latest reading.
         */
        void log(const BaseReading& reading);

        /**
         * Records a new alert to the rolling history.
        */
        void log(const alert::Alert& alert);

        /**
         * Prints a "Dashboard" view of all current readings to the console.
         */
        void display() const;
    };
}
#endif //THING_LIVELOGGER_H