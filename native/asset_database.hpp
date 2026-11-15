
#pragma once

#include "asset_types.hpp"
#include "asset_handle.hpp"

#include <unordered_map>
#include <filesystem>
#include <optional>
#include <string>
#include <cstdint>

namespace gamengine {

class AssetDatabase {
public:
    AssetHandle registerAsset(const AssetMetadata& metadata) {
        const std::string key =
            metadata.sourcePath.lexically_normal().string();

        auto existing = pathToHandle_.find(key);
        if (existing != pathToHandle_.end()) {
            assets_[existing->second.value] = metadata;
            return existing->second;
        }

        AssetHandle handle{nextHandle_++};

        assets_[handle.value] = metadata;
        pathToHandle_[key] = handle;

        return handle;
    }

    const AssetMetadata* find(AssetHandle handle) const {
        auto it = assets_.find(handle.value);

        if (it == assets_.end()) {
            return nullptr;
        }

        return &it->second;
    }

    AssetHandle findByPath(
        const std::filesystem::path& path
    ) const {
        const std::string key = path.lexically_normal().string();

        auto it = pathToHandle_.find(key);

        return it == pathToHandle_.end()
            ? AssetHandle{}
            : it->second;
    }

    bool remove(AssetHandle handle) {
        auto it = assets_.find(handle.value);

        if (it == assets_.end()) {
            return false;
        }

        pathToHandle_.erase(
            it->second.sourcePath.lexically_normal().string()
        );

        assets_.erase(it);
        return true;
    }

    std::size_t size() const {
        return assets_.size();
    }

    void clear() {
        assets_.clear();
        pathToHandle_.clear();
    }

private:
    std::uint64_t nextHandle_ = 1;

    std::unordered_map<std::uint64_t, AssetMetadata> assets_;

    std::unordered_map<std::string, AssetHandle> pathToHandle_;
};

}
