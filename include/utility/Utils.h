//
// Created by Nagareddy on 05/07/26.
//

#ifndef THING_UTILS_H
#define THING_UTILS_H

#include <chrono>
#include <string>
#include <format>

namespace utility {

    /**
     * Converts a steady_clock::time_point to a human-readable system time string.
     * Note: This is an approximation based on the current relationship between clocks.
     */
    inline std::string format_time_point(std::chrono::steady_clock::time_point tp) {
        // Calculate the difference between the target time and "now" in steady_clock
        auto now_steady = std::chrono::steady_clock::now();
        auto diff = tp - now_steady;

        // Apply that difference to the current wall-clock (system_clock)
        auto now_system = std::chrono::system_clock::now();
        auto wall_time = now_system + std::chrono::duration_cast<std::chrono::system_clock::duration>(diff);

        // Format using C++20 std::format (HH:MM:SS.ms)
        return std::format("{:%H:%M:%S}", wall_time);
    }
}

#endif //THING_UTILS_H