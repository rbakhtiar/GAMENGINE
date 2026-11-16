#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <cstdint>
#include <mutex>

namespace GAMENGINE {

struct PerformanceSample {
    double lastMilliseconds = 0.0;
    double averageMilliseconds = 0.0;
    std::uint64_t calls = 0;
};

class Diagnostics {
public:
    void begin(const std::string& name);

    void end(const std::string& name);

    PerformanceSample get(
        const std::string& name
    ) const;

    void increment(
        const std::string& counter,
        std::uint64_t amount = 1
    );

    std::uint64_t counter(
        const std::string& name
    ) const;

    void reset();

private:
    using Clock = std::chrono::steady_clock;

    mutable std::mutex mutex;

    std::unordered_map<
        std::string,
        Clock::time_point
    > activeTimers;

    std::unordered_map<
        std::string,
        PerformanceSample
    > samples;

    std::unordered_map<
        std::string,
        std::uint64_t
    > counters;
};

}
