#include <iostream>
#include <utility>
#include <builder/HandBuilder.h>

#include "control/CommandLineController.h"
#include "telemetry/TelemetryManager.h"
#include "telemetry/alert/AlertHandler.h"
#include "telemetry/logging/LiveLogger.h"


void register_log_action() {
    using namespace telemetry::alert;
    auto log_action = [](const Alert& alert) {
        telemetry::logging::LiveLogger::get_instance().log(alert);
    };
    AlertHandler::get_instance().register_action_handler(SuggestedAction::LOG, log_action);
}

void register_shutdown_action() {
    using namespace telemetry::alert;
    using namespace telemetry;
    auto shutdown_action = [](const Alert& alert) {
        // CRITICAL: Call TelemetryManager::stop() before exiting
        TelemetryManager::stop();
        std::exit(EXIT_FAILURE);
    };
    AlertHandler::get_instance().register_action_handler(SuggestedAction::SHUTDOWN, shutdown_action);
}

 void register_freeze_action() {
     using namespace telemetry::alert;
     using namespace telemetry;
     AlertHandler::get_instance().register_action_handler(SuggestedAction::FREEZE, [](const Alert& alert) {
         if (alert.reading) {
             IMonitorable<JointReading> *monitorable_joint = TelemetryManager::get_monitorable(
                  alert.reading->monitorable_id);
              if (auto joint = dynamic_cast<articulation::Joint*>(monitorable_joint)) {
                  joint->freeze();
              }
         }
     });
 }

 void register_unfreeze_action() {
     using namespace telemetry::alert;
     using namespace telemetry;
     AlertHandler::get_instance().register_action_handler(SuggestedAction::UNFREEZE, [](const Alert& alert) {
         if (alert.reading) {
             IMonitorable<JointReading> *monitorable_joint = TelemetryManager::get_monitorable(
                  alert.reading->monitorable_id);
              if (auto joint = dynamic_cast<Joint*>(monitorable_joint)) { // Safely cast to Joint*
                  joint->unfreeze();
              }
         }
     });
 }

 void register_reset_action() {
     using namespace telemetry::alert;
     using namespace telemetry;
     AlertHandler::get_instance().register_action_handler(SuggestedAction::RESET, [](const Alert& alert) {
         if (alert.reading) {
             IMonitorable<JointReading> *monitorable_joint = TelemetryManager::get_monitorable(
                  alert.reading->monitorable_id);
              if (auto joint = dynamic_cast<Joint*>(monitorable_joint)) { // Safely cast to Joint*
                  joint->unfreeze(); // Ensure it's not frozen so it can move
                  joint->reset();
              }
         }
     });
 }

 void register_throttle_action() {
     using namespace telemetry::alert;
     using namespace telemetry;
     AlertHandler::get_instance().register_action_handler(SuggestedAction::THROTTLE, [](const Alert& alert) {
         if (alert.reading) {
             IMonitorable<JointReading> *monitorable_joint = TelemetryManager::get_monitorable(
                  alert.reading->monitorable_id);
              if (auto joint = dynamic_cast<Joint*>(monitorable_joint)) { // Safely cast to Joint*
                  joint->unfreeze(); // Ensure it's not frozen so it can move
                  joint->throttle();
              }
         }
     });
 }

 void register_unthrottle_action() {
     using namespace telemetry::alert;
     using namespace telemetry;
     AlertHandler::get_instance().register_action_handler(SuggestedAction::UNTHROTTLE, [](const Alert& alert) {
         if (alert.reading) {
             IMonitorable<JointReading> *monitorable_joint = TelemetryManager::get_monitorable(
                  alert.reading->monitorable_id);
              if (auto joint = dynamic_cast<Joint*>(monitorable_joint)) { // Safely cast to Joint*
                  joint->unfreeze(); // Ensure it's not frozen so it can move
                  joint->unthrottle();
              }
         }
     });
 }


void register_alert_actions() {
    register_log_action();
    register_shutdown_action();
    register_freeze_action();
    register_reset_action();
    register_throttle_action();
    register_unthrottle_action();
    register_unfreeze_action();
}

void initialize_telemetry() {
    telemetry::TelemetryManager::start();
    telemetry::logging::LiveLogger::get_instance().display();
}

void initialize_controller(std::shared_ptr<anatomy::hand::Hand> hand) {
    // Create an instance of the CommandLineController
    // We need to pass the hand object to the controller so it can execute movements
    auto controller = control::CommandLineController(std::move(hand));
    controller.start(); // Start the controller's main loop
}

int main() {
    std::shared_ptr<anatomy::hand::Hand> hand = builder::HandBuilder::build();
    register_alert_actions();
    initialize_telemetry();
    initialize_controller(hand);

    // CRITICAL: Explicitly stop the TelemetryManager before main exits
    //telemetry::TelemetryManager::stop();

    return 0;
}