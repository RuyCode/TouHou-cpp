#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "mobinspector.h"

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
    void SaveFile();
    void LoadFile();
    void UndoSlot();
    void RedoSlot();

private:
    void CreateDockWidgets();
    void CreateSceneWindow();

    QDockWidget* hierarchyDock;
    MobInspector* mobInspector;
    QDockWidget* projectDock;
    QOpenGLWidget* sceneWidget;
};

#endif  // GAMEWINDOW_H
