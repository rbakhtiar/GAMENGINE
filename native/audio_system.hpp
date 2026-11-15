
#pragma once

#include "audio_source.hpp"
#include "audio_listener.hpp"
#include "audio_mixer.hpp"

#include <memory>
#include <unordered_map>
#include <string>

namespace gamengine {

class AudioBackend {
public:
    virtual ~AudioBackend() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

    virtual AudioVoiceId play(
        AssetHandle clip,
        float volume,
        float pitch,
        bool looping,
        bool spatial,
        const AudioVector3& position
    ) = 0;

    virtual void stop(AudioVoiceId voice) = 0;
    virtual void pause(AudioVoiceId voice) = 0;
    virtual void resume(AudioVoiceId voice) = 0;

    virtual void setVoiceVolume(
        AudioVoiceId voice,
        float volume
    ) = 0;

    virtual void setVoicePitch(
        AudioVoiceId voice,
        float pitch
    ) = 0;

    virtual void setListener(
        const AudioListener& listener
    ) = 0;

    virtual void update() = 0;
};

class AudioSystem {
public:
    explicit AudioSystem(std::shared_ptr<AudioBackend> backend)
        : backend_(std::move(backend)) {}

    bool initialize() {
        if (!backend_) {
            return false;
        }

        initialized_ = backend_->initialize();
        return initialized_;
    }

    void shutdown() {
        if (backend_ && initialized_) {
            for (const auto& entry : voices_) {
                backend_->stop(entry.second);
            }

            voices_.clear();
            backend_->shutdown();
        }

        initialized_ = false;
    }

    bool play(AudioEntityId entity, AudioSource& source) {
        if (!initialized_ || !source.clip) {
            return false;
        }

        stop(entity);

        const float gain = mixer_.calculateGain(
            source.group,
            source.volume,
            listener_.masterVolume
        );

        AudioVoiceId voice = backend_->play(
            source.clip,
            gain,
            source.pitch,
            source.looping,
            source.spatial,
            source.position
        );

        if (voice == 0) {
            return false;
        }

        voices_[entity] = voice;
        source.state = AudioState::Playing;

        return true;
    }

    void stop(AudioEntityId entity) {
        auto it = voices_.find(entity);

        if (it == voices_.end()) {
            return;
        }

        backend_->stop(it->second);
        voices_.erase(it);
    }

    void pause(AudioEntityId entity) {
        auto it = voices_.find(entity);

        if (it != voices_.end()) {
            backend_->pause(it->second);
        }
    }

    void resume(AudioEntityId entity) {
        auto it = voices_.find(entity);

        if (it != voices_.end()) {
            backend_->resume(it->second);
        }
    }

    void setListener(const AudioListener& listener) {
        listener_ = listener;

        if (initialized_) {
            backend_->setListener(listener_);
        }
    }

    AudioMixer& mixer() {
        return mixer_;
    }

    const AudioListener& listener() const {
        return listener_;
    }

    void update() {
        if (initialized_) {
            backend_->update();
        }
    }

    bool isInitialized() const {
        return initialized_;
    }

private:
    std::shared_ptr<AudioBackend> backend_;

    AudioMixer mixer_;
    AudioListener listener_;

    std::unordered_map<AudioEntityId, AudioVoiceId> voices_;

    bool initialized_ = false;
};

}
