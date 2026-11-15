
#include "terrain.hpp"
#include "terrain_material.hpp"
#include "biome.hpp"
#include "vegetation.hpp"
#include "world_bounds.hpp"
#include "scene_graph.hpp"
#include "resource_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

int main() {
    using namespace gamengine;

    std::cout << "\n";
    std::cout << "================================\n";
    std::cout << " GAMENGINE WORLD SYSTEMS TEST\n";
    std::cout << "================================\n";

    // TERRAIN

    TerrainSettings settings;
    settings.resolution = 33;
    settings.seed = 2026;

    TerrainGenerator terrain(settings);

    TerrainMesh mesh = terrain.generate(0, 0);

    assert(!mesh.vertices.empty());
    assert(!mesh.indices.empty());

    std::cout << "[PASS] Terrain generation\n";

    // MATERIALS

    TerrainMaterialSystem materials;

    auto weights = materials.calculate(10.0f, 0.2f);

    assert(weights.total() > 0.99f);
    assert(weights.total() < 1.01f);

    std::cout << "[PASS] Terrain materials\n";

    // BIOMES

    BiomeSystem biomes;

    auto biome = biomes.classify(
        10.0f,
        0.8f,
        0.1f
    );

    assert(biome == BiomeType::Forest);

    std::cout << "[PASS] Biome classification: "
              << BiomeSystem::name(biome) << '\n';

    // VEGETATION

    VegetationGenerator::Settings vegetationSettings;
    vegetationSettings.attempts = 100;

    VegetationGenerator vegetation(
        terrain,
        vegetationSettings
    );

    auto instances = vegetation.generate(
        0.0f,
        0.0f,
        128.0f,
        128.0f
    );

    std::cout << "[PASS] Vegetation placement: "
              << instances.size()
              << " instances\n";

    // WORLD BOUNDS

    WorldBounds bounds = WorldBounds::fromCenter(
        {0, 0, 0},
        {10, 10, 10}
    );

    assert(bounds.contains({0, 0, 0}));
    assert(!bounds.contains({20, 0, 0}));

    std::cout << "[PASS] World bounds\n";

    // SCENE GRAPH

    SceneGraph scene;

    SceneNode& player =
        scene.root().createChild("Player");

    player.transform().position = {10, 0, 5};

    SceneNode& camera =
        player.createChild("Camera");

    camera.transform().position = {0, 2, 0};

    Vec3 cameraPosition = camera.worldPosition();

    assert(cameraPosition.x == 10);
    assert(cameraPosition.y == 2);
    assert(cameraPosition.z == 5);

    std::cout << "[PASS] Scene graph\n";

    // RESOURCE REGISTRY

    ResourceRegistry<std::string> resources;

    auto texture = std::make_shared<std::string>(
        "Forest_Albedo"
    );

    assert(resources.add("texture.forest", texture));
    assert(resources.contains("texture.forest"));

    auto loaded = resources.get("texture.forest");

    assert(loaded != nullptr);
    assert(*loaded == "Forest_Albedo");

    std::cout << "[PASS] Resource registry\n";

    std::cout << "================================\n";
    std::cout << " ALL WORLD SYSTEM TESTS PASSED\n";
    std::cout << "================================\n";

    return 0;
}
