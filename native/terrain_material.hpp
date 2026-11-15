
#pragma once

#include <algorithm>
#include <array>
#include <cmath>

namespace gamengine {

enum class TerrainSurface {
    Grass,
    Rock,
    Sand,
    Snow
};

struct TerrainMaterialWeights {
    float grass = 0.0f;
    float rock = 0.0f;
    float sand = 0.0f;
    float snow = 0.0f;

    float total() const {
        return grass + rock + sand + snow;
    }
};

class TerrainMaterialSystem {
public:
    struct Settings {
        float snowHeight = 28.0f;
        float sandHeight = 3.0f;
        float steepSlope = 0.65f;
        float transition = 8.0f;
    };

    explicit TerrainMaterialSystem(Settings settings = {})
        : settings_(settings) {}

    TerrainMaterialWeights calculate(
        float height,
        float slope
    ) const {
        slope = std::clamp(slope, 0.0f, 1.0f);

        TerrainMaterialWeights weights;

        const float snow = smoothstep(
            settings_.snowHeight - settings_.transition,
            settings_.snowHeight + settings_.transition,
            height
        );

        const float sand = 1.0f - smoothstep(
            settings_.sandHeight - settings_.transition,
            settings_.sandHeight + settings_.transition,
            height
        );

        const float rock = smoothstep(
            settings_.steepSlope,
            1.0f,
            slope
        );

        const float remaining =
            (1.0f - rock) * (1.0f - snow);

        weights.snow = snow;
        weights.rock = rock;
        weights.sand = remaining * sand;
        weights.grass = remaining * (1.0f - sand);

        normalize(weights);

        return weights;
    }

private:
    Settings settings_;

    static float smoothstep(
        float edge0,
        float edge1,
        float value
    ) {
        if (edge0 == edge1)
            return value < edge0 ? 0.0f : 1.0f;

        float t = std::clamp(
            (value - edge0) / (edge1 - edge0),
            0.0f,
            1.0f
        );

        return t * t * (3.0f - 2.0f * t);
    }

    static void normalize(TerrainMaterialWeights& w) {
        float sum = w.total();

        if (sum <= 0.00001f) {
            w.grass = 1.0f;
            return;
        }

        w.grass /= sum;
        w.rock /= sum;
        w.sand /= sum;
        w.snow /= sum;
    }
};

} // namespace gamengine
