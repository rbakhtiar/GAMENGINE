
#pragma once

#include "asset_types.hpp"

#include <filesystem>
#include <string>

namespace gamengine {

struct ImportResult {
    bool success = false;
    std::string message;
    std::filesystem::path outputPath;
};

class AssetImporter {
public:
    virtual ~AssetImporter() = default;

    virtual bool supports(
        const std::filesystem::path& file
    ) const = 0;

    virtual ImportResult importAsset(
        const std::filesystem::path& source,
        const std::filesystem::path& destination
    ) = 0;
};

}
