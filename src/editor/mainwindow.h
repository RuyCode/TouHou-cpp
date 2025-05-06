#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "game.pb.h"

#include <QDockWidget>
#include <QListView>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTreeView>
#include <QUndoStack>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void showMobInspector(game::Mob *mob);
    void showMobBatchInspector(game::MobBatch *mobBatch);
    void showMobPrefabInspector(game::MobPrefab *mobPrefab);

private:
    void setupUI();

private slots:
    void loadFile();
    void saveFile();
    void undoSlot();
    void redoSlot();
    void UpdateHierarchySlot();

private:
    QDockWidget *m_hierarchyDock;
    QDockWidget *m_inspectorDock;
    QDockWidget *m_projectDock;
    QOpenGLWidget *m_sceneWidget;

    QStackedWidget *m_inspectorStacked;

    QUndoStack *m_undoStack;
};

#endif // GAMEWINDOW_H
