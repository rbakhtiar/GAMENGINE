
#pragma once

#include "terrain.hpp"

#include <algorithm>

namespace gamengine {

enum class BiomeType {
    Grassland,
    Forest,
    Desert,
    Mountain,
    Snowfield
};

struct BiomeDefinition {
    BiomeType type;
    float minimumHeight;
    float maximumHeight;
    float minimumMoisture;
    float maximumMoisture;
};

class BiomeSystem {
public:
    BiomeType classify(
        float height,
        float moisture,
        float slope
    ) const {
        if (height >= 35.0f)
            return BiomeType::Snowfield;

        if (slope >= 0.72f)
            return BiomeType::Mountain;

        if (moisture < 0.25f && height < 15.0f)
            return BiomeType::Desert;

        if (moisture > 0.62f)
            return BiomeType::Forest;

        return BiomeType::Grassland;
    }

    float moistureAt(
        float worldX,
        float worldZ,
        unsigned int seed = 12345
    ) const {
        unsigned int x =
            static_cast<unsigned int>(worldX * 17.0f);

        unsigned int z =
            static_cast<unsigned int>(worldZ * 17.0f);

        unsigned int h = seed;

        h ^= x * 0x9E3779B9u;
        h ^= z * 0x85EBCA6Bu;

        h ^= h >> 16;
        h *= 0x7FEB352Du;
        h ^= h >> 15;

        return static_cast<float>(h) /
               4294967295.0f;
    }

    static const char* name(BiomeType type) {
        switch (type) {
            case BiomeType::Grassland:
                return "Grassland";

            case BiomeType::Forest:
                return "Forest";

            case BiomeType::Desert:
                return "Desert";

            case BiomeType::Mountain:
                return "Mountain";

            case BiomeType::Snowfield:
                return "Snowfield";
        }

        return "Unknown";
    }
};

} // namespace gamengine
