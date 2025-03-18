#include <QApplication>
#include "qt/mainwindow.h"

namespace {
const int kWidth = 800;
const int kHeight = 600;
}  // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(kWidth, kHeight);
    window.SetTestData();
    window.show();
    return app.exec();
}
