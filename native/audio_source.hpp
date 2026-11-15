
#pragma once

#include "asset_handle.hpp"
#include "audio_types.hpp"

namespace gamengine {

struct AudioSource {
    AssetHandle clip;

    AudioGroup group = AudioGroup::SFX;
    AudioState state = AudioState::Stopped;

    float volume = 1.0f;
    float pitch = 1.0f;

    bool looping = false;
    bool spatial = true;

    AudioVector3 position;

    void setVolume(float value) {
        volume = value < 0.0f ? 0.0f
               : value > 1.0f ? 1.0f
               : value;
    }

    void setPitch(float value) {
        pitch = value < 0.01f ? 0.01f : value;
    }
};

}
