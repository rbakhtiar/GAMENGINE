
#pragma once

#include "terrain.hpp"
#include "biome.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace gamengine {

enum class VegetationType {
    Tree,
    Grass,
    Rock
};

struct VegetationInstance {
    VegetationType type;
    Vec3 position;
    float scale;
    float rotation;
};

class VegetationGenerator {
public:
    struct Settings {
        int attempts = 500;
        float spacing = 3.0f;
        float maximumSlope = 0.55f;
        unsigned int seed = 12345;
    };

    VegetationGenerator(
        const TerrainGenerator& terrain,
        Settings settings = {}
    )
        : terrain_(terrain), settings_(settings) {
        settings_.attempts =
            std::max(0, settings_.attempts);

        settings_.spacing =
            std::max(0.1f, settings_.spacing);
    }

    std::vector<VegetationInstance> generate(
        float originX,
        float originZ,
        float width,
        float depth
    ) const {
        std::vector<VegetationInstance> output;

        if (width <= 0.0f || depth <= 0.0f)
            return output;

        BiomeSystem biomes;

        for (int i = 0; i < settings_.attempts; ++i) {
            const float x = originX +
                random(i * 2 + 1) * width;

            const float z = originZ +
                random(i * 2 + 2) * depth;

            const float height =
                terrain_.heightAt(x, z);

            const float delta = 1.0f;

            const float left =
                terrain_.heightAt(x - delta, z);

            const float right =
                terrain_.heightAt(x + delta, z);

            const float back =
                terrain_.heightAt(x, z - delta);

            const float front =
                terrain_.heightAt(x, z + delta);

            const float slope = std::clamp(
                std::sqrt(
                    (right - left) * (right - left) +
                    (front - back) * (front - back)
                ) / (2.0f * delta),
                0.0f,
                1.0f
            );

            if (slope > settings_.maximumSlope)
                continue;

            const float moisture =
                biomes.moistureAt(x, z, settings_.seed);

            const BiomeType biome =
                biomes.classify(height, moisture, slope);

            VegetationType type;

            switch (biome) {
                case BiomeType::Forest:
                    type = VegetationType::Tree;
                    break;

                case BiomeType::Grassland:
                    type = VegetationType::Grass;
                    break;

                case BiomeType::Desert:
                case BiomeType::Mountain:
                case BiomeType::Snowfield:
                    type = VegetationType::Rock;
                    break;

                default:
                    continue;
            }

            output.push_back({
                type,
                {x, height, z},
                0.7f + random(i * 3 + 3) * 1.3f,
                random(i * 3 + 4) * 6.2831853f
            });
        }

        return output;
    }

private:
    const TerrainGenerator& terrain_;
    Settings settings_;

    float random(int index) const {
        std::uint32_t h = settings_.seed;

        h ^= static_cast<std::uint32_t>(index) *
             0x9E3779B9u;

        h ^= h >> 16;
        h *= 0x7FEB352Du;
        h ^= h >> 15;
        h *= 0x846CA68Bu;
        h ^= h >> 16;

        return static_cast<float>(
            h / 4294967295.0
        );
    }
};

} // namespace gamengine
