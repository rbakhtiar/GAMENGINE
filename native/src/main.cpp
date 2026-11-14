
#include "gamengine/bridge.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    GE_Engine* engine = ge_engine_create();

    if (!engine) {
        std::cerr << "Engine initialization failed\n";
        return 1;
    }

    std::cout << "GAMENGINE CLI 0.2.0\n";
    std::cout << "Commands: create, list, move, delete, clear, quit\n";

    std::string line;

    while (std::cout << "gamengine> " &&
           std::getline(std::cin, line)) {

        std::istringstream input(line);
        std::string command;
        input >> command;

        if (command == "quit" || command == "exit") {
            break;
        }

        if (command == "create") {
            std::string name;
            std::getline(input >> std::ws, name);

            if (name.empty()) {
                std::cout << "Usage: create NAME\n";
                continue;
            }

            uint64_t id = ge_object_create(
                engine, name.c_str()
            );

            std::cout << "Created object ID: " << id << '\n';
        }

        else if (command == "list") {
            size_t count = ge_object_count(engine);

            std::cout << "Objects: " << count << '\n';

            for (size_t i = 0; i < count; ++i) {
                uint64_t id = ge_object_id_at(engine, i);
                char name[256];

                if (ge_object_get_name(
                    engine, id, name, sizeof(name)
                )) {
                    std::cout << id << " | " << name << '\n';
                }
            }
        }

        else if (command == "move") {
            uint64_t id;
            float x, y, z;

            if (!(input >> id >> x >> y >> z)) {
                std::cout << "Usage: move ID X Y Z\n";
                continue;
            }

            std::cout << (
                ge_object_set_position(
                    engine, id, x, y, z
                ) ? "Position updated\n" : "Object not found\n"
            );
        }

        else if (command == "delete") {
            uint64_t id;

            if (!(input >> id)) {
                std::cout << "Usage: delete ID\n";
                continue;
            }

            std::cout << (
                ge_object_remove(engine, id)
                ? "Object removed\n"
                : "Object not found\n"
            );
        }

        else if (command == "clear") {
            ge_scene_clear(engine);
            std::cout << "Scene cleared\n";
        }

        else if (!command.empty()) {
            std::cout << "Unknown command\n";
        }
    }

    ge_engine_destroy(engine);

    std::cout << "GAMENGINE CLI closed\n";
    return 0;
}
