#include "hardware/MacSerialBus.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>
#include <cstring>

namespace hardware {

    MacSerialBus::MacSerialBus(std::string device, int baudrate)
        : fd_(-1), device_(std::move(device)), baudrate_(baudrate) {}

    MacSerialBus::~MacSerialBus() {
        MacSerialBus::close();
    }

    bool MacSerialBus::open() {
        // O_NONBLOCK is important on macOS to avoid hanging on open if DCD is not asserted
        fd_ = ::open(device_.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (fd_ == -1) return false;

        // Remove non-blocking flag for normal operation
        fcntl(fd_, F_SETFL, 0);

        struct termios options{};
        tcgetattr(fd_, &options);

        // Set basic attributes
        cfmakeraw(&options);
        options.c_cflag |= (CLOCAL | CREAD | CS8);
        options.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
        
        // Set standard baud rates first
        speed_t std_speed = B9600;
        if (baudrate_ == 115200) std_speed = B115200;
        
        cfsetispeed(&options, std_speed);
        cfsetospeed(&options, std_speed);

        tcsetattr(fd_, TCSANOW, &options);

        // macOS specific: set high-speed baud rates via ioctl
        speed_t speed = static_cast<speed_t>(baudrate_);
        if (ioctl(fd_, IOSSIOSPEED, &speed) == -1) {
            // If 1Mbps fails, we fall back to what was set in termios
        }

        return true;
    }

    void MacSerialBus::close() {
        if (fd_ != -1) {
            ::close(fd_);
            fd_ = -1;
        }
    }

    bool MacSerialBus::write(const std::vector<uint8_t>& data) {
        if (fd_ == -1) return false;
        ssize_t sent = ::write(fd_, data.data(), data.size());
        return sent == static_cast<ssize_t>(data.size());
    }

    std::vector<uint8_t> MacSerialBus::read(size_t length, int timeout_ms) {
        std::vector<uint8_t> buffer;
        if (fd_ == -1) return buffer;

        buffer.reserve(length);
        size_t total_read = 0;

        while (total_read < length) {
            fd_set set;
            struct timeval timeout{};

            FD_ZERO(&set);
            FD_SET(fd_, &set);

            timeout.tv_sec = 0;
            timeout.tv_usec = timeout_ms * 1000;

            int rv = select(fd_ + 1, &set, nullptr, nullptr, &timeout);
            if (rv <= 0) break; 

            uint8_t byte;
            if (::read(fd_, &byte, 1) > 0) {
                buffer.push_back(byte);
                total_read++;
            } else {
                break;
            }
        }
        return buffer;
    }

    void MacSerialBus::flush() {
        if (fd_ != -1) tcflush(fd_, TCIOFLUSH);
    }
}