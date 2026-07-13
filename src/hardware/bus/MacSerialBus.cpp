#include "../../../include/hardware/bus/MacSerialBus.h"
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

        // Ensure we are in blocking mode for standard operations after open
        if (fcntl(fd_, F_SETFL, 0) == -1) return false;

        struct termios options{};
        if (tcgetattr(fd_, &options) == -1) return false;

        // cfmakeraw sets the most critical flags for high-speed binary protocols
        cfmakeraw(&options);

        options.c_cflag |= (CLOCAL | CREAD);
        options.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
        options.c_cc[VMIN] = 0;  // Non-blocking read (we use select)
        options.c_cc[VTIME] = 0;
        
        // Set standard baud rates as a fallback
        speed_t std_speed = B9600;
        if (baudrate_ == 115200) std_speed = B115200;
        cfsetispeed(&options, std_speed);
        cfsetospeed(&options, std_speed);

        if (tcsetattr(fd_, TCSANOW, &options) == -1) return false;

        // macOS specific: set high-speed baud rates (1Mbps) via ioctl
        speed_t speed = static_cast<speed_t>(baudrate_);
        ioctl(fd_, IOSSIOSPEED, &speed);

        tcflush(fd_, TCIOFLUSH); // Clear any power-on junk or noise

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
        if (sent != static_cast<ssize_t>(data.size())) return false;
        
        // In half-duplex TTL, we MUST wait for the bytes to physically leave the 
        // hardware buffer before we can safely flush the echo or start reading.
        tcdrain(fd_); 
        return true;
    }

    std::vector<uint8_t> MacSerialBus::read(size_t length, int timeout_ms) {
        std::vector<uint8_t> buffer;
        if (fd_ == -1) return buffer;

        buffer.resize(length);
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

            // Efficiency: Read as many bytes as possible in one system call
            // instead of byte-by-byte, which is prone to jitter at 1Mbps.
            ssize_t n = ::read(fd_, &buffer[total_read], length - total_read);
            if (n > 0) {
                total_read += n;
            } else if (n < 0 && errno != EAGAIN) {
                break; // Real error
            }
        }
        
        buffer.resize(total_read);
        return buffer;
    }

    void MacSerialBus::flush() {
        if (fd_ != -1) tcflush(fd_, TCIOFLUSH);
    }
}