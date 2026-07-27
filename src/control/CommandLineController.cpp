#include "control/CommandLineController.h"
#include "gestures/Gestures.h"
#include "hardware/protocol/SCS0009Protocol.h"
#include "hardware/HardwareConstants.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <vector>

namespace control {

    void CommandLineController::print_help() {
        std::cout << "\n--- Thing Command Controller ---" << std::endl;
        std::cout << "Available commands:" << std::endl;
        std::cout << "  pinch    - Execute Pinky Pinch" << std::endl;
        std::cout << "  thumbsup - Execute Thumbs Up" << std::endl;
        std::cout << "  reset    - Reset all servos to default" << std::endl;
        std::cout << "  stats <id>    - Get real-time servo telemetry" << std::endl;
        std::cout << "  set <id> <angle> - Set individual servo angle (0-300 degrees)" << std::endl;
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

            // Normalize input to lowercase for command comparison, but keep original for parsing
            std::string lower_input = input;
            std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);

            if (lower_input == "exit" || lower_input == "quit") {
                running_ = false;
                std::cout << "Shutting down controller..." << std::endl;
            } 
            else if (lower_input == "pinch") {
                std::cout << "Executing Pinky Pinch..." << std::endl;
                auto movements = gestures::Gestures::pinky_pinch();
                hand_->apply(movements);
            } 
            else if (lower_input == "thumbsup") {
                std::cout << "Executing Thumbs Up..." << std::endl;
                auto movements = gestures::Gestures::thumbs_up();
                hand_->apply(movements);
            }
            else if (lower_input == "reset") {
                std::cout << "Resetting all servos to default positions..." << std::endl;
                auto movements = gestures::Gestures::reset_all();
                hand_->apply(movements);
            }
            else if (lower_input.find("stats") == 0) {
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
                    // Mask Bit 10 (direction) and scale 0-1023 to 0-100%
                    float load_pct = static_cast<float>(DEFAULT_SCS0009_PROTOCOL->read_present_load(id) & 0x3FF) / 10.23f;
                    std::cout << "  Load:        " << load_pct << " %" << std::endl;
                    // Cast int8_t to int so cout prints the number 25 instead of the ASCII character
                    std::cout << "  Temperature: " << static_cast<int>(DEFAULT_SCS0009_PROTOCOL->read_temperature(id)) << " °C" << std::endl;
                    std::cout << "  Voltage:     " << DEFAULT_SCS0009_PROTOCOL->read_voltage(id) << " mV" << std::endl;
                    std::cout << "------------------------------------" << std::endl;
                }
                else {
                    std::cout << "Error: No joint found with Servo ID " << id << std::endl;
                }
            }
            else if (lower_input.find("set") == 0) {
                std::stringstream ss(input);
                std::string cmd;
                int id;
                float angle_degrees; // Use float for angle to allow decimal input

                if (!(ss >> cmd >> id >> angle_degrees)) {
                    std::cout << "Usage: set <servoID> <angle>" << std::endl;
                    continue;
                }

                if (hardware::constants::DEFAULT_SCS0009_PROTOCOL) {
                    using hardware::constants::DEFAULT_SCS0009_PROTOCOL;
                    // Convert angle from degrees (0-300) to steps (0-1023)
                    uint16_t steps = static_cast<uint16_t>((angle_degrees / 300.0f) * 1023.0f);

                    std::cout << "Setting servo " << id << " to angle " << angle_degrees << " degrees (" << steps << " steps)..." << std::endl;
                    if (DEFAULT_SCS0009_PROTOCOL->move_steps(id, steps)) {
                        std::cout << "Servo " << id << " moved successfully." << std::endl;
                    } else {
                        std::cout << "Failed to move servo " << id << "." << std::endl;
                    }
                } else {
                    std::cout << "Error: SCS0009Protocol not initialized." << std::endl;
                }
            }
            else {
                std::cout << "Unknown command: " << input << std::endl;
            }
        }
    }
}