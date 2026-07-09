#ifndef THING_HARDWARECONSTANTS_H
#define THING_HARDWARECONSTANTS_H

#include <memory>
#include <string>

#include "bus/MacSerialBus.h"
#include "protocol/DummySCS0009ConsoleProtocol.h"

namespace hardware::constants {

    inline const std::string DEFAULT_MAC_PORT = "/dev/cu.usbserial-110";
    inline constexpr int DEFAULT_BAUD_RATE = 1000000; // 1 Mbps

    /**
     * Centralized instance of the MacSerialBus.
     * Using inline allows this to be shared across all translation units 
     * that include this header.
     */
    inline auto MAC_SERIAL_BUS = std::make_shared<hardware::MacSerialBus>(
        DEFAULT_MAC_PORT, 
        DEFAULT_BAUD_RATE
    );
    inline auto DEFAULT_SERIAL_BUS = MAC_SERIAL_BUS;


    inline auto SCS0009_PROTOCOL = std::make_shared<hardware::SCS0009Protocol>(DEFAULT_SERIAL_BUS);
    inline auto SCS0009_DUMMY_CONSOLE_PROTOCOL = std::make_shared<hardware::DummySCS0009ConsoleProtocol>(DEFAULT_SERIAL_BUS);

    inline auto DEFAULT_SCS0009_PROTOCOL = SCS0009_DUMMY_CONSOLE_PROTOCOL;
}

#endif //THING_HARDWARECONSTANTS_H