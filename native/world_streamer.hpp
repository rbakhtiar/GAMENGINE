
#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <vector>

namespace gamengine {

struct ChunkCoord {
    int x = 0;
    int z = 0;

    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }
};

struct ChunkHash {
    std::size_t operator()(const ChunkCoord& c) const {
        const auto x = static_cast<std::uint32_t>(c.x);
        const auto z = static_cast<std::uint32_t>(c.z);

        return (static_cast<std::uint64_t>(x) << 32) | z;
    }
};

class WorldStreamer {
public:
    using LoadCallback =
        std::function<bool(const ChunkCoord&)>;

    using UnloadCallback =
        std::function<void(const ChunkCoord&)>;

    struct Settings {
        float chunkSize = 128.0f;
        int loadRadius = 4;
        int maxOperationsPerTick = 2;
    };

    explicit WorldStreamer(Settings settings = {})
        : settings_(settings) {
        if (settings_.chunkSize <= 0.0f)
            settings_.chunkSize = 128.0f;

        if (settings_.loadRadius < 0)
            settings_.loadRadius = 0;

        if (settings_.maxOperationsPerTick < 1)
            settings_.maxOperationsPerTick = 1;
    }

    void setLoader(LoadCallback callback) {
        loader_ = std::move(callback);
    }

    void setUnloader(UnloadCallback callback) {
        unloader_ = std::move(callback);
    }

    void update(float worldX, float worldZ) {
        center_ = {
            static_cast<int>(std::floor(
                worldX / settings_.chunkSize)),
            static_cast<int>(std::floor(
                worldZ / settings_.chunkSize))
        };

        rebuildDesiredChunks();
        processUnloads();
        processLoads();
    }

    bool isLoaded(const ChunkCoord& coord) const {
        return loaded_.find(coord) != loaded_.end();
    }

    std::size_t loadedCount() const {
        return loaded_.size();
    }

    ChunkCoord centerChunk() const {
        return center_;
    }

    const std::unordered_set<ChunkCoord, ChunkHash>&
    loadedChunks() const {
        return loaded_;
    }

    void unloadAll() {
        if (unloader_) {
            for (const auto& chunk : loaded_)
                unloader_(chunk);
        }

        loaded_.clear();
        desired_.clear();
    }

private:
    Settings settings_;

    ChunkCoord center_;

    std::unordered_set<
        ChunkCoord,
        ChunkHash
    > loaded_;

    std::unordered_set<
        ChunkCoord,
        ChunkHash
    > desired_;

    struct Candidate {
        ChunkCoord coord;
        int distanceSquared;
    };

    static int distanceSquared(
        const ChunkCoord& a,
        const ChunkCoord& b
    ) {
        const int dx = a.x - b.x;
        const int dz = a.z - b.z;

        return dx * dx + dz * dz;
    }

    void rebuildDesiredChunks() {
        desired_.clear();

        const int radius = settings_.loadRadius;

        for (int z = -radius; z <= radius; ++z) {
            for (int x = -radius; x <= radius; ++x) {
                ChunkCoord coord {
                    center_.x + x,
                    center_.z + z
                };

                desired_.insert(coord);
            }
        }
    }

    void processUnloads() {
        if (!unloader_)
            return;

        int operations = 0;

        std::vector<ChunkCoord> obsolete;

        for (const auto& chunk : loaded_) {
            if (desired_.find(chunk) == desired_.end())
                obsolete.push_back(chunk);
        }

        std::sort(
            obsolete.begin(),
            obsolete.end(),
            [&](const ChunkCoord& a,
                const ChunkCoord& b) {
                return distanceSquared(a, center_) >
                       distanceSquared(b, center_);
            }
        );

        for (const auto& chunk : obsolete) {
            if (operations >=
                settings_.maxOperationsPerTick)
                break;

            unloader_(chunk);
            loaded_.erase(chunk);
            ++operations;
        }
    }

    void processLoads() {
        if (!loader_)
            return;

        std::vector<Candidate> candidates;

        for (const auto& chunk : desired_) {
            if (isLoaded(chunk))
                continue;

            candidates.push_back({
                chunk,
                distanceSquared(chunk, center_)
            });
        }

        std::sort(
            candidates.begin(),
            candidates.end(),
            [](const Candidate& a,
               const Candidate& b) {
                return a.distanceSquared <
                       b.distanceSquared;
            }
        );

        int operations = 0;

        for (const auto& candidate : candidates) {
            if (operations >=
                settings_.maxOperationsPerTick)
                break;

            if (loader_(candidate.coord))
                loaded_.insert(candidate.coord);

            ++operations;
        }
    }

    LoadCallback loader_;
    UnloadCallback unloader_;
};

} // namespace gamengine
