#include "mainwindow.h"

#include "datamanager.h"
#include "scenewidget.h"
#include "mobinspector.h"
#include "mobbatchinspector.h"
#include "mobprefabinspector.h"
#include "attackinspector.h"
#include "bulletinspector.h"
#include "hierarchywindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFormLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QStandardItem>
#include <QToolBar>

#include <filesystem>

namespace {
const QString kInspectorTitle = "Inspector";
const QString kHierarchyTitle = "Hierarchy";
const QString kProjectTitle = "Project";

const int kWindowWidth = 800;
const int kWinowHeight = 600;

const int kOpenGLMinWidth = 320;
const int kOpenGLMinHeight = 60;

enum class InspectorType {
    MobInspector,
    MobBatchInspector,
    MobPrefabInspector,
    AttackInspector,
    BulletInspector,
};
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_hierarchyDock(new QDockWidget(kHierarchyTitle, this)),
      m_inspectorDock(new QDockWidget(kInspectorTitle, this)),
      m_projectDock(new QDockWidget(kProjectTitle, this)),
      m_sceneWidget(new SceneWidget(this)),
      m_inspectorStacked(new QStackedWidget(m_inspectorDock)),
      m_undoStack(new QUndoStack(this))
{
    setWindowTitle("Qt Unity-like Interface");
    setMinimumSize(kWindowWidth, kWinowHeight);

    setupUI();

    // TODO: remove at release version
    std::string filename = "level.bin";
    const std::filesystem::path path(filename);
    if (std::filesystem::exists(path)) {
        qDebug() << "loaded test level";
        DataManager::loadLevel(path);

        game::Level &level = DataManager::getLevel();

        HierarchyWindow *hierarchyTree = qobject_cast<HierarchyWindow *>(m_hierarchyDock->widget());
        hierarchyTree->updateHierarchy(level);

        m_inspectorStacked->setCurrentIndex(static_cast<int>(InspectorType::MobInspector));
        auto mobInspector = qobject_cast<MobInspector *>(m_inspectorStacked->currentWidget());
        mobInspector->clear();
        auto mob = level.mutable_mob_batches(0)->mutable_mobs(0);
        mobInspector->setMob(mob);
    }
}

void MainWindow::showMobInspector(game::Mob *mob)
{
    m_inspectorStacked->setCurrentIndex(static_cast<int>(InspectorType::MobInspector));
    auto mobInspector = qobject_cast<MobInspector *>(m_inspectorStacked->currentWidget());
    mobInspector->clear();
    mobInspector->setMob(mob);
}

void MainWindow::showMobBatchInspector(game::MobBatch *mobBatch)
{
    m_inspectorStacked->setCurrentIndex(static_cast<int>(InspectorType::MobBatchInspector));
    auto mobBatchInspector = qobject_cast<MobBatchInspector *>(m_inspectorStacked->currentWidget());
    mobBatchInspector->clear();
    mobBatchInspector->setMobBatch(mobBatch);
}

void MainWindow::showMobPrefabInspector(game::MobPrefab *mobPrefab)
{
    m_inspectorStacked->setCurrentIndex(static_cast<int>(InspectorType::MobPrefabInspector));
    auto mobPrefabInspector =
            qobject_cast<MobPrefabInspector *>(m_inspectorStacked->currentWidget());
    mobPrefabInspector->clear();
    mobPrefabInspector->setMobPrefab(mobPrefab);
}

void MainWindow::showAttackInspector(game::Attack *attack)
{
    m_inspectorStacked->setCurrentIndex(static_cast<int>(InspectorType::AttackInspector));
    auto attackInspector = qobject_cast<AttackInspector *>(m_inspectorStacked->currentWidget());
    attackInspector->clear();
    attackInspector->setAttack(attack);
}

void MainWindow::showBulletInspector(game::Bullet *bullet)
{
    m_inspectorStacked->setCurrentIndex(static_cast<int>(InspectorType::BulletInspector));
    auto bulletInspector = qobject_cast<BulletInspector *>(m_inspectorStacked->currentWidget());
    bulletInspector->clear();
    bulletInspector->setBullet(bullet);
}

void MainWindow::setupUI()
{
    // hierachyDock - hierachy of objects
    HierarchyWindow *hierarchyTree =
            new HierarchyWindow(this, qobject_cast<QWidget *>(m_hierarchyDock));
    m_hierarchyDock->setWidget(hierarchyTree);
    m_hierarchyDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, m_hierarchyDock);

    // projectDock - project
    QListView *projectList = new QListView(m_projectDock);
    m_projectDock->setWidget(projectList);
    m_projectDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, m_projectDock);

    // inspectorDock - inspector
    m_inspectorStacked->addWidget(new MobInspector(m_undoStack, m_inspectorStacked));
    m_inspectorStacked->addWidget(new MobBatchInspector(m_undoStack, m_inspectorStacked));
    m_inspectorStacked->addWidget(new MobPrefabInspector(m_undoStack, m_inspectorStacked));
    m_inspectorStacked->addWidget(new AttackInspector(m_undoStack, m_inspectorStacked));
    m_inspectorStacked->addWidget(new BulletInspector(m_undoStack, m_inspectorStacked));
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

    QAction *UpdateHierarchyAction = toolBar->addAction("Update Hierarchy");

    toolBar->setFloatable(false);
    toolBar->setMovable(false);

    connect(SaveFile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(LoadFile, &QAction::triggered, this, &MainWindow::loadFile);
    connect(UndoAction, &QAction::triggered, this, &MainWindow::undoSlot);
    connect(RedoAction, &QAction::triggered, this, &MainWindow::redoSlot);
    connect(UpdateHierarchyAction, &QAction::triggered, this, &MainWindow::UpdateHierarchySlot);
}

void MainWindow::loadFile()
{
    QString executableDir = QCoreApplication::applicationDirPath();

    QString fileName = QFileDialog::getOpenFileName(this, "Choose a file", executableDir, "*.bin");

    if (fileName.isEmpty()) {
        return;
    }

    DataManager::loadLevel(fileName.toStdString());

    game::Level &level = DataManager::getLevel();

    HierarchyWindow *hierarchyTree = qobject_cast<HierarchyWindow *>(m_hierarchyDock->widget());
    hierarchyTree->updateHierarchy(level);

    m_inspectorStacked->setCurrentIndex(static_cast<int>(InspectorType::MobInspector));
    auto mobInspector = qobject_cast<MobInspector *>(m_inspectorStacked->currentWidget());
    mobInspector->clear();
    auto mob = level.mutable_mob_batches(0)->mutable_mobs(0);
    mobInspector->setMob(mob);
}

void MainWindow::saveFile()
{
    DataManager::saveLevel();
}

void MainWindow::undoSlot()
{
    m_undoStack->undo();
}

void MainWindow::redoSlot()
{
    m_undoStack->redo();
}

void MainWindow::UpdateHierarchySlot()
{
    game::Level &level = DataManager::getLevel();
    HierarchyWindow *hierarchyTree = qobject_cast<HierarchyWindow *>(m_hierarchyDock->widget());
    hierarchyTree->updateHierarchy(level);
}
