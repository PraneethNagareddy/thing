#include <iostream>
#include <utility>
#include <builder/HandBuilder.h>
#include <cstdlib> // For std::system, std::atexit, EXIT_FAILURE
#include <string>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::seconds

// For named pipes (FIFOs)
#include <sys/stat.h> // For mkfifo
#include <unistd.h>   // For unlink
#include <cerrno>     // For errno
#include <cstring>    // For strerror

#include "control/CommandLineController.h"
#include "telemetry/TelemetryManager.h"
#include "telemetry/alert/AlertHandler.h"
#include "telemetry/logging/LiveLogger.h"


// Define the named pipe path
const std::string LIVE_LOGGER_FIFO_PATH = "/tmp/thing_telemetry_fifo";

// Function to clean up the named pipe on exit
void cleanup_fifo() {
    std::cout << "Cleaning up named pipe: " << LIVE_LOGGER_FIFO_PATH << std::endl;
    if (std::remove(LIVE_LOGGER_FIFO_PATH.c_str()) != 0) {
        // Ignore ENOENT (No such file or directory) if it was already removed
        if (errno != ENOENT) {
            std::cerr << "Error cleaning up named pipe: " << strerror(errno) << std::endl;
        }
    }
}


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
              if (auto joint = dynamic_cast<articulation::Joint*>(monitorable_joint)) { // Safely cast to Joint*
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
    // telemetry::logging::LiveLogger::get_instance().display(); // No longer display to main console
}

void initialize_controller(std::shared_ptr<anatomy::hand::Hand> hand) {
    // Create an instance of the CommandLineController
    // We need to pass the hand object to the controller so it can execute movements
    auto controller = control::CommandLineController(std::move(hand));
    controller.start(); // Start the controller's main loop
}

int main() {
    // 1. Create the named pipe (FIFO)
    // Remove it first if it already exists from a previous run
    std::remove(LIVE_LOGGER_FIFO_PATH.c_str());
    if (mkfifo(LIVE_LOGGER_FIFO_PATH.c_str(), 0666) == -1) {
        if (errno != EEXIST) { // EEXIST means it already exists, which is fine if we didn't remove it
            std::cerr << "Error creating named pipe: " << strerror(errno) << std::endl;
            return 1;
        }
    }
    std::atexit(cleanup_fifo); // Register cleanup function

    // 2. Launch a new terminal window to 'cat' the named pipe
    // This command is specific to macOS Terminal.app
    std::string terminal_command = "osascript -e 'tell application \"Terminal\" to do script \"cat " + LIVE_LOGGER_FIFO_PATH + "; exit\"' > /dev/null 2>&1";
    std::system(terminal_command.c_str());
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Give terminal time to open and cat

    // 3. Set LiveLogger to output to the named pipe
    telemetry::logging::LiveLogger::get_instance().set_output_path(LIVE_LOGGER_FIFO_PATH);
    // NEW: Start the LiveLogger display thread
    telemetry::logging::LiveLogger::get_instance().start_display_thread(std::chrono::seconds(1)); // Refresh every 1 second


    std::shared_ptr<anatomy::hand::Hand> hand = builder::HandBuilder::build();
    register_alert_actions();
    initialize_telemetry();
    initialize_controller(hand);

    // CRITICAL: Explicitly stop the TelemetryManager before main exits
    telemetry::TelemetryManager::stop();

    return 0;
}