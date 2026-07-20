//
// Created by Nagareddy on 09/07/26.
//

#ifndef THING_TELEMETRYMANAGER_H
#define THING_TELEMETRYMANAGER_H
#include <vector>
#include <algorithm>
#include <concepts>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <map>
#include <iostream>
#include <condition_variable>
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string

// For stack trace (macOS/Linux)
#include <execinfo.h> // For backtrace, backtrace_symbols
#include <cxxabi.h>   // For __cxa_demangle (to demangle C++ symbols)


#include "telemetry/IMonitorable.h"
#include "telemetry/alert/ReadingEvaluator.h"

namespace telemetry {


    class TelemetryManager {
    private:
        static inline std::vector<telemetry::IMonitorable<JointReading>*> monitorables_;
        static inline std::map<uint8_t, IMonitorable<JointReading>*> monitorable_objects_map_;
        static inline std::unordered_map<IMonitorable<JointReading>*, std::jthread> threads_;
        static inline std::mutex registry_mutex;
        static inline bool is_running = false;

        // Helper to print stack trace
        static void print_stack_trace() {
            void* callstack[128];
            int frames = backtrace(callstack, 128);
            char** strs = backtrace_symbols(callstack, frames);
            std::cerr << "--- STACK TRACE ---" << std::endl;
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
            std::cerr << "-------------------" << std::endl;
            free(strs);
        }


        static void start_monitoring_thread(IMonitorable<JointReading>* monitorable) {
            threads_[monitorable] = std::jthread([monitorable](std::stop_token stop_token) {
                while (!stop_token.stop_requested()) {
                    const JointReading reading = monitorable->poll();
                    ReadingEvaluator::evaluate(reading);
                    // Use a local condition variable or simple sleep to avoid blocking
                    // the TelemetryManager's ability to register/unregister other objects.
                    auto interval = std::chrono::milliseconds(monitorable->get_polling_interval_ms());
                    std::mutex dummy_mtx;
                    std::unique_lock<std::mutex> lock(dummy_mtx);
                    std::condition_variable_any().wait_for(lock, stop_token, interval, [&] { return stop_token.stop_requested(); });
                    lock.unlock();
                }
            });
        }

        // Private helper for internal unregistration logic (assumes registry_mutex is already held)
        static void remove_monitorable_internal(IMonitorable<JointReading>* monitorable);

    public:
        static void register_monitorable(const uint8_t id, IMonitorable<JointReading>* monitorable) {
            if (!monitorable) return;
            // Check if a monitorable with this ID already exists
            if (monitorable_objects_map_.count(id)) {
                throw std::runtime_error("Monitorable with ID " + std::to_string(id) + " already registered.");
            }
            monitorable_objects_map_[id] = monitorable;

            std::unique_lock<std::mutex> lock(registry_mutex);

            // Prevent duplicate registration
            if (std::find(monitorables_.begin(), monitorables_.end(), monitorable) != monitorables_.end()) return;

            monitorables_.push_back(monitorable);

            // Reactivity: if manager is already started, spin up the thread immediately
            if (is_running) {
                start_monitoring_thread(monitorable);
            }
            lock.unlock();
        }

        // Public unregister_monitorable now acquires the lock and calls the internal helper
        static void unregister_monitorable(IMonitorable<JointReading>* monitorable) {
            // Acquire lock only for modifying shared data structures
            std::unique_lock<std::mutex> lock(registry_mutex);
            remove_monitorable_internal(monitorable);
            // Lock is released here, before any potential join() in stop()
        }

        static void start() {
            std::unique_lock<std::mutex> lock(registry_mutex);
            if (is_running) return;

            is_running = true;
            for (auto* m : monitorables_) {
                start_monitoring_thread(m);
            }
            lock.unlock();
        }

        static void stop() {
            // Acquire lock only for modifying shared data structures
            std::unique_lock<std::mutex> lock(registry_mutex);
            if (!is_running) return;

            is_running = false; // Signal all threads to stop

            // Create a copy of monitorables_ and threads_ map to process outside the lock
            std::vector<IMonitorable<JointReading>*> monitorables_to_stop = monitorables_;
            std::unordered_map<IMonitorable<JointReading>*, std::jthread> threads_to_join = std::move(threads_); // Move threads out

            // Clear the original lists while holding the lock
            monitorables_.clear();
            monitorable_objects_map_.clear();
            // threads_ is now empty after the move

            lock.unlock(); // <--- RELEASE THE LOCK BEFORE JOINING THREADS

            std::cerr << "[TelemetryManager] Requesting stop and joining " << threads_to_join.size() << " monitoring threads..." << std::endl;

            // Now, iterate through the moved threads and let their destructors join them
            // The destructor of std::jthread will call request_stop() and then join()
            // This loop ensures all jthreads in threads_to_join are destroyed.
            for (auto& pair : threads_to_join) {
                // The jthread destructor will be called as 'pair' goes out of scope or when threads_to_join is destroyed
                // We don't need to explicitly call join() here, jthread's destructor handles it.
                // However, if a thread is self-unregistering, it would have been detached.
                if (pair.second.joinable()) { // Check if it's still joinable (not detached)
                    // The destructor will join, but we can explicitly request stop here for clarity
                    pair.second.request_stop();
                }
            }
            // All jthreads in threads_to_join are destroyed here, implicitly joining their threads.

            std::cerr << "[TelemetryManager] All monitoring threads stopped and joined." << std::endl;
        }

        static IMonitorable<JointReading>* get_monitorable(const uint8_t id) {
            // This method needs a lock too, as monitorable_objects_map_ is modified by other locked methods
            std::lock_guard<std::mutex> lock(registry_mutex);
            auto it = monitorable_objects_map_.find(id);
            if (it == monitorable_objects_map_.end()) {
                throw std::out_of_range("Monitorable with ID " + std::to_string(id) + " not found.");
            }
            return it->second;
        }

    };
}

#endif //THING_TELEMETRYMANAGER_H