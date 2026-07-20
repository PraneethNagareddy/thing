//
// Created by Nagareddy on 05/07/26.
//

#ifndef THING_LIVELOGGER_H
#define THING_LIVELOGGER_H

#include <string>
#include <map>
#include <mutex> // Required for std::mutex
#include <vector>
#include <iostream> // For std::cerr
// For stack trace (macOS/Linux)
#include <execinfo.h> // For backtrace, backtrace_symbols
#include <cxxabi.h>   // For __cxa_demangle (to demangle C++ symbols)


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
        mutable std::recursive_mutex log_mutex_; // <--- CHANGED TO std::recursive_mutex

        static constexpr size_t MAX_ALERTS = 15;
        LiveLogger() = default;

        // Helper to print stack trace (copied from TelemetryManager for local use)
        static void print_stack_trace() {
            void* callstack[128];
            int frames = backtrace(callstack, 128);
            char** strs = backtrace_symbols(callstack, frames);
            std::cerr << "--- STACK TRACE (LiveLogger) ---" << std::endl;
            for (int i = 0; i < frames; ++i) {
                std::string s(strs[i]);
                // Attempt to demangle C++ symbols
                size_t start = s.find('(');
                size_t end = s.find('+', start);
                if (start != std::string::npos && end != std::string::npos) {
                    std::string mangled_name = s.substr(start + 1, end - (start + 1));
                    int status;
                    char* demangled_name = abi::__cxa_demangle(mangled_name.c_str(), nullptr, nullptr, &status);
                    if (status == 0) {
                        std::cerr << s.substr(0, start + 1) << demangled_name << s.substr(end) << std::endl;
                    } else {
                        std::cerr << s << std::endl;
                    }
                    free(demangled_name);
                } else {
                    std::cerr << s << std::endl;
                }
            }
            std::cerr << "--------------------------------" << std::endl;
            free(strs);
        }

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