#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>

namespace GAMENGINE {

using KeyCode = std::int32_t;

class InputMap {
public:
    void bind(
        const std::string& action,
        KeyCode key
    ) {
        bindings[action].push_back(key);
    }

    void unbind(const std::string& action) {
        bindings.erase(action);
    }

    bool is_bound(
        const std::string& action,
        KeyCode key
    ) const {
        auto it = bindings.find(action);

        if (it == bindings.end())
            return false;

        for (KeyCode value : it->second) {
            if (value == key)
                return true;
        }

        return false;
    }

    bool pressed(
        const std::string& action,
        KeyCode key
    ) const {
        return is_bound(action, key) &&
               pressedKeys.contains(key);
    }

    void set_key_state(
        KeyCode key,
        bool down
    ) {
        if (down)
            pressedKeys.insert(key);
        else
            pressedKeys.erase(key);
    }

    void clear_states() {
        pressedKeys.clear();
    }

private:
    std::unordered_map<
        std::string,
        std::vector<KeyCode>
    > bindings;

    std::unordered_set<KeyCode> pressedKeys;
};

}
