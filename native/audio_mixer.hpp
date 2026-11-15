
#pragma once

#include "audio_types.hpp"

#include <array>
#include <algorithm>

namespace gamengine {

class AudioMixer {
public:
    AudioMixer() {
        volumes_.fill(1.0f);
    }

    void setVolume(AudioGroup group, float volume) {
        volumes_[index(group)] =
            std::clamp(volume, 0.0f, 1.0f);
    }

    float getVolume(AudioGroup group) const {
        return volumes_[index(group)];
    }

    float calculateGain(
        AudioGroup group,
        float sourceVolume,
        float listenerVolume
    ) const {
        return getVolume(AudioGroup::Master)
             * getVolume(group)
             * std::clamp(sourceVolume, 0.0f, 1.0f)
             * std::clamp(listenerVolume, 0.0f, 1.0f);
    }

private:
    static constexpr std::size_t index(AudioGroup group) {
        return static_cast<std::size_t>(group);
    }

    std::array<float, 5> volumes_{};
};

}
