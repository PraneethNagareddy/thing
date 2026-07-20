//
// Created by Nagareddy on 05/07/26.
//

#include "telemetry/logging/LiveLogger.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdio> // For remove()

#include "utility/Utils.h"

namespace telemetry::logging {

    // Destructor implementation
    LiveLogger::~LiveLogger() {
        stop_display_thread(); // Ensure the display thread is stopped
        if (output_stream_.is_open()) {
            output_stream_.close();
        }
    }

    void LiveLogger::set_output_path(const std::string& filepath) {
        std::lock_guard<std::recursive_mutex> lock(log_mutex_);
        if (output_stream_.is_open()) {
            output_stream_.close();
        }
        output_path_ = filepath;
        // Open the stream. For named pipes, this will block until a reader opens the other end.
        output_stream_.open(output_path_, std::ios_base::out);
        if (!output_stream_.is_open()) {
            std::cerr << "LiveLogger: Failed to open output stream to " << output_path_ << std::endl;
        } else {
            std::cout << "LiveLogger: Output stream opened to " << output_path_ << std::endl;
        }
    }

    void LiveLogger::start_display_thread(std::chrono::seconds interval) {
        if (display_running_.exchange(true)) { // Set to true, if it was false, proceed
            std::cout << "LiveLogger: Display thread already running." << std::endl;
            return;
        }

        display_thread_ = std::jthread([this, interval](std::stop_token st) {
            std::cout << "LiveLogger: Display thread started." << std::endl;
            while (!st.stop_requested()) {
                this->display(); // Call the const display method
                std::this_thread::sleep_for(interval);
            }
            std::cout << "LiveLogger: Display thread stopping." << std::endl;
        });
    }

    void LiveLogger::stop_display_thread() {
        if (display_running_.exchange(false)) { // Set to false, if it was true, proceed
            if (display_thread_.joinable()) {
                display_thread_.request_stop(); // Signal the thread to stop
                // jthread's destructor will call join(), so no explicit join() here
                // If this is called from LiveLogger's destructor, display_thread_ will be destroyed
                // and implicitly joined.
            }
            std::cout << "LiveLogger: Display thread stopped." << std::endl;
        }
    }


    void LiveLogger::log(const BaseReading& reading) {
        //print_stack_trace(); // Uncomment if you suspect this lock is the issue
        std::lock_guard<std::recursive_mutex> lock(log_mutex_);

        std::string type_name = "Unknown";
        std::stringstream ss;

        // Type-specific extraction
        if (auto joint = dynamic_cast<const JointReading*>(&reading)) {
            type_name = "Joint";
            ss << "Monitorable ID: " << reading.monitorable_id << " | "
               << "Temp: " << std::fixed << std::setprecision(1) << joint->temperature_celsius << "C | "
               << "Volt: " << joint->voltage_mv << "mV | "
               << "Load: " << joint->load_percentage << "% | "
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
        std::lock_guard<std::recursive_mutex> lock(log_mutex_);

        // Determine output stream: named pipe/file or console
        // FIX: Explicitly cast output_stream_ to std::ostream&
        std::ostream& os = output_stream_.is_open() ? static_cast<std::ostream&>(output_stream_) : std::cout;

        // Clear screen (ANSI escape code) - optional, for a true "Live" feel
        // This will clear the *target* terminal, whether it's the main one or the popup.
        os << "\033[2J\033[1;1H";

        os << "================== TELEMETRY DASHBOARD ==================\n";

        if (registry_.empty()) {
            os << "Waiting for data...\n";
        }

        for (const auto& [type, sensors] : registry_) {
            os << "\n[" << type << "]\n";
            for (const auto& [id, data] : sensors) {
                os << "  > " << std::left << std::setw(10) << id
                          << " | " << data << "\n";
            }
        }
        os << "\n=========================================================\n";

        os << "\n--------------------- RECENT ALERTS ---------------------\n";
        if (recent_alerts_.empty()) {
            os << "No alerts detected.\n";
        } else {
            for (const auto& alert : recent_alerts_) {
                os << "[" << utility::format_time_point(alert.timestamp) << "] "
                          << severity_to_string(alert.level) << " "
                          << "ID:" << alert.reading->monitorable_id << " - " // Corrected access
                          << alert.message << "\n";
            }
        }

        os << "=========================================================\n";
        os << std::flush; // Ensure data is written immediately
    }
}