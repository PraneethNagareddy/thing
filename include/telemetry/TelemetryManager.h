//
// Created by Nagareddy on 03/07/26.
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
#include <condition_variable>
#include "telemetry/IMonitorable.h"

namespace telemetry {

    template <typename M>
    requires std::derived_from<M, BaseReading>

    class TelemetryManager {
    private:
        static inline std::vector<telemetry::IMonitorable<M>*> monitorables_;
        static inline std::unordered_map<IMonitorable<M>*, std::jthread> threads_;
        static inline std::mutex registry_mutex;
        static inline bool is_running = false;

        static void start_monitoring_thread(IMonitorable<M>* monitorable) {
            threads_[monitorable] = std::jthread([monitorable](std::stop_token stop_token) {
                while (!stop_token.stop_requested()) {
                    monitorable->poll();
                    
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
        static void register_monitorable(IMonitorable<M>* monitorable) {
            if (!monitorable) return;

            std::lock_guard<std::mutex> lock(registry_mutex);
            
            // Prevent duplicate registration
            if (std::find(monitorables_.begin(), monitorables_.end(), monitorable) != monitorables.end()) return;

            monitorables_.push_back(monitorable);

            // Reactivity: if manager is already started, spin up the thread immediately
            if (is_running) {
                start_monitoring_thread(monitorable);
            }
        }

        static void unregister_monitorable(IMonitorable<M>* monitorable) {
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
    };
}


/**
 * Implementations for IMonitorable methods that depend on TelemetryManager.
 * These are defined here to ensure the compiler has the full definition of TelemetryManager.
 */
namespace telemetry {
    template <typename M>
    requires std::derived_from<M, BaseReading>
    inline void IMonitorable<M>::start_monitoring() {
        TelemetryManager<M>::register_monitorable(this);
    }

    template <typename M>
    requires std::derived_from<M, BaseReading>
    inline void IMonitorable<M>::stop_monitoring() {
        TelemetryManager<M>::unregister_monitorable(this);
    }
}

#endif //THING_TELEMETRYMANAGER_H
