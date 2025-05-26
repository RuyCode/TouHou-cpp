#include "scripts/managers/GameManager.h"

#include "scripts/utils/Logger.h"
#include "scripts/utils/Debug.h"

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--debug") {
            DEBUG_MODE = true;
        }
    }

    if (DEBUG_MODE) {
        Logger::Instance().Info("Debug mode is enabled");
    }

    GameManager gameManager = GameManager();
    gameManager.Run();
}