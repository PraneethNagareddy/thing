#ifndef THING_HARDWARECONSTANTS_H
#define THING_HARDWARECONSTANTS_H

#include <memory>
#include <string>

#include "bus/MacSerialBus.h"
#include "protocol/DummySCS0009ConsoleProtocol.h"
#include "protocol/STS3215Protocol.h"

namespace hardware::constants {

    inline const std::string DEFAULT_MAC_PORT = "/dev/cu.usbserial-1320";
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


    /**
     * Shared bus-level mutex. Any protocols that communicate over DEFAULT_SERIAL_BUS
     * must share this mutex to prevent interleaved packets.
     */
    inline auto SERIAL_BUS_MUTEX = std::make_shared<std::mutex>();

    inline auto SCS0009_PROTOCOL = std::make_shared<hardware::SCS0009Protocol>(DEFAULT_SERIAL_BUS, SERIAL_BUS_MUTEX);
    inline auto SCS0009_DUMMY_CONSOLE_PROTOCOL = std::make_shared<hardware::DummySCS0009ConsoleProtocol>(DEFAULT_SERIAL_BUS);

    inline auto DEFAULT_SCS0009_PROTOCOL = SCS0009_PROTOCOL;

    inline auto STS3215_PROTOCOL = std::make_shared<hardware::STS3215Protocol>(DEFAULT_SERIAL_BUS, SERIAL_BUS_MUTEX);
    inline auto DEFAULT_STS3215_PROTOCOL = STS3215_PROTOCOL;
}

#endif //THING_HARDWARECONSTANTS_H