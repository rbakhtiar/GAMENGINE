
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace gamengine {

template<typename T>
class ResourceRegistry {
public:
    using Resource = std::shared_ptr<T>;

    bool add(const std::string& id, Resource resource) {
        if (id.empty() || !resource)
            return false;

        return resources_.emplace(id, std::move(resource)).second;
    }

    bool replace(const std::string& id, Resource resource) {
        if (id.empty() || !resource)
            return false;

        resources_[id] = std::move(resource);
        return true;
    }

    Resource get(const std::string& id) const {
        auto it = resources_.find(id);

        if (it == resources_.end())
            return nullptr;

        return it->second;
    }

    bool contains(const std::string& id) const {
        return resources_.find(id) != resources_.end();
    }

    bool remove(const std::string& id) {
        return resources_.erase(id) > 0;
    }

    void clear() {
        resources_.clear();
    }

    std::size_t size() const {
        return resources_.size();
    }

private:
    std::unordered_map<std::string, Resource> resources_;
};

} // namespace gamengine
