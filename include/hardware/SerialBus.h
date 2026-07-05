//
// Created by Nagareddy on 06/07/26.
//

#ifndef THING_SERIALBUS_H
 #define THING_SERIALBUS_H

 #include <vector>
 #include <cstdint>

 namespace hardware {

     /**
      * Abstract interface for the Serial Communication Hardware.
      * Implementation should handle the half-duplex switching (TX/RX).
      */
     class SerialBus {
     public:
         virtual ~SerialBus() = default;

        virtual bool open() = 0;
        virtual void close() = 0;
        virtual bool is_open() const = 0;

         virtual bool write(const std::vector<uint8_t>& data) = 0;
         virtual std::vector<uint8_t> read(size_t length, int timeout_ms = 100) = 0;
         virtual void flush() = 0;
     };

 }

#endif //THING_SERIALBUS_H