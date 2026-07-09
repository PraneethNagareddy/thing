#ifndef THING_LINUXSERIALBUS_H
#define THING_LINUXSERIALBUS_H

#include "SerialBus.h"
#include <string>

namespace hardware {

    class LinuxSerialBus : public SerialBus {
    private:
        int fd_;
        std::string device_;
        int baudrate_;

    public:
        LinuxSerialBus(std::string device, int baudrate);
        ~LinuxSerialBus() override;

        bool open() override;
        void close() override;

        bool write(const std::vector<uint8_t>& data) override;
        std::vector<uint8_t> read(size_t length, int timeout_ms = 100) override;
        void flush() override;

        bool is_open() const override { return fd_ != -1; }
    };

}

#endif //THING_LINUXSERIALBUS_H