#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "mobinspector.h"

#include <QDockWidget>
#include <QListView>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTreeView>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    void SetupUI();

    QDockWidget* hierarchyDock;
    QDockWidget* inspectorDock;
    QDockWidget* projectDock;
    QOpenGLWidget* sceneWidget;

    QStackedWidget* inspectorStacked;

private slots:
    void LoadFile();
    void SaveFile();
    void UndoSlot();
    void RedoSlot();
};

#endif  // GAMEWINDOW_H
