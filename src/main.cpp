#include "editor/game.pb.h"
#include "editor/mainwindow.h"
#include "levelgenerator.h"

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

    window.resize(kWidth, kHeight);
    window.SetTestData();
    window.show();

    return app.exec();
}
