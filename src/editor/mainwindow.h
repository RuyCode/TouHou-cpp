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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();

private slots:
    void loadFile();
    void saveFile();
    void undoSlot();
    void redoSlot();

private:
    QDockWidget *m_hierarchyDock;
    QDockWidget *m_inspectorDock;
    QDockWidget *m_projectDock;
    QOpenGLWidget *m_sceneWidget;

    QStackedWidget *m_inspectorStacked;
};

#endif // GAMEWINDOW_H
