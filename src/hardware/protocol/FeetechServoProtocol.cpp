#include "hardware/protocol/FeetechServoProtocol.h"
#include <numeric>

namespace hardware {

    uint8_t FeetechServoProtocol::calculate_checksum(uint8_t id, uint8_t length, uint8_t inst, const std::vector<uint8_t>& params) {
        uint32_t sum = id + length + inst;
        for (uint8_t p : params) sum += p;
        return static_cast<uint8_t>(~(sum & 0xFF));
    }

    bool FeetechServoProtocol::send_packet(uint8_t id, uint8_t inst, const std::vector<uint8_t>& params) const {
        if (!bus_) return false;

        uint8_t length = static_cast<uint8_t>(params.size() + 2);
        std::vector<uint8_t> packet = {START_BYTE, START_BYTE, id, length, inst};
        packet.insert(packet.end(), params.begin(), params.end());
        packet.push_back(calculate_checksum(id, length, inst, params));

        bus_->flush();
        return bus_->write(packet);
    }

    std::vector<uint8_t> FeetechServoProtocol::receive_packet(uint8_t id, size_t expected_params) const {
        if (!bus_) return {};

        // Response format: FF FF ID LEN ERR PARAMS... CHK
        size_t total_expected = expected_params + 6;
        auto response = bus_->read(total_expected);

        if (response.size() < total_expected) return {};
        if (response[0] != START_BYTE || response[1] != START_BYTE) return {};
        if (response[2] != id) return {};

        uint8_t len = response[3];
        uint8_t err = response[4];
        std::vector<uint8_t> params(response.begin() + 5, response.end() - 1);

        // Verify checksum
        uint32_t sum = id + len + err;
        for (uint8_t p : params) sum += p;
        if (static_cast<uint8_t>(~(sum & 0xFF)) != response.back()) return {};

        return params;
    }

    bool FeetechServoProtocol::write8(uint8_t id, uint8_t reg, uint8_t value) const {
        return send_packet(id, INST_WRITE, {reg, value});
    }

    bool FeetechServoProtocol::write16(uint8_t id, uint8_t reg, uint16_t value) const {
        // Feetech uses Big-Endian (High byte first)
        return send_packet(id, INST_WRITE, {reg, static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value & 0xFF)});
    }

    int16_t FeetechServoProtocol::read16(uint8_t id, uint8_t reg) const {
        if (!send_packet(id, INST_READ, {reg, 2})) return -1;
        auto res = receive_packet(id, 2);
        if (res.size() != 2) return -1;
        return static_cast<int16_t>((res[0] << 8) | res[1]);
    }

}