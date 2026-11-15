
#include "world_streamer.hpp"
#include <iostream>

int main() {
    gamengine::WorldStreamer::Settings settings;

    settings.chunkSize = 128.0f;
    settings.loadRadius = 3;
    settings.maxOperationsPerTick = 2;

    gamengine::WorldStreamer world(settings);

    world.setLoader([](const gamengine::ChunkCoord& c) {
        std::cout << "Loading chunk "
                  << c.x << ", " << c.z << '\n';

        // Replace with actual terrain/asset loading.
        return true;
    });

    world.setUnloader([](const gamengine::ChunkCoord& c) {
        std::cout << "Unloading chunk "
                  << c.x << ", " << c.z << '\n';

        // Replace with actual resource cleanup.
    });

    // Simulate player movement.
    for (int frame = 0; frame < 10; ++frame) {
        const float playerX = frame * 50.0f;
        const float playerZ = 0.0f;

        world.update(playerX, playerZ);

        std::cout << "Active chunks: "
                  << world.loadedCount() << '\n';
    }

    world.unloadAll();

    return 0;
}
