//
// Created by Nagareddy on 05/07/26.
//

#ifndef THING_ALERTHANDLER_H
#define THING_ALERTHANDLER_H

#include <functional>
#include <map>
#include <mutex>
#include <vector>
#include "Alert.h"

namespace telemetry::alert {

    /**
     * AlertHandler is a singleton responsible for executing the SuggestedActions
     * attached to generated Alerts.
     */
    class AlertHandler {
    public:
        using ActionCallback = std::function<void(const Alert&)>;

        static AlertHandler& getInstance() {
            static AlertHandler instance;
            return instance;
        }

        // Delete copy/move for singleton integrity
        AlertHandler(const AlertHandler&) = delete;
        AlertHandler& operator=(const AlertHandler&) = delete;

        /**
         * Registers a specific function to be executed for a SuggestedAction.
         */
        void register_action_handler(SuggestedAction action, ActionCallback callback);

        /**
         * Processes an alert by executing all of its suggested actions.
         */
        void handle(const Alert& alert);

    private:
        AlertHandler(); // Private constructor initializes default handlers

        std::map<SuggestedAction, std::vector<ActionCallback>> handlers_;
        std::mutex handler_mutex_;
    };

}

#endif //THING_ALERTHANDLER_H