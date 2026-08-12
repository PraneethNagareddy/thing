#include "control/CommandLineController.h"
#include "gestures/Gestures.h"
#include "hardware/protocol/SCS0009Protocol.h"
#include "hardware/HardwareConstants.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <vector>

namespace control {

    void CommandLineController::print_help() {
        std::cout << "\n--- Thing Command Controller ---" << std::endl;
        std::cout << "Available commands:" << std::endl;
        std::cout << "  pinch1    - Execute Pinky Pinch" << std::endl;
        std::cout << "  pinch2    - Execute Ring Pinch" << std::endl;
        std::cout << "  pinch3    - Execute Middle Pinch" << std::endl;
        std::cout << "  pinch4    - Execute Index Pinch" << std::endl;
        std::cout << "  grab <mm> - Grab object of specified size in mm" << std::endl;
        std::cout << "  f2        - 2 finger Salue" << std::endl;
        std::cout << "  rock      - Rock On" << std::endl;
        std::cout << "  gun       - Finger Gun" << std::endl;
        std::cout << "  thumbsup  - Execute Thumbs Up" << std::endl;
        std::cout << "  reset     - Reset all servos to default" << std::endl;
        std::cout << "  web_shoot - Shoot Web" << std::endl;
        std::cout << "  rotate    - Rotate fist" << std::endl;
        std::cout << "  stream_rotate - Execute wrist rotation using movement stream" << std::endl;
        std::cout << "  show_off    - Show Off All gestures" << std::endl;
        std::cout << "  wrist <pitch_pct> <yaw_pct> - Move wrist pitch & yaw (0.0 to 1.0)" << std::endl;
        std::cout << "  pitch <pct> - Move wrist pitch (0.0 to 1.0)" << std::endl;
        std::cout << "  yaw <pct>   - Move wrist yaw (0.0 to 1.0)" << std::endl;
        std::cout << "  stats <id>    - Get real-time servo telemetry" << std::endl;
        std::cout << "  set <id> <angle> - Set individual servo angle (0-300 deg for SCS0009, 0-360 deg for STS3215)" << std::endl;
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
            else if (lower_input == "pinch1") {
                std::cout << "Executing Pinky Pinch..." << std::endl;
                auto movements = gestures::Gestures::pinky_pinch();
                hand_->apply(movements);
            }
            else if (lower_input == "pinch2") {
                std::cout << "Executing Ring Pinch..." << std::endl;
                auto movements = gestures::Gestures::ring_pinch();
                hand_->apply(movements);
            }
            else if (lower_input == "pinch3") {
                std::cout << "Executing Middle Pinch..." << std::endl;
                auto movements = gestures::Gestures::middle_pinch();
                hand_->apply(movements);
            }
            else if (lower_input == "pinch4") {
                std::cout << "Executing Index Pinch..." << std::endl;
                auto movements = gestures::Gestures::index_pinch();
                hand_->apply(movements);
            }
            else if (lower_input.find("grab") == 0) {
                std::stringstream ss(input);
                std::string cmd;
                float size_mm;
                if (ss >> cmd >> size_mm) {
                    std::cout << "Executing Grab Object for size " << size_mm << " mm..." << std::endl;
                    auto movements = gestures::Gestures::grab_object(size_mm);
                    hand_->apply(movements);
                } else {
                    std::cout << "Usage: grab <size_mm>" << std::endl;
                }
            }
            else if (lower_input.find("wrist") == 0) {
                std::stringstream ss(input);
                std::string cmd;
                float pitch_pct, yaw_pct;
                if (ss >> cmd >> pitch_pct >> yaw_pct) {
                    std::cout << "Moving Wrist Pitch to " << pitch_pct << ", Yaw to " << yaw_pct << "..." << std::endl;
                    anatomy::hand::WristMovement mov(
                        articulation::movement::Pitch{pitch_pct, 1000},
                        articulation::movement::Yaw{yaw_pct, 1000}
                    );
                    std::vector<std::variant<anatomy::hand::FingerMovement, anatomy::hand::ThumbMovement, anatomy::hand::WristMovement>> movements = { mov };
                    hand_->apply(movements);
                } else {
                    std::cout << "Usage: wrist <pitch_pct> <yaw_pct>" << std::endl;
                }
            }
            else if (lower_input.find("pitch") == 0) {
                std::stringstream ss(input);
                std::string cmd;
                float pitch_pct;
                if (ss >> cmd >> pitch_pct) {
                    std::cout << "Moving Wrist Pitch to " << pitch_pct << "..." << std::endl;
                    anatomy::hand::WristMovement mov(
                        articulation::movement::Pitch{pitch_pct, 1000}
                    );
                    std::vector<std::variant<anatomy::hand::FingerMovement, anatomy::hand::ThumbMovement, anatomy::hand::WristMovement>> movements = { mov };
                    hand_->apply(movements);
                } else {
                    std::cout << "Usage: pitch <pct>" << std::endl;
                }
            }
            else if (lower_input.find("yaw") == 0) {
                std::stringstream ss(input);
                std::string cmd;
                float yaw_pct;
                if (ss >> cmd >> yaw_pct) {
                    std::cout << "Moving Wrist Yaw to " << yaw_pct << "..." << std::endl;
                    anatomy::hand::WristMovement mov(
                        std::monostate{},
                        articulation::movement::Yaw{yaw_pct, 1000}
                    );
                    std::vector<std::variant<anatomy::hand::FingerMovement, anatomy::hand::ThumbMovement, anatomy::hand::WristMovement>> movements = { mov };
                    hand_->apply(movements);
                } else {
                    std::cout << "Usage: yaw <pct>" << std::endl;
                }
            }
            else if (lower_input == "peace") {
                std::cout << "Executing peace sign..." << std::endl;
                auto movements = gestures::Gestures::peace();
                hand_->apply(movements);
            }
            else if (lower_input == "rock") {
                std::cout << "Executing Rock On..." << std::endl;
                auto movements = gestures::Gestures::rock_on();
                hand_->apply(movements);
            }
            else if (lower_input == "gun") {
                std::cout << "Executing Finger Gun..." << std::endl;
                auto movements = gestures::Gestures::finger_gun();
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
            else if (lower_input == "web_shoot") {
                std::cout << "Shooting web..." << std::endl;
                auto movements = gestures::Gestures::spidey_web();
                hand_->apply(movements);
            }
            else if (lower_input == "rotate") {
                std::cout << "Rotating fist" << std::endl;
                auto movements = gestures::Gestures::fist_rotate();
                hand_->apply(movements);
            }
            else if (lower_input == "stream_rotate") {
                std::cout << "Streaming continuous wrist rotation..." << std::endl;
                auto stream = gestures::Gestures::rotate_wrist_stream();
                hand_->apply_stream(stream);
            }
            else if (lower_input == "show_off") {
                std::cout << "Showing Off..." << std::endl;
                auto movements = gestures::Gestures::pinky_pinch();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000));
                movements = gestures::Gestures::ring_pinch();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000));
                movements = gestures::Gestures::middle_pinch();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000));
                movements = gestures::Gestures::index_pinch();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000));
                movements = gestures::Gestures::reset_all();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000));
                movements = gestures::Gestures::rock_on();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1500));
                movements = gestures::Gestures::reset_all();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000));
                movements = gestures::Gestures::peace();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1500));
                movements = gestures::Gestures::finger_gun();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1500));
                movements = gestures::Gestures::thumbs_up();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1500));
                movements = gestures::Gestures::reset_all();
                hand_->apply(movements);
                std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000));
            }
            else if (lower_input.find("stats") == 0) {
                std::stringstream ss(input);
                std::string cmd;
                int id;

                if (!(ss >> cmd >> id)) {
                    std::cout << "Usage: stats <servoID>" << std::endl;
                    continue;
                }

                if (id >= 11) {
                    using hardware::constants::DEFAULT_STS3215_PROTOCOL;
                    if (DEFAULT_STS3215_PROTOCOL) {
                        std::cout << "\n--- Servo Telemetry (ID: " << id << " - STS3215) ---" << std::endl;
                        std::cout << std::fixed << std::setprecision(2);
                        std::cout << "  Position:    " << DEFAULT_STS3215_PROTOCOL->read_present_position(id) << " steps" << std::endl;
                        float load_pct = static_cast<float>(DEFAULT_STS3215_PROTOCOL->read_present_load(id) & 0x3FF) / 10.23f;
                        std::cout << "  Load:        " << load_pct << " %" << std::endl;
                        std::cout << "  Temperature: " << static_cast<int>(DEFAULT_STS3215_PROTOCOL->read_temperature(id)) << " °C" << std::endl;
                        std::cout << "  Voltage:     " << DEFAULT_STS3215_PROTOCOL->read_voltage(id) << " mV" << std::endl;
                        std::cout << "------------------------------------" << std::endl;
                    }
                } else if (hardware::constants::DEFAULT_SCS0009_PROTOCOL) {
                    using hardware::constants::DEFAULT_SCS0009_PROTOCOL;
                    std::cout << "\n--- Servo Telemetry (ID: " << id << " - SCS0009) ---" << std::endl;
                    std::cout << std::fixed << std::setprecision(2);
                    std::cout << "  Position:    " << DEFAULT_SCS0009_PROTOCOL->read_present_position(id) << " steps" << std::endl;
                    float load_pct = static_cast<float>(DEFAULT_SCS0009_PROTOCOL->read_present_load(id) & 0x3FF) / 10.23f;
                    std::cout << "  Load:        " << load_pct << " %" << std::endl;
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
                float angle_degrees;

                if (!(ss >> cmd >> id >> angle_degrees)) {
                    std::cout << "Usage: set <servoID> <angle>" << std::endl;
                    continue;
                }

                if (id >= 11) {
                    using hardware::constants::DEFAULT_STS3215_PROTOCOL;
                    if (DEFAULT_STS3215_PROTOCOL) {
                        uint16_t steps = static_cast<uint16_t>((angle_degrees / 360.0f) * 4095.0f);
                        std::cout << "Setting STS3215 servo " << id << " to angle " << angle_degrees << " degrees (" << steps << " steps)..." << std::endl;
                        if (DEFAULT_STS3215_PROTOCOL->move_steps(id, steps)) {
                            std::cout << "Servo " << id << " moved successfully." << std::endl;
                        } else {
                            std::cout << "Failed to move servo " << id << "." << std::endl;
                        }
                    }
                } else if (hardware::constants::DEFAULT_SCS0009_PROTOCOL) {
                    using hardware::constants::DEFAULT_SCS0009_PROTOCOL;
                    uint16_t steps = static_cast<uint16_t>((angle_degrees / 300.0f) * 1023.0f);
                    std::cout << "Setting SCS0009 servo " << id << " to angle " << angle_degrees << " degrees (" << steps << " steps)..." << std::endl;
                    if (DEFAULT_SCS0009_PROTOCOL->move_steps(id, steps)) {
                        std::cout << "Servo " << id << " moved successfully." << std::endl;
                    } else {
                        std::cout << "Failed to move servo " << id << "." << std::endl;
                    }
                } else {
                    std::cout << "Error: Servo protocol not initialized." << std::endl;
                }
            }
            else {
                std::cout << "Unknown command: " << input << std::endl;
            }
        }
    }
}