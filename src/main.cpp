#include "editor/game.pb.h"
#include "levelgenerator.h"
#include "qt/mainwindow.h"

#include <QApplication>

#include <filesystem>
#include <fstream>

namespace {
const int kWidth = 800;
const int kHeight = 600;
}  // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;

    std::string filename = "level.bin";
    const std::filesystem::path path(filename);
    if (!std::filesystem::exists(path)) {
        GenerateTestLevel(path.string());
    }

    game::Level level;
    std::ifstream ifs(path.string());
    level.ParseFromIstream(&ifs);
    std::cout << "Level loaded: " << path.string() << std::endl;
    std::cout << level.DebugString() << std::endl;
    level.Clear();
    std::cout << "Level cleared: " << path.string() << std::endl;
    std::cout << level.DebugString() << std::endl;

    window.resize(kWidth, kHeight);
    window.SetTestData();
    window.show();

    return app.exec();
}
