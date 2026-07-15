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
        mutable std::mutex transaction_mutex_;

        static constexpr uint8_t START_BYTE = 0xFF;
        static constexpr uint8_t INST_READ  = 0x02;
        static constexpr uint8_t INST_WRITE = 0x03;

        static uint8_t calculate_checksum(uint8_t id, uint8_t length, uint8_t inst, const std::vector<uint8_t>& params) ;

        bool send_packet(uint8_t id, uint8_t inst, const std::vector<uint8_t>& params) const;

        std::vector<uint8_t> receive_packet(uint8_t id, size_t expected_params) const;

    public:
        explicit FeetechServoProtocol(std::shared_ptr<SerialBus> bus) : bus_(std::move(bus)) {
            bus_->open();
        }

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