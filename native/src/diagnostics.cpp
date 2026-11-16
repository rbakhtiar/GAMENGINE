#include <gamengine/diagnostics.hpp>
#include <stdexcept>

namespace GAMENGINE {

void Diagnostics::begin(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex);

    activeTimers[name] = Clock::now();
}

void Diagnostics::end(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex);

    auto it = activeTimers.find(name);

    if (it == activeTimers.end())
        throw std::runtime_error(
            "Diagnostics: timer was not started: " + name
        );

    double elapsed =
        std::chrono::duration<double, std::milli>(
            Clock::now() - it->second
        ).count();

    auto& sample = samples[name];

    ++sample.calls;

    sample.lastMilliseconds = elapsed;

    sample.averageMilliseconds +=
        (elapsed - sample.averageMilliseconds) /
        static_cast<double>(sample.calls);

    activeTimers.erase(it);
}

PerformanceSample Diagnostics::get(
    const std::string& name
) const {
    std::lock_guard<std::mutex> lock(mutex);

    auto it = samples.find(name);

    if (it == samples.end())
        return {};

    return it->second;
}

void Diagnostics::increment(
    const std::string& name,
    std::uint64_t amount
) {
    std::lock_guard<std::mutex> lock(mutex);

    counters[name] += amount;
}

std::uint64_t Diagnostics::counter(
    const std::string& name
) const {
    std::lock_guard<std::mutex> lock(mutex);

    auto it = counters.find(name);

    return it == counters.end() ? 0 : it->second;
}

void Diagnostics::reset() {
    std::lock_guard<std::mutex> lock(mutex);

    activeTimers.clear();
    samples.clear();
    counters.clear();
}

}
