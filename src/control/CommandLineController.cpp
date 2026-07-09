#include "control/CommandLineController.h"
#include "gestures/Gestures.h"
#include <iostream>
#include <string>
#include <algorithm>

namespace control {

    void CommandLineController::print_help() {
        std::cout << "\n--- Thing Command Controller ---" << std::endl;
        std::cout << "Available commands:" << std::endl;
        std::cout << "  pinch    - Execute Pinky Pinch" << std::endl;
        std::cout << "  thumbsup - Execute Thumbs Up" << std::endl;
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
            else {
                std::cout << "Unknown command: " << input << std::endl;
            }
        }
    }
}