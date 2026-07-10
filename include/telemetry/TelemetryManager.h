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
#include <condition_variable>
#include "telemetry/IMonitorable.h"
#include "telemetry/alert/ReadingEvaluator.h"

namespace telemetry {

    template <typename M>
    requires std::derived_from<M, BaseReading>

    class TelemetryManager {
    private:
        static inline std::vector<telemetry::IMonitorable<M>*> monitorables_;
        static inline std::map<uint8_t, IMonitorable<M>*> monitorable_objects_map_;
        static inline std::unordered_map<IMonitorable<M>*, std::jthread> threads_;
        static inline std::mutex registry_mutex;
        static inline bool is_running = false;

        static void start_monitoring_thread(IMonitorable<M>* monitorable) {
            threads_[monitorable] = std::jthread([monitorable](std::stop_token stop_token) {
                while (!stop_token.stop_requested()) {
                    const M reading = monitorable->poll();
                    ReadingEvaluator::evaluate(reading);
                    // Use a local condition variable or simple sleep to avoid blocking
                    // the TelemetryManager's ability to register/unregister other objects.
                    auto interval = std::chrono::milliseconds(monitorable->get_polling_interval_ms());
                    std::mutex dummy_mtx;
                    std::unique_lock<std::mutex> lock(dummy_mtx);
                    std::condition_variable_any().wait_for(lock, stop_token, interval, [&] { return stop_token.stop_requested(); });
                }
            });
        }

    public:
        static void register_monitorable(const uint8_t id, IMonitorable<M>* monitorable) {
            if (!monitorable) return;
            // Check if a monitorable with this ID already exists
            if (monitorable_objects_map_.count(id)) {
                throw std::runtime_error("Monitorable with ID " + std::to_string(id) + " already registered.");
            }
            monitorable_objects_map_[id] = monitorable;

            std::lock_guard<std::mutex> lock(registry_mutex);

            // Prevent duplicate registration
            if (std::find(monitorables_.begin(), monitorables_.end(), monitorable) != monitorables_.end()) return;

            monitorables_.push_back(monitorable);

            // Reactivity: if manager is already started, spin up the thread immediately
            if (is_running) {
                start_monitoring_thread(monitorable);
            }
        }

        static void unregister_monitorable(IMonitorable<M>* monitorable) {
            if (!monitorable) return;
            // Find the ID associated with the monitorable pointer
            uint8_t found_id = 0;
            bool found_in_map = false;
            for (auto const& [id, ptr] : monitorable_objects_map_) {
                if (ptr == monitorable) {
                    found_id = id;
                    found_in_map = true;
                    break;
                }
            }

            if (!found_in_map) {
                // Monitorable not found in the primary map, nothing to unregister.
                return;
            }

            // Remove from monitorable_objects_map_
            monitorable_objects_map_.erase(found_id);

            std::lock_guard<std::mutex> lock(registry_mutex);

            monitorables_.erase(
                std::remove(monitorables_.begin(), monitorables_.end(), monitorable),
                monitorables_.end()
            );

            // Reactivity: removing from the map destroys the jthread, which triggers request_stop() and join()
            threads_.erase(monitorable);
        }

        static void start() {
            std::lock_guard<std::mutex> lock(registry_mutex);
            if (is_running) return;

            is_running = true;
            for (auto* m : monitorables_) {
                start_monitoring_thread(m);
            }
        }

        static void stop() {
            is_running = false;
            for (auto* m : monitorables_) {
                unregister_monitorable(m);
            }
        }

        static IMonitorable<M>* get_monitorable(const uint8_t id) {
            return monitorable_objects_map_[id];
        }

        static IMonitorable<M>* get_all_monitorables() {
            return monitorables_;
        }
    };
}

//
// /**
//  * Implementations for IMonitorable methods that depend on TelemetryManager.
//  * These are defined here to ensure the compiler has the full definition of TelemetryManager.
//  */
// namespace telemetry {
//     template <typename M>
//     requires std::derived_from<M, BaseReading>
//     inline void IMonitorable<M>::start_monitoring() {
//         TelemetryManager<M>::register_monitorable(this);
//     }
//
//     template <typename M>
//     requires std::derived_from<M, BaseReading>
//     inline void IMonitorable<M>::stop_monitoring() {
//         TelemetryManager<M>::unregister_monitorable(this);
//     }
// }

#endif //THING_TELEMETRYMANAGER_H
