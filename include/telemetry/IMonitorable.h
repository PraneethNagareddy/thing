//
// Created by Nagareddy on 29/06/26.
//

#ifndef THING_IMONITERABLE_H
#define THING_IMONITERABLE_H
#include <concepts>

#include "Reading.h"

namespace telemetry {

    template <typename M>
    requires std::derived_from<M, BaseReading>

    class IMonitorable {
    private:
        const int frequency_hz_;
    public:
        IMonitorable(const int frequency_hz = 25) : frequency_hz_(frequency_hz){};
        virtual ~IMonitorable() = default;
        virtual const M poll() = 0;
    };
}

#endif //THING_IMONITERABLE_H
