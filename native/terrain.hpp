
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace gamengine {

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct TerrainVertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
};

struct TerrainMesh {
    std::vector<TerrainVertex> vertices;
    std::vector<std::uint32_t> indices;

    void clear() {
        vertices.clear();
        indices.clear();
    }
};

struct TerrainSettings {
    float chunkSize = 128.0f;
    float heightScale = 35.0f;

    int resolution = 65;
    int octaves = 5;
    int lodStep = 1;

    float frequency = 0.018f;
    float persistence = 0.5f;
    float lacunarity = 2.0f;

    std::uint32_t seed = 12345;
};

class TerrainGenerator {
public:
    explicit TerrainGenerator(TerrainSettings settings = {})
        : settings_(settings) {

        if (settings_.chunkSize <= 0.0f)
            settings_.chunkSize = 128.0f;

        if (settings_.resolution < 2)
            settings_.resolution = 2;

        settings_.octaves =
            std::max(1, settings_.octaves);

        settings_.lodStep =
            std::max(1, settings_.lodStep);

        settings_.frequency =
            std::max(0.0001f, settings_.frequency);

        settings_.persistence =
            std::clamp(settings_.persistence, 0.0f, 1.0f);

        settings_.lacunarity =
            std::max(1.0f, settings_.lacunarity);
    }

    float heightAt(float worldX, float worldZ) const {
        float amplitude = 1.0f;
        float frequency = settings_.frequency;

        float total = 0.0f;
        float weight = 0.0f;

        for (int i = 0; i < settings_.octaves; ++i) {
            total += noise(
                worldX * frequency,
                worldZ * frequency
            ) * amplitude;

            weight += amplitude;

            amplitude *= settings_.persistence;
            frequency *= settings_.lacunarity;
        }

        if (weight == 0.0f)
            return 0.0f;

        return (total / weight) * settings_.heightScale;
    }

    TerrainMesh generate(
        int chunkX,
        int chunkZ,
        int lodStep = -1
    ) const {

        TerrainMesh mesh;

        const int segments = settings_.resolution - 1;

        const int step = std::clamp(
            lodStep < 0 ? settings_.lodStep : lodStep,
            1,
            segments
        );

        const float spacing =
            settings_.chunkSize / segments;

        const float originX =
            chunkX * settings_.chunkSize;

        const float originZ =
            chunkZ * settings_.chunkSize;

        std::vector<int> coordinates;

        for (int i = 0; i < segments; i += step)
            coordinates.push_back(i);

        if (coordinates.empty() ||
            coordinates.back() != segments) {
            coordinates.push_back(segments);
        }

        const int side =
            static_cast<int>(coordinates.size());

        mesh.vertices.reserve(side * side);

        for (int z : coordinates) {
            for (int x : coordinates) {
                const float worldX =
                    originX + x * spacing;

                const float worldZ =
                    originZ + z * spacing;

                const float height =
                    heightAt(worldX, worldZ);

                const float normalOffset = spacing;

                const float left =
                    heightAt(worldX - normalOffset, worldZ);

                const float right =
                    heightAt(worldX + normalOffset, worldZ);

                const float back =
                    heightAt(worldX, worldZ - normalOffset);

                const float front =
                    heightAt(worldX, worldZ + normalOffset);

                Vec3 normal {
                    left - right,
                    2.0f * normalOffset,
                    back - front
                };

                normalize(normal);

                TerrainVertex vertex;

                vertex.position = {
                    x * spacing,
                    height,
                    z * spacing
                };

                vertex.normal = normal;

                vertex.uv = {
                    static_cast<float>(x) / segments,
                    static_cast<float>(z) / segments
                };

                mesh.vertices.push_back(vertex);
            }
        }

        for (int z = 0; z < side - 1; ++z) {
            for (int x = 0; x < side - 1; ++x) {
                const std::uint32_t a =
                    z * side + x;

                const std::uint32_t b = a + 1;

                const std::uint32_t c =
                    (z + 1) * side + x;

                const std::uint32_t d = c + 1;

                // Upward-facing triangle winding.

                mesh.indices.push_back(a);
                mesh.indices.push_back(c);
                mesh.indices.push_back(b);

                mesh.indices.push_back(b);
                mesh.indices.push_back(c);
                mesh.indices.push_back(d);
            }
        }

        return mesh;
    }

    const TerrainSettings& settings() const {
        return settings_;
    }

private:
    TerrainSettings settings_;

    static void normalize(Vec3& v) {
        const float length = std::sqrt(
            v.x * v.x +
            v.y * v.y +
            v.z * v.z
        );

        if (length <= 0.000001f) {
            v = {0.0f, 1.0f, 0.0f};
            return;
        }

        v.x /= length;
        v.y /= length;
        v.z /= length;
    }

    std::uint32_t hash(int x, int y) const {
        std::uint32_t h = settings_.seed;

        h ^= static_cast<std::uint32_t>(x) *
             0x9E3779B9u;

        h ^= static_cast<std::uint32_t>(y) *
             0x85EBCA6Bu;

        h ^= h >> 16;
        h *= 0x7FEB352Du;
        h ^= h >> 15;
        h *= 0x846CA68Bu;
        h ^= h >> 16;

        return h;
    }

    float randomValue(int x, int y) const {
        const std::uint32_t h = hash(x, y);

        return static_cast<float>(
            h / 4294967295.0
        ) * 2.0f - 1.0f;
    }

    static float smooth(float t) {
        return t * t * (3.0f - 2.0f * t);
    }

    static float lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    float noise(float x, float y) const {
        const int x0 = static_cast<int>(std::floor(x));
        const int y0 = static_cast<int>(std::floor(y));

        const int x1 = x0 + 1;
        const int y1 = y0 + 1;

        const float tx = smooth(x - x0);
        const float ty = smooth(y - y0);

        const float a = lerp(
            randomValue(x0, y0),
            randomValue(x1, y0),
            tx
        );

        const float b = lerp(
            randomValue(x0, y1),
            randomValue(x1, y1),
            tx
        );

        return lerp(a, b, ty);
    }
};

} // namespace gamengine
