#include "mainwindow.h"

#include "../editor/datamanager.h"
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

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), sceneWidget(new SceneWidget(this)) {
    setWindowTitle("Qt Unity-like Interface");

    hierarchyDock = new QDockWidget("Hierarchy", this);
    projectDock = new QDockWidget("Project", this);
    inspectorDock = new QDockWidget("Inspector", this);
    createDockWidgets();

    createSceneWindow();
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

    // inspectorDock - inspector
    QFormLayout* inspectorLayout = new QFormLayout();
    inspectorLayout->addRow("Name:", new QLineEdit("Object 1"));
    inspectorLayout->addRow("Position:", new QLineEdit("0, 0, 0"));
    inspectorLayout->addRow("Rotation:", new QLineEdit("0, 0, 0"));
    inspectorLayout->addRow("Scale:", new QLineEdit("1, 1, 1"));

    QWidget* inspectorWidget = new QWidget();
    inspectorWidget->setLayout(inspectorLayout);
    inspectorDock->setWidget(inspectorWidget);

    // projectDock - project
    QListView* projectList = dynamic_cast<QListView*>(projectDock->widget());
    QFileSystemModel* projectModel = new QFileSystemModel(projectList);
    projectModel->setRootPath(QDir::currentPath());
    projectList->setModel(projectModel);
    projectList->setRootIndex(projectModel->index(QDir::currentPath()));

    // toolBar - toolbar
    QToolBar* toolBar = addToolBar("Tools");

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

void MainWindow::LoadFile() {
    QString executableDir = QCoreApplication::applicationDirPath();

    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", executableDir, "*.bin");

    if (!fileName.isEmpty()) {
        DataManager::GetInstance().LoadLevel(fileName.toStdString());
        qDebug() << "fileName: " << fileName;
        qDebug() << QString::fromStdString(DataManager::GetInstance().level.DebugString());
    }
}

void MainWindow::UndoSlot() {
    qDebug() << "Undo";
}

void MainWindow::RedoSlot() {
    qDebug() << "Redo";
}

void MainWindow::createDockWidgets() {
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

    // inspectorDock - inspector
    inspectorDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QTextEdit* inspectorText = new QTextEdit(inspectorDock);
    inspectorDock->setWidget(inspectorText);
    addDockWidget(Qt::RightDockWidgetArea, inspectorDock);
}

void MainWindow::createSceneWindow() {
    // scene window (QOpenGLWidget)
    sceneWidget->setMinimumSize(kMinWidth, kMinHeight);
    setCentralWidget(sceneWidget);
}
