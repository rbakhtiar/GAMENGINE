#include <gamengine/event_bus.hpp>

namespace GAMENGINE {

void EventBus::clear() {
    std::lock_guard<std::mutex> lock(mutex);
    listeners.clear();
}

std::size_t EventBus::listener_count() const {
    std::lock_guard<std::mutex> lock(mutex);

    std::size_t count = 0;

    for (const auto& [type, handlers] : listeners)
        count += handlers.size();

    return count;
}

}
