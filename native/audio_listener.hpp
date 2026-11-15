
#pragma once

#include "audio_types.hpp"

namespace gamengine {

struct AudioListener {
    AudioVector3 position;
    AudioVector3 forward{0.0f, 0.0f, -1.0f};
    AudioVector3 up{0.0f, 1.0f, 0.0f};

    float masterVolume = 1.0f;

    void setMasterVolume(float value) {
        masterVolume = value < 0.0f ? 0.0f
                      : value > 1.0f ? 1.0f
                      : value;
    }
};

}
