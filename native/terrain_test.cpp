
#include "terrain.hpp"

#include <iostream>

int main() {
    gamengine::TerrainSettings settings;

    settings.chunkSize = 128.0f;
    settings.resolution = 65;
    settings.heightScale = 45.0f;
    settings.octaves = 5;
    settings.seed = 2026;

    gamengine::TerrainGenerator terrain(settings);

    auto mesh = terrain.generate(0, 0);

    std::cout << "GAMENGINE TERRAIN TEST\n";

    std::cout << "Vertices: "
              << mesh.vertices.size() << '\n';

    std::cout << "Triangles: "
              << mesh.indices.size() / 3 << '\n';

    std::cout << "Height at origin: "
              << terrain.heightAt(0, 0) << '\n';

    auto lowDetail = terrain.generate(0, 0, 4);

    std::cout << "LOD vertices: "
              << lowDetail.vertices.size() << '\n';

    return 0;
}
