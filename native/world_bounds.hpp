
#pragma once

#include "terrain.hpp"

#include <algorithm>

namespace gamengine {

struct WorldBounds {
    Vec3 minimum;
    Vec3 maximum;

    bool contains(const Vec3& point) const {
        return
            point.x >= minimum.x &&
            point.x <= maximum.x &&
            point.y >= minimum.y &&
            point.y <= maximum.y &&
            point.z >= minimum.z &&
            point.z <= maximum.z;
    }

    bool intersects(const WorldBounds& other) const {
        return
            minimum.x <= other.maximum.x &&
            maximum.x >= other.minimum.x &&

            minimum.y <= other.maximum.y &&
            maximum.y >= other.minimum.y &&

            minimum.z <= other.maximum.z &&
            maximum.z >= other.minimum.z;
    }

    Vec3 center() const {
        return {
            (minimum.x + maximum.x) * 0.5f,
            (minimum.y + maximum.y) * 0.5f,
            (minimum.z + maximum.z) * 0.5f
        };
    }

    Vec3 size() const {
        return {
            maximum.x - minimum.x,
            maximum.y - minimum.y,
            maximum.z - minimum.z
        };
    }

    void expand(float amount) {
        minimum.x -= amount;
        minimum.y -= amount;
        minimum.z -= amount;

        maximum.x += amount;
        maximum.y += amount;
        maximum.z += amount;
    }

    static WorldBounds fromCenter(
        const Vec3& center,
        const Vec3& halfSize
    ) {
        return {
            {
                center.x - halfSize.x,
                center.y - halfSize.y,
                center.z - halfSize.z
            },
            {
                center.x + halfSize.x,
                center.y + halfSize.y,
                center.z + halfSize.z
            }
        };
    }
};

} // namespace gamengine
