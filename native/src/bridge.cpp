
#include "gamengine/bridge.h"

#include <algorithm>
#include <mutex>
#include <string>
#include <vector>

struct GE_Object {
    uint64_t id;
    std::string name;
    float x = 0;
    float y = 0;
    float z = 0;
};

struct GE_Engine {
    std::mutex mutex;
    std::vector<GE_Object> objects;
    uint64_t next_id = 1;
};

extern "C" {

GE_Engine* ge_engine_create(void) {
    try {
        return new GE_Engine();
    } catch (...) {
        return nullptr;
    }
}

void ge_engine_destroy(GE_Engine* engine) {
    delete engine;
}

uint64_t ge_object_create(
    GE_Engine* engine,
    const char* name
) {
    if (!engine || !name || name[0] == '\0')
        return 0;

    try {
        std::lock_guard<std::mutex> lock(engine->mutex);

        const uint64_t id = engine->next_id++;

        engine->objects.push_back({
            id, name, 0.0f, 0.0f, 0.0f
        });

        return id;
    } catch (...) {
        return 0;
    }
}

size_t ge_object_count(GE_Engine* engine) {
    if (!engine) return 0;

    std::lock_guard<std::mutex> lock(engine->mutex);
    return engine->objects.size();
}

uint64_t ge_object_id_at(
    GE_Engine* engine,
    size_t index
) {
    if (!engine) return 0;

    std::lock_guard<std::mutex> lock(engine->mutex);

    if (index >= engine->objects.size())
        return 0;

    return engine->objects[index].id;
}

int ge_object_get_name(
    GE_Engine* engine,
    uint64_t id,
    char* buffer,
    size_t capacity
) {
    if (!engine || !buffer || capacity == 0)
        return 0;

    std::lock_guard<std::mutex> lock(engine->mutex);

    for (const auto& object : engine->objects) {
        if (object.id == id) {
            const size_t length = std::min(
                object.name.size(),
                capacity - 1
            );

            std::copy_n(
                object.name.data(),
                length,
                buffer
            );

            buffer[length] = '\0';
            return 1;
        }
    }

    buffer[0] = '\0';
    return 0;
}

int ge_object_set_position(
    GE_Engine* engine,
    uint64_t id,
    float x,
    float y,
    float z
) {
    if (!engine) return 0;

    std::lock_guard<std::mutex> lock(engine->mutex);

    for (auto& object : engine->objects) {
        if (object.id == id) {
            object.x = x;
            object.y = y;
            object.z = z;
            return 1;
        }
    }

    return 0;
}

int ge_object_get_position(
    GE_Engine* engine,
    uint64_t id,
    float* x,
    float* y,
    float* z
) {
    if (!engine || !x || !y || !z)
        return 0;

    std::lock_guard<std::mutex> lock(engine->mutex);

    for (const auto& object : engine->objects) {
        if (object.id == id) {
            *x = object.x;
            *y = object.y;
            *z = object.z;
            return 1;
        }
    }

    return 0;
}

int ge_object_remove(
    GE_Engine* engine,
    uint64_t id
) {
    if (!engine) return 0;

    std::lock_guard<std::mutex> lock(engine->mutex);

    auto it = std::find_if(
        engine->objects.begin(),
        engine->objects.end(),
        [id](const GE_Object& object) {
            return object.id == id;
        }
    );

    if (it == engine->objects.end())
        return 0;

    engine->objects.erase(it);
    return 1;
}

void ge_scene_clear(GE_Engine* engine) {
    if (!engine) return;

    std::lock_guard<std::mutex> lock(engine->mutex);
    engine->objects.clear();
}

}
