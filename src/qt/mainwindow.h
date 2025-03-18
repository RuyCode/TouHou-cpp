#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDockWidget>
#include <QListView>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QTextEdit>
#include <QTreeView>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    void SetTestData();

private slots:
    void UndoSlot();
    void RedoSlot();

private:
    void createDockWidgets();
    void createSceneWindow();

    QDockWidget* hierarchyDock;
    QDockWidget* inspectorDock;
    QDockWidget* projectDock;
    QOpenGLWidget* sceneWidget;
};

#endif // GAMEWINDOW_H
