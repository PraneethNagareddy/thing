//
// Created by Nagareddy on 06/07/26.
//

#ifndef THING_FEETECHSERVOPROTOCOL_H
#define THING_FEETECHSERVOPROTOCOL_H

#include "../bus/SerialBus.h"
#include <memory>
#include <vector>
#include <mutex>

namespace hardware {

    class FeetechServoProtocol {
    protected:
        std::shared_ptr<SerialBus> bus_;
        // Shared bus-level mutex. When two protocol objects share the same
        // physical SerialBus (e.g. SCS0009Protocol + STS3215Protocol on one
        // UART), they must also share this mutex so that their transactions
        // are fully serialized on the wire.
        std::shared_ptr<std::mutex> bus_mutex_;

        static constexpr uint8_t START_BYTE = 0xFF;
        static constexpr uint8_t INST_READ  = 0x02;
        static constexpr uint8_t INST_WRITE = 0x03;

        static uint8_t calculate_checksum(uint8_t id, uint8_t length, uint8_t inst, const std::vector<uint8_t>& params);

        bool send_packet(uint8_t id, uint8_t inst, const std::vector<uint8_t>& params) const;

        std::vector<uint8_t> receive_packet(uint8_t id, size_t expected_params) const;

    public:
        // Primary constructor: accepts an externally-owned mutex so that two
        // protocol objects that share the same bus can pass the same mutex
        // instance and thereby serialize all traffic through one lock.
        FeetechServoProtocol(std::shared_ptr<SerialBus> bus,
                             std::shared_ptr<std::mutex> bus_mutex)
            : bus_(std::move(bus)), bus_mutex_(std::move(bus_mutex)) {
            bus_->open();
        }

        // Convenience constructor: creates its own private mutex.
        // Suitable when only one protocol object is ever talking to this bus.
        explicit FeetechServoProtocol(std::shared_ptr<SerialBus> bus)
            : FeetechServoProtocol(std::move(bus), std::make_shared<std::mutex>()) {}

        virtual ~FeetechServoProtocol() {
            bus_->close();
        }

        bool is_connected() const { return bus_ && bus_->is_open(); }

        bool write8(uint8_t id, uint8_t reg, uint8_t value) const;

        bool write16(uint8_t id, uint8_t reg, uint16_t value) const;

        int16_t read16(uint8_t id, uint8_t reg) const;

        int8_t read8(uint8_t id, uint8_t reg) const;
    };
}

#endif //THING_FEETECHSERVOPROTOCOL_H