#ifndef THING_ICONTROLLER_H
#define THING_ICONTROLLER_H

namespace control {

    class IController {
    public:
        virtual ~IController() = default;

        /**
         * Starts the controller's main loop to listen for commands.
         */
        virtual void start() = 0;
    };
}

#endif //THING_ICONTROLLER_H