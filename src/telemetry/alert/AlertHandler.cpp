//
// Created by Nagareddy on 05/07/26.
//

#include "telemetry/alert/AlertHandler.h"
#include "telemetry/logging/LiveLogger.h"
#include <iostream>

namespace telemetry::alert {

    AlertHandler::AlertHandler() = default;

    void AlertHandler::register_action_handler(SuggestedAction action, ActionCallback callback) {
        std::lock_guard<std::mutex> lock(handler_mutex_);
        handlers_[action].push_back(std::move(callback));
    }

    void AlertHandler::handle(const Alert& alert) {
        std::lock_guard<std::mutex> lock(handler_mutex_);

        for (const auto& action : alert.actions) {
            auto it = handlers_.find(action);
            if (it != handlers_.end()) {
                for (const auto& callback : it->second) {
                    if (callback) {
                        callback(alert);
                    }
                }
            } else {
                // Fail-safe logging if an action is suggested but no hardware handler is registered
                std::cerr << "[AlertHandler] No handler registered for action: "
                          << static_cast<int>(action)
                          << " (Alert: " << alert.message << ")" << std::endl;
            }
        }
    }
}