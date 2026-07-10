#include "../../../include/hardware/bus/LinuxSerialBus.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>

namespace hardware {

    LinuxSerialBus::LinuxSerialBus(std::string device, int baudrate)
        : fd_(-1), device_(std::move(device)), baudrate_(baudrate) {}

    LinuxSerialBus::~LinuxSerialBus() {
        close();
    }

    bool LinuxSerialBus::open() {
        fd_ = ::open(device_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd_ == -1) return false;

        struct termios options{};
        tcgetattr(fd_, &options);

        speed_t speed;
        switch (baudrate_) {
            case 9600:   speed = B9600;   break;
            case 115200: speed = B115200; break;
            case 1000000: speed = B115200; break;
            default:     speed = B115200; break;
        }

        cfsetispeed(&options, speed);
        cfsetospeed(&options, speed);

        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        options.c_iflag &= ~(INPCK | ISTRIP);
        options.c_oflag &= ~OPOST;

        tcsetattr(fd_, TCSANOW, &options);
        return true;
    }

    void LinuxSerialBus::close() {
        if (fd_ != -1) {
            ::close(fd_);
            fd_ = -1;
        }
    }

    bool LinuxSerialBus::write(const std::vector<uint8_t>& data) {
        if (fd_ == -1) return false;
        ssize_t sent = ::write(fd_, data.data(), data.size());
        return sent == static_cast<ssize_t>(data.size());
    }

    std::vector<uint8_t> LinuxSerialBus::read(size_t length, int timeout_ms) {
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
            if (rv <= 0) break; // Timeout or error

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

    void LinuxSerialBus::flush() {
        if (fd_ != -1) tcflush(fd_, TCIOFLUSH);
    }
}