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
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_hierarchyDock(new QDockWidget(kHierarchyTitle, this)),
      m_inspectorDock(new QDockWidget(kInspectorTitle, this)),
      m_projectDock(new QDockWidget(kProjectTitle, this)),
      m_sceneWidget(new SceneWidget(this)),
      m_inspectorStacked(new QStackedWidget(m_inspectorDock))
{
    setWindowTitle("Qt Unity-like Interface");
    setMinimumSize(kWindowWidth, kWinowHeight);

    setupUI();
}

void MainWindow::setupUI()
{
    // hierachyDock - hierachy of objects
    QTreeView *hierarchyTree = new QTreeView(m_hierarchyDock);
    m_hierarchyDock->setWidget(hierarchyTree);
    m_hierarchyDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, m_hierarchyDock);

    // projectDock - project
    QListView *projectList = new QListView(m_projectDock);
    m_projectDock->setWidget(projectList);
    m_projectDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, m_projectDock);

    // inspectorDock - inspector
    m_inspectorStacked->addWidget(new MobInspector(m_inspectorStacked));
    QScrollArea *inspectorScroll = new QScrollArea(m_inspectorDock);
    inspectorScroll->setWidget(m_inspectorStacked);
    inspectorScroll->setWidgetResizable(true);
    m_inspectorDock->setWidget(inspectorScroll);
    m_inspectorDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, m_inspectorDock);

    // scene window (QOpenGLWidget)
    m_sceneWidget->setMinimumSize(kOpenGLMinWidth, kOpenGLMinHeight);
    setCentralWidget(m_sceneWidget);

    // toolBar - toolbar
    QToolBar *toolBar = addToolBar("Tools");

    QAction *SaveFile = toolBar->addAction("Save File");
    SaveFile->setShortcut(QKeySequence::Save);

    QAction *LoadFile = toolBar->addAction("Load File");
    LoadFile->setShortcut(QKeySequence::Open);

    QAction *UndoAction = toolBar->addAction("Undo");
    UndoAction->setShortcut(QKeySequence::Undo);

    QAction *RedoAction = toolBar->addAction("Redo");
    RedoAction->setShortcut(QKeySequence::Redo);

    toolBar->setFloatable(false);
    toolBar->setMovable(false);

    connect(SaveFile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(LoadFile, &QAction::triggered, this, &MainWindow::loadFile);
    connect(UndoAction, &QAction::triggered, this, &MainWindow::undoSlot);
    connect(RedoAction, &QAction::triggered, this, &MainWindow::redoSlot);
}

void MainWindow::loadFile()
{
    QString executableDir = QCoreApplication::applicationDirPath();

    QString fileName = QFileDialog::getOpenFileName(this, "Choose a file", executableDir, "*.bin");

    if (fileName.isEmpty()) {
        return;
    }

    DataManager::loadLevel(fileName.toStdString());

    m_inspectorStacked->setCurrentIndex(kMobInspectorIndex);
    auto mobInspector = qobject_cast<MobInspector *>(m_inspectorStacked->currentWidget());
    mobInspector->clear();
    game::Level &level = DataManager::getLevel();
    auto mob = level.mutable_mob_batches(0)->mutable_mobs(0);
    mobInspector->setMob(mob);
}

void MainWindow::saveFile()
{
    DataManager::saveLevel();
}

void MainWindow::undoSlot()
{
    qDebug() << "Undo";
}

void MainWindow::redoSlot()
{
    qDebug() << "Redo";
}
