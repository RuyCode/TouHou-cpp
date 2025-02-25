#include "mainwindow.h"

#include "scenewidget.h"

#include <QFileSystemModel>
#include <QFormLayout>
#include <QLineEdit>
#include <QStandardItem>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Qt Unity-like Interface");

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
    toolBar->addAction("Move");
    toolBar->addAction("Rotate");
    toolBar->addAction("Scale");
}

void MainWindow::createDockWidgets() {
    // hierachyDock - hierachy of objects
    hierarchyDock = new QDockWidget("Hierarchy", this);
    hierarchyDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QTreeView* hierarchyTree = new QTreeView(hierarchyDock);
    hierarchyDock->setWidget(hierarchyTree);
    addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

    // projectDock - project
    projectDock = new QDockWidget("Project", this);
    projectDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QListView* projectList = new QListView(projectDock);
    projectDock->setWidget(projectList);
    addDockWidget(Qt::BottomDockWidgetArea, projectDock);

    // inspectorDock - inspector
    inspectorDock = new QDockWidget("Inspector", this);
    inspectorDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QTextEdit* inspectorText = new QTextEdit(inspectorDock);
    inspectorDock->setWidget(inspectorText);
    addDockWidget(Qt::RightDockWidgetArea, inspectorDock);
}

void MainWindow::createSceneWindow() {
    // scene window (QOpenGLWidget)
    sceneWidget = new SceneWidget(this);
    sceneWidget->setMinimumSize(320, 60);
    setCentralWidget(sceneWidget);
}
