#ifndef THING_HARDWARECONSTANTS_H
#define THING_HARDWARECONSTANTS_H

#include <memory>
#include <string>

#include "bus/MacSerialBus.h"

namespace hardware::constants {

    // Default configuration for the Feetech SCS series on macOS
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
}

#endif //THING_HARDWARECONSTANTS_H