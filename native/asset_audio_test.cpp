
#include "asset_database.hpp"
#include "asset_registry.hpp"
#include "audio_system.hpp"

#include <cassert>
#include <memory>

using namespace gamengine;

class TestAudioBackend final : public AudioBackend {
public:
    bool initialize() override {
        initialized = true;
        return true;
    }

    void shutdown() override {
        initialized = false;
    }

    AudioVoiceId play(
        AssetHandle,
        float volume,
        float pitch,
        bool,
        bool,
        const AudioVector3&
    ) override {
        lastVolume = volume;
        lastPitch = pitch;
        return 1;
    }

    void stop(AudioVoiceId) override {
        stopped = true;
    }

    void pause(AudioVoiceId) override {}
    void resume(AudioVoiceId) override {}

    void setVoiceVolume(AudioVoiceId, float) override {}
    void setVoicePitch(AudioVoiceId, float) override {}

    void setListener(const AudioListener&) override {}

    void update() override {}

    bool initialized = false;
    bool stopped = false;

    float lastVolume = 0.0f;
    float lastPitch = 0.0f;
};

int main() {
    // Asset database
    AssetDatabase database;

    AssetMetadata metadata;
    metadata.name = "Test Sound";
    metadata.sourcePath = "assets/audio/test.wav";
    metadata.type = AssetType::Audio;

    AssetHandle sound = database.registerAsset(metadata);

    assert(sound.valid());
    assert(database.size() == 1);
    assert(database.find(sound) != nullptr);
    assert(database.findByPath("assets/audio/test.wav") == sound);

    // Runtime asset registry
    AssetRegistry<int> registry;

    auto resource = std::make_shared<int>(42);

    assert(registry.add(sound, resource));
    assert(registry.contains(sound));
    assert(*registry.get(sound) == 42);

    // Mixer
    AudioMixer mixer;

    mixer.setVolume(AudioGroup::Master, 0.8f);
    mixer.setVolume(AudioGroup::SFX, 0.5f);

    float gain = mixer.calculateGain(
        AudioGroup::SFX,
        1.0f,
        1.0f
    );

    assert(gain > 0.39f && gain < 0.41f);

    // Audio system
    auto backend = std::make_shared<TestAudioBackend>();

    AudioSystem audio(backend);

    assert(audio.initialize());

    audio.mixer().setVolume(AudioGroup::Master, 1.0f);

    AudioSource source;
    source.clip = sound;
    source.group = AudioGroup::SFX;
    source.setVolume(0.75f);
    source.setPitch(1.0f);

    assert(audio.play(100, source));
    assert(backend->lastVolume > 0.0f);

    audio.stop(100);
    assert(backend->stopped);

    audio.shutdown();
    assert(!audio.isInitialized());

    return 0;
}
