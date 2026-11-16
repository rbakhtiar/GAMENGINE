#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <cstddef>

namespace GAMENGINE {

class EventBus {
public:
    template<typename Event>
    using Callback = std::function<void(const Event&)>;

    template<typename Event>
    void subscribe(Callback<Event> callback) {
        std::lock_guard<std::mutex> lock(mutex);

        auto& handlers = listeners[typeid(Event)];

        handlers.push_back(
            [fn = std::move(callback)](const void* event) {
                fn(*static_cast<const Event*>(event));
            }
        );
    }

    template<typename Event>
    void publish(const Event& event) {
        std::vector<std::function<void(const void*)>> handlers;

        {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = listeners.find(typeid(Event));
            if (it == listeners.end()) return;

            handlers = it->second;
        }

        for (const auto& handler : handlers)
            handler(&event);
    }

    void clear();
    std::size_t listener_count() const;

private:
    mutable std::mutex mutex;

    std::unordered_map<
        std::type_index,
        std::vector<std::function<void(const void*)>>
    > listeners;
};

}
