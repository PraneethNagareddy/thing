#ifndef THING_COMMANDLINECONTROLLER_H
#define THING_COMMANDLINECONTROLLER_H

#include "IController.h"
#include "anatomy/Hand.h"
#include <memory>

namespace control {

    class CommandLineController : public IController {
    private:
        std::shared_ptr<anatomy::hand::Hand> hand_;
        bool running_;

        void print_help();

    public:
        explicit CommandLineController(std::shared_ptr<anatomy::hand::Hand> hand)
            : hand_(std::move(hand)), running_(false) {}

        /**
         * Loops and waits for string input from the user.
         */
        void start() override;
    };
}

#endif //THING_COMMANDLINECONTROLLER_H