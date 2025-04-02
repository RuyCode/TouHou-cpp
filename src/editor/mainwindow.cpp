#include "mainwindow.h"

#include "datamanager.h"
#include "scenewidget.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFormLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QStandardItem>
#include <QToolBar>

namespace {
const QString kInspectorTitle = "Inspector";
const QString kHierarchyTitle = "Hierarchy";
const QString kProjectTitle = "Project";

const int kWindowWidth = 800;
const int kWinowHeight = 600;

const int kOpenGLMinWidth = 320;
const int kOpenGLMinHeight = 60;

const int kMobInspectorIndex = 0;
}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      hierarchyDock(new QDockWidget(kHierarchyTitle, this)),
      inspectorDock(new QDockWidget(kInspectorTitle, this)),
      projectDock(new QDockWidget(kProjectTitle, this)),
      sceneWidget(new SceneWidget(this)),
      inspectorStacked(new QStackedWidget(inspectorDock)) {
    setWindowTitle("Qt Unity-like Interface");
    setMinimumSize(kWindowWidth, kWinowHeight);

    SetupUI();
}

void MainWindow::SetupUI() {
    // hierachyDock - hierachy of objects
    QTreeView* hierarchyTree = new QTreeView(hierarchyDock);
    hierarchyDock->setWidget(hierarchyTree);
    hierarchyDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

    // projectDock - project
    QListView* projectList = new QListView(projectDock);
    projectDock->setWidget(projectList);
    projectDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, projectDock);

    // inspectorDock - inspector
    inspectorStacked->addWidget(new MobInspector(inspectorStacked));
    QScrollArea* inspectorScroll = new QScrollArea(inspectorDock);
    inspectorScroll->setWidget(inspectorStacked);
    inspectorScroll->setWidgetResizable(true);
    inspectorDock->setWidget(inspectorScroll);
    inspectorDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, inspectorDock);

    // scene window (QOpenGLWidget)
    sceneWidget->setMinimumSize(kOpenGLMinWidth, kOpenGLMinHeight);
    setCentralWidget(sceneWidget);

    // toolBar - toolbar
    QToolBar* toolBar = addToolBar("Tools");

    QAction* SaveFile = toolBar->addAction("Save File");
    SaveFile->setShortcut(QKeySequence::Save);

    QAction* LoadFile = toolBar->addAction("Load File");
    LoadFile->setShortcut(QKeySequence::Open);

    QAction* UndoAction = toolBar->addAction("Undo");
    UndoAction->setShortcut(QKeySequence::Undo);

    QAction* RedoAction = toolBar->addAction("Redo");
    RedoAction->setShortcut(QKeySequence::Redo);

    toolBar->setFloatable(false);
    toolBar->setMovable(false);

    connect(SaveFile, &QAction::triggered, this, &MainWindow::SaveFile);
    connect(LoadFile, &QAction::triggered, this, &MainWindow::LoadFile);
    connect(UndoAction, &QAction::triggered, this, &MainWindow::UndoSlot);
    connect(RedoAction, &QAction::triggered, this, &MainWindow::RedoSlot);
}

void MainWindow::LoadFile() {
    QString executableDir = QCoreApplication::applicationDirPath();

    QString fileName = QFileDialog::getOpenFileName(this, "Choose a file", executableDir, "*.bin");

    if (fileName.isEmpty()) {
        return;
    }

    DataManager::LoadLevel(fileName.toStdString());

    inspectorStacked->setCurrentIndex(kMobInspectorIndex);
    auto mobInspector = qobject_cast<MobInspector*>(inspectorStacked->currentWidget());
    mobInspector->Clear();
    game::Level& level = DataManager::level;
    auto mob = level.mutable_mob_batches(0)->mutable_mobs(0);
    mobInspector->SetMob(mob);
}

void MainWindow::SaveFile() {
    DataManager::SaveLevel();
}

void MainWindow::UndoSlot() {
    qDebug() << "Undo";
}

void MainWindow::RedoSlot() {
    qDebug() << "Redo";
}
