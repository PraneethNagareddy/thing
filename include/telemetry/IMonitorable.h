//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_IMONITORABLE_H
#define THING_IMONITORABLE_H
#include <concepts>

#include "Reading.h"

namespace telemetry {

    template <typename M>
    requires std::derived_from<M, BaseReading>

    class IMonitorable {
    private:
        const int frequency_hz_;
        static constexpr int DEFAULT_FREQUENCY_HZ = 10;
    public:
        IMonitorable(const int frequency_hz = 10) : frequency_hz_(frequency_hz){};
        virtual ~IMonitorable() = default;
        virtual const M poll() = 0;
        const int get_frequency_hz() const {
            return frequency_hz_;
        }
        const int get_polling_interval_ms() const {
            return (frequency_hz_ > 0) ? 1000 / frequency_hz_ : 1000 / DEFAULT_FREQUENCY_HZ;
        }
        void start_monitoring();
        void stop_monitoring();
    };
}

#endif //THING_IMONITORABLE_H
