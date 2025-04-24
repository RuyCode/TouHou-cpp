#include "editor/game.pb.h"
#include "editor/mainwindow.h"
#include "levelgenerator.h"
#include "editor/datamanager.h"

#include <QApplication>

#include <filesystem>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    std::string filename = "level.bin";
    const std::filesystem::path path(filename);
    if (!std::filesystem::exists(path)) {
        qGenerateTestLevel(path.string());
    }

    window.show();

    return app.exec();
}
