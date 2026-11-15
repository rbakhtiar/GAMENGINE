
#pragma once

#include <cstdint>

namespace gamengine {

using AudioVoiceId = std::uint64_t;
using AudioEntityId = std::uint64_t;

struct AudioVector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

enum class AudioState {
    Stopped,
    Playing,
    Paused
};

enum class AudioGroup {
    Master,
    Music,
    SFX,
    Dialogue,
    Ambient
};

struct AudioFormat {
    std::uint32_t sampleRate = 48000;
    std::uint16_t channels = 2;
    std::uint16_t bitsPerSample = 16;
};

}
