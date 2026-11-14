
#include "gamengine/bridge.h"

#include <jni.h>
#include <cstdint>

static GE_Engine* get_engine(jlong handle) {
    return reinterpret_cast<GE_Engine*>(
        static_cast<intptr_t>(handle)
    );
}

extern "C" {

JNIEXPORT jlong JNICALL
Java_gamengine_GAMENGINE_nativeCreate(
    JNIEnv*, jobject
) {
    return static_cast<jlong>(
        reinterpret_cast<intptr_t>(
            ge_engine_create()
        )
    );
}

JNIEXPORT void JNICALL
Java_gamengine_GAMENGINE_nativeDestroy(
    JNIEnv*, jobject, jlong handle
) {
    ge_engine_destroy(get_engine(handle));
}

JNIEXPORT jlong JNICALL
Java_gamengine_GAMENGINE_nativeCreateObject(
    JNIEnv* env,
    jobject,
    jlong handle,
    jstring name
) {
    if (!name) return 0;

    const char* value = env->GetStringUTFChars(
        name, nullptr
    );

    if (!value) return 0;

    const uint64_t id = ge_object_create(
        get_engine(handle), value
    );

    env->ReleaseStringUTFChars(name, value);

    return static_cast<jlong>(id);
}

JNIEXPORT jlong JNICALL
Java_gamengine_GAMENGINE_nativeCount(
    JNIEnv*, jobject, jlong handle
) {
    return static_cast<jlong>(
        ge_object_count(get_engine(handle))
    );
}

JNIEXPORT jboolean JNICALL
Java_gamengine_GAMENGINE_nativeSetPosition(
    JNIEnv*,
    jobject,
    jlong handle,
    jlong id,
    jfloat x,
    jfloat y,
    jfloat z
) {
    return ge_object_set_position(
        get_engine(handle),
        static_cast<uint64_t>(id),
        x, y, z
    ) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_gamengine_GAMENGINE_nativeRemove(
    JNIEnv*, jobject, jlong handle, jlong id
) {
    return ge_object_remove(
        get_engine(handle),
        static_cast<uint64_t>(id)
    ) ? JNI_TRUE : JNI_FALSE;
}

}
