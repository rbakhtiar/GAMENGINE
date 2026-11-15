
#pragma once

#include "asset_handle.hpp"

#include <unordered_map>
#include <memory>
#include <utility>
#include <cstddef>

namespace gamengine {

template <typename T>
class AssetRegistry {
public:
    bool add(AssetHandle handle, std::shared_ptr<T> asset) {
        if (!handle || !asset) {
            return false;
        }

        assets_[handle] = std::move(asset);
        return true;
    }

    std::shared_ptr<T> get(AssetHandle handle) const {
        auto it = assets_.find(handle);

        return it == assets_.end()
            ? nullptr
            : it->second;
    }

    bool contains(AssetHandle handle) const {
        return assets_.find(handle) != assets_.end();
    }

    bool remove(AssetHandle handle) {
        return assets_.erase(handle) > 0;
    }

    void clear() {
        assets_.clear();
    }

    std::size_t size() const {
        return assets_.size();
    }

private:
    std::unordered_map<
        AssetHandle,
        std::shared_ptr<T>,
        AssetHandleHash
    > assets_;
};

}
