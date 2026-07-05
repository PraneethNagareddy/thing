//
// Created by Nagareddy on 06/07/26.
//

#ifndef THING_FEETECHSERVO_H
#define THING_FEETECHSERVO_H

#include "SerialBus.h"
#include <memory>

namespace hardware {

    class FeetechServo {
    protected:
        std::shared_ptr<SerialBus> bus_;

        static constexpr uint8_t START_BYTE = 0xFF;
        static constexpr uint8_t INST_READ  = 0x02;
        static constexpr uint8_t INST_WRITE = 0x03;

        uint8_t calculate_checksum(uint8_t id, uint8_t length, uint8_t inst, const std::vector<uint8_t>& params) {
            uint32_t sum = id + length + inst;
            for (uint8_t p : params) sum += p;
            return static_cast<uint8_t>(~(sum & 0xFF));
        }

        bool send_packet(uint8_t id, uint8_t inst, const std::vector<uint8_t>& params) {
            uint8_t length = static_cast<uint8_t>(params.size() + 2);
            std::vector<uint8_t> packet = {START_BYTE, START_BYTE, id, length, inst};
            packet.insert(packet.end(), params.begin(), params.end());
            packet.push_back(calculate_checksum(id, length, inst, params));

            bus_->flush();
            return bus_->write(packet);
        }

        std::vector<uint8_t> receive_packet(uint8_t id, size_t expected_params) {
            // Expected: FF FF ID LEN ERR PARAMS... CHK
            size_t total_len = expected_params + 6;
            auto response = bus_->read(total_len);

            if (response.size() < total_len) return {};
            if (response[0] != START_BYTE || response[1] != START_BYTE) return {};
            if (response[2] != id) return {};

            // Checksum verification
            uint8_t len = response[3];
            uint8_t err = response[4];
            std::vector<uint8_t> params(response.begin() + 5, response.end() - 1);

            uint32_t sum = id + len + err;
            for (uint8_t p : params) sum += p;
            if (static_cast<uint8_t>(~(sum & 0xFF)) != response.back()) return {};

            return params;
        }

    public:
        explicit FeetechServo(std::shared_ptr<SerialBus> bus) : bus_(std::move(bus)) {}

        bool write8(uint8_t id, uint8_t reg, uint8_t value) {
            return send_packet(id, INST_WRITE, {reg, value});
        }

        bool write16(uint8_t id, uint8_t reg, uint16_t value) {
            return send_packet(id, INST_WRITE, {reg, static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value & 0xFF)});
        }

        int16_t read16(uint8_t id, uint8_t reg) {
            if (!send_packet(id, INST_READ, {reg, 2})) return -1;
            auto res = receive_packet(id, 2);
            if (res.size() != 2) return -1;
            return static_cast<int16_t>((res[0] << 8) | res[1]);
        }

        int8_t read8(uint8_t id, uint8_t reg) {
            if (!send_packet(id, INST_READ, {reg, 1})) return -1;
            auto res = receive_packet(id, 1);
            if (res.size() != 1) return -1;
            return static_cast<int8_t>(res[0]);
        }
    };
}

#endif //THING_FEETECHSERVO_H