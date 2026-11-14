
#include <iostream>
#include <string>

namespace GAMENGINE {

class Application {
public:
    Application()
        : running(false) {}

    void Initialize() {
        std::cout << "-----------------------------\n";
        std::cout << "       GAMENGINE CORE        \n";
        std::cout << "-----------------------------\n";

        std::cout << "Version: "
                  << GAMENGINE_VERSION << '\n';

        std::cout << "Platform: Native\n";
        std::cout << "Language: C++20\n";

        running = true;

        std::cout << "Engine initialized.\n";
    }

    void Run() {
        if (!running) {
            return;
        }

        std::cout << "GAMENGINE native core is running.\n";
        std::cout << "Waiting for renderer integration...\n";
    }

    void Shutdown() {
        running = false;
        std::cout << "GAMENGINE shutting down.\n";
    }

private:
    bool running;
};

}

int main() {
    GAMENGINE::Application application;

    application.Initialize();
    application.Run();
    application.Shutdown();

    return 0;
}
