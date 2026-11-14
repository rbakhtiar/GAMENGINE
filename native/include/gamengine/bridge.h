
#ifndef GAMENGINE_BRIDGE_H
#define GAMENGINE_BRIDGE_H

#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
    #if defined(GAMENGINE_BUILD)
        #define GE_API __declspec(dllexport)
    #else
        #define GE_API __declspec(dllimport)
    #endif
#else
    #define GE_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GE_Engine GE_Engine;

GE_API GE_Engine* ge_engine_create(void);
GE_API void ge_engine_destroy(GE_Engine* engine);

GE_API uint64_t ge_object_create(
    GE_Engine* engine,
    const char* name
);

GE_API size_t ge_object_count(
    GE_Engine* engine
);

GE_API uint64_t ge_object_id_at(
    GE_Engine* engine,
    size_t index
);

GE_API int ge_object_get_name(
    GE_Engine* engine,
    uint64_t id,
    char* buffer,
    size_t capacity
);

GE_API int ge_object_set_position(
    GE_Engine* engine,
    uint64_t id,
    float x,
    float y,
    float z
);

GE_API int ge_object_get_position(
    GE_Engine* engine,
    uint64_t id,
    float* x,
    float* y,
    float* z
);

GE_API int ge_object_remove(
    GE_Engine* engine,
    uint64_t id
);

GE_API void ge_scene_clear(GE_Engine* engine);

#ifdef __cplusplus
}
#endif

#endif
