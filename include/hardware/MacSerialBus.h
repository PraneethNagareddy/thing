#ifndef THING_MACSERIALBUS_H
#define THING_MACSERIALBUS_H

#include "SerialBus.h"
#include <string>

namespace hardware {

    class MacSerialBus : public SerialBus {
    private:
        int fd_;
        std::string device_;
        int baudrate_;

    public:
        MacSerialBus(std::string device, int baudrate);
        ~MacSerialBus() override;

        bool open() override;
        void close() override;

        bool write(const std::vector<uint8_t>& data) override;
        std::vector<uint8_t> read(size_t length, int timeout_ms = 100) override;
        void flush() override;

        bool is_open() const override { return fd_ != -1; }
    };

}

#endif // THING_MACSERIALBUS_H