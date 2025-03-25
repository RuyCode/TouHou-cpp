#include "mainwindow.h"

#include "datamanager.h"
#include "scenewidget.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFormLayout>
#include <QLineEdit>
#include <QStandardItem>
#include <QToolBar>

namespace {
const int kMinWidth = 320;
const int kMinHeight = 60;
}  // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), mobInspector(new MobInspector(this)), sceneWidget(new SceneWidget(this)) {
    setWindowTitle("Qt Unity-like Interface");

    hierarchyDock = new QDockWidget("Hierarchy", this);
    projectDock = new QDockWidget("Project", this);
    CreateDockWidgets();

    CreateSceneWindow();
}

void MainWindow::SetTestData() {
    // hierarchyDock - hierachy of objects
    QTreeView* hierarchyTree = dynamic_cast<QTreeView*>(hierarchyDock->widget());
    QStandardItemModel* model = new QStandardItemModel(hierarchyTree);
    model->setHorizontalHeaderLabels({"Objects"});

    QStandardItem* rootItem = model->invisibleRootItem();
    QStandardItem* object1 = new QStandardItem("Object 1");
    QStandardItem* object2 = new QStandardItem("Object 2");
    rootItem->appendRow(object1);
    rootItem->appendRow(object2);

    hierarchyTree->setModel(model);

    // mobInspector - inspector

    // projectDock - project
    QListView* projectList = dynamic_cast<QListView*>(projectDock->widget());
    QFileSystemModel* projectModel = new QFileSystemModel(projectList);
    projectModel->setRootPath(QDir::currentPath());
    projectList->setModel(projectModel);
    projectList->setRootIndex(projectModel->index(QDir::currentPath()));

    // toolBar - toolbar
    QToolBar* toolBar = addToolBar("Tools");

    QAction* SaveFile = toolBar->addAction("Save File");
    SaveFile->setShortcut(QKeySequence::Save);
    connect(SaveFile, &QAction::triggered, this, &MainWindow::SaveFile);

    QAction* LoadFile = toolBar->addAction("Load File");
    LoadFile->setShortcut(QKeySequence::Open);
    connect(LoadFile, &QAction::triggered, this, &MainWindow::LoadFile);

    QAction* UndoAction = toolBar->addAction("Undo");
    UndoAction->setShortcut(QKeySequence::Undo);
    connect(UndoAction, &QAction::triggered, this, &MainWindow::UndoSlot);

    QAction* RedoAction = toolBar->addAction("Redo");
    RedoAction->setShortcut(QKeySequence::Redo);
    connect(RedoAction, &QAction::triggered, this, &MainWindow::RedoSlot);

    toolBar->setFloatable(false);
    toolBar->setMovable(false);
}

void MainWindow::SaveFile() {
    std::cout << "save fileName: " << DataManager::file_path;
    std::cout << DataManager::GetInstance().level.DebugString();
    DataManager::SaveLevel();
}

void MainWindow::LoadFile() {
    QString executableDir = QCoreApplication::applicationDirPath();

    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", executableDir, "*.bin");

    if (fileName.isEmpty()) {
        qDebug() << "wrong file name: " << fileName;
        return;
    }

    DataManager::LoadLevel(fileName.toStdString());
    std::cout << "load fileName: " << fileName.toStdString();
    std::cout << DataManager::GetInstance().level.DebugString();

    mobInspector->Clear();
    game::Level& level = DataManager::level;
    auto mob = level.mutable_mobs(0);
    mobInspector->SetMob(mob);
}

void MainWindow::UndoSlot() {
    qDebug() << "Undo";
}

void MainWindow::RedoSlot() {
    qDebug() << "Redo";
}

void MainWindow::CreateDockWidgets() {
    // hierachyDock - hierachy of objects
    hierarchyDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QTreeView* hierarchyTree = new QTreeView(hierarchyDock);
    hierarchyDock->setWidget(hierarchyTree);
    addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

    // projectDock - project
    projectDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QListView* projectList = new QListView(projectDock);
    projectDock->setWidget(projectList);
    addDockWidget(Qt::BottomDockWidgetArea, projectDock);

    // mobInspector - inspector
    mobInspector->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, mobInspector);
}

void MainWindow::CreateSceneWindow() {
    // scene window (QOpenGLWidget)
    sceneWidget->setMinimumSize(kMinWidth, kMinHeight);
    setCentralWidget(sceneWidget);
}
