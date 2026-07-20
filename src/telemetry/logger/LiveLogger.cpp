//
// Created by Nagareddy on 05/07/26.
//

#include "telemetry/logging/LiveLogger.h"
#include <iomanip>
#include <iostream>
#include <sstream>

#include "utility/Utils.h"

namespace telemetry::logging {

    void LiveLogger::log(const BaseReading& reading) {
        //print_stack_trace(); // Uncomment if you suspect this lock is the issue
        std::lock_guard<std::recursive_mutex> lock(log_mutex_);

        std::string type_name = "Unknown";
        std::stringstream ss;

        // Type-specific extraction
        if (auto joint = dynamic_cast<const JointReading*>(&reading)) {
            type_name = "Joint";
            ss << "Monitorable ID: " << reading.monitorable_id
               << "Temp: " << std::fixed << std::setprecision(1) << joint->temperature_celsius << "C | "
               << "Volt: " << joint->voltage_mv << "mV | "
               << "Load: " << joint->load_percentage << "%"
               << "Timestap: " << utility::format_time_point(reading.timestamp);
        }
        // Add else-if blocks here for other Reading types (Battery, IMU, etc.)
        else {
            ss << "Raw Value: " << utility::format_time_point(reading.timestamp);
        }

        registry_[type_name][reading.monitorable_id] = ss.str();
    }

    void LiveLogger::log(const alert::Alert& alert) {
        //print_stack_trace(); // Uncomment if you suspect this lock is the issue
        std::lock_guard<std::recursive_mutex> lock(log_mutex_);

        recent_alerts_.insert(recent_alerts_.begin(), alert);

        if (recent_alerts_.size() > MAX_ALERTS) {
            recent_alerts_.pop_back();
        }
    }

    std::string severity_to_string(alert::SeverityLevel level) {
        switch (level) {
            case alert::SeverityLevel::CRITICAL: return "\033[1;31m[CRITICAL]\033[0m"; // Bold Red
            case alert::SeverityLevel::HIGH:     return "\033[31m[HIGH]\033[0m";      // Red
            case alert::SeverityLevel::MEDIUM:   return "\033[33m[MEDIUM]\033[0m";    // Yellow
            case alert::SeverityLevel::NORMAL:   return "\033[32m[NORMAL]\033[0m";    // Green
            default:                             return "[INFO]";
        }
    }


    void LiveLogger::display() const {
        //print_stack_trace(); // Uncomment if you suspect this lock is the issue
        std::lock_guard<std::recursive_mutex> lock(log_mutex_); // <--- REMOVED const_cast

        // Clear screen (ANSI escape code) - optional, for a true "Live" feel
        std::cout << "\033[2J\033[1;1H";

        std::cout << "================== TELEMETRY DASHBOARD ==================\n";

        if (registry_.empty()) {
            std::cout << "Waiting for data...\n";
        }

        for (const auto& [type, sensors] : registry_) {
            std::cout << "\n[" << type << "]\n";
            for (const auto& [id, data] : sensors) {
                std::cout << "  > " << std::left << std::setw(10) << id
                          << " | " << data << "\n";
            }
        }
        std::cout << "\n=========================================================\n";

        std::cout << "\n--------------------- RECENT ALERTS ---------------------\n";
        if (recent_alerts_.empty()) {
            std::cout << "No alerts detected.\n";
        } else {
            for (const auto& alert : recent_alerts_) {
                std::cout << "[" << utility::format_time_point(alert.timestamp) << "] "
                          << severity_to_string(alert.level) << " "
                          << "ID:" << alert.reading->monitorable_id << " - "
                          << alert.message << "\n";
            }
        }

        std::cout << "=========================================================\n";
    }
}