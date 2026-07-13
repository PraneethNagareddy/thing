#include "control/CommandLineController.h"
#include "gestures/Gestures.h"
#include "hardware/protocol/SCS0009Protocol.h"
#include "hardware/HardwareConstants.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>

namespace control {

    void CommandLineController::print_help() {
        std::cout << "\n--- Thing Command Controller ---" << std::endl;
        std::cout << "Available commands:" << std::endl;
        std::cout << "  pinch    - Execute Pinky Pinch" << std::endl;
        std::cout << "  thumbsup - Execute Thumbs Up" << std::endl;
        std::cout << "  reset    - Reset all servos to default" << std::endl;
        std::cout << "  stats <id>    - Get real-time servo telemetry" << std::endl;
        std::cout << "  exit     - Stop the controller" << std::endl;
        std::cout << ">> ";
    }

    void CommandLineController::start() {
        if (!hand_) {
            std::cerr << "Controller error: No hand instance provided." << std::endl;
            return;
        }

        running_ = true;
        std::string input;

        while (running_) {
            print_help();
            if (!std::getline(std::cin, input)) break;

            // Normalize input to lowercase
            std::transform(input.begin(), input.end(), input.begin(), ::tolower);

            if (input == "exit" || input == "quit") {
                running_ = false;
                std::cout << "Shutting down controller..." << std::endl;
            } 
            else if (input == "pinch") {
                std::cout << "Executing Pinky Pinch..." << std::endl;
                auto movements = gestures::Gestures::pinky_pinch();
                hand_->apply(movements);
            } 
            else if (input == "thumbsup") {
                std::cout << "Executing Thumbs Up..." << std::endl;
                auto movements = gestures::Gestures::thumbs_up();
                hand_->apply(movements);
            }
            else if (input == "reset") {
                std::cout << "Resetting all servos to default positions..." << std::endl;
                auto movements = gestures::Gestures::reset_all();
                hand_->apply(movements);
            }
            else if (input.find("stats") == 0) {
                std::stringstream ss(input);
                std::string cmd;
                int id;

                if (!(ss >> cmd >> id)) {
                    std::cout << "Usage: stats <servoID>" << std::endl;
                    continue;
                }

                if (hardware::constants::DEFAULT_SCS0009_PROTOCOL) {
                    using hardware::constants::DEFAULT_SCS0009_PROTOCOL;
                    std::cout << "\n--- Servo Telemetry (ID: " << id << ") ---" << std::endl;
                    std::cout << std::fixed << std::setprecision(2);
                    std::cout << "  Position:    " << DEFAULT_SCS0009_PROTOCOL->read_present_position(id) << " steps" << std::endl;
                    std::cout << "  Load:       " << DEFAULT_SCS0009_PROTOCOL->read_present_load(id) << "°" << std::endl;
                    std::cout << "  Temperature: " << DEFAULT_SCS0009_PROTOCOL->read_temperature(id) << " °C" << std::endl;
                    std::cout << "  Voltage:     " << DEFAULT_SCS0009_PROTOCOL->read_voltage(id) << " mV" << std::endl;
                    std::cout << "------------------------------------" << std::endl;
                }
                else {
                    std::cout << "Error: No joint found with Servo ID " << id << std::endl;
                }
            }
            else {
                std::cout << "Unknown command: " << input << std::endl;
            }
        }
    }
}
