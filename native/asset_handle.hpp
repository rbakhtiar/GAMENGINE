
#pragma once

#include <cstdint>
#include <functional>

namespace gamengine {

struct AssetHandle {
    std::uint64_t value = 0;

    constexpr bool valid() const {
        return value != 0;
    }

    constexpr explicit operator bool() const {
        return valid();
    }

    constexpr bool operator==(const AssetHandle& other) const {
        return value == other.value;
    }

    constexpr bool operator!=(const AssetHandle& other) const {
        return !(*this == other);
    }
};

struct AssetHandleHash {
    std::size_t operator()(const AssetHandle& handle) const noexcept {
        return std::hash<std::uint64_t>{}(handle.value);
    }
};

}
