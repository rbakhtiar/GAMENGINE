
#pragma once

#include <cstdint>
#include <string>
#include <filesystem>

namespace gamengine {

enum class AssetType {
    Unknown,
    Texture,
    Model,
    Material,
    Audio,
    Font,
    Shader,
    Animation,
    Scene,
    Script
};

struct AssetMetadata {
    std::string name;
    std::filesystem::path sourcePath;
    std::filesystem::path cachePath;
    AssetType type = AssetType::Unknown;
    std::uint64_t fileSize = 0;
    bool imported = false;
};

inline const char* assetTypeName(AssetType type) {
    switch (type) {
        case AssetType::Texture: return "Texture";
        case AssetType::Model: return "Model";
        case AssetType::Material: return "Material";
        case AssetType::Audio: return "Audio";
        case AssetType::Font: return "Font";
        case AssetType::Shader: return "Shader";
        case AssetType::Animation: return "Animation";
        case AssetType::Scene: return "Scene";
        case AssetType::Script: return "Script";
        default: return "Unknown";
    }
}

}
