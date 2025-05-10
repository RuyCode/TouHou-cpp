#include "hierarchywindow.h"
#include "mainwindow.h"
#include "datamanager.h"

#include <QHeaderView>

namespace {
const int kResizeSize = 20;
const int kColumnCount = 2;
} // namespace

HierarchyWindow::HierarchyWindow(MainWindow *mainWindow, QWidget *parent)
    : QTreeWidget(parent), m_mainWindow(mainWindow)
{
    setHeaderHidden(true);
    setColumnCount(kColumnCount);
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    header()->setSectionResizeMode(1, QHeaderView::Fixed);
    header()->resizeSection(1, kResizeSize);

    connect(this, &QTreeWidget::itemClicked, this, &HierarchyWindow::onItemClicked);
}

void HierarchyWindow::updateHierarchy(const game::Level &level)
{
    clear();
    addMobBatches(level);
    addMobPrefabs(level);
    addAttacks(level);
    addBullets(level);
    addBosses(level);
}

void HierarchyWindow::addMobBatches(const game::Level &level)
{
    QTreeWidgetItem *mobBatchesItem = new QTreeWidgetItem(this);
    mobBatchesItem->setText(0, "Mob Batches");
    mobBatchesItem->setData(0, Qt::UserRole,
                            QVariant::fromValue(ObjectRef{ ObjectType::None, -1 }));

    for (int i = 0; i < level.mob_batches_size(); ++i) {
        const auto &batch = level.mob_batches(i);
        QTreeWidgetItem *batchItem = new QTreeWidgetItem(mobBatchesItem);
        batchItem->setText(0, QString::fromStdString(batch.name()));
        batchItem->setData(0, Qt::UserRole,
                           QVariant::fromValue(ObjectRef{ ObjectType::MobBatch, i }));

        for (int j = 0; j < batch.mobs_size(); ++j) {
            const auto &mob = batch.mobs(j);
            QTreeWidgetItem *mobItem = new QTreeWidgetItem(batchItem);
            mobItem->setText(0, QString::fromStdString(mob.name()));
            mobItem->setData(0, Qt::UserRole,
                             QVariant::fromValue(ObjectRef{ ObjectType::Mob, j,
                                                            i })); // j - mob index, i - batch index
        }
    }
}

void HierarchyWindow::addMobPrefabs(const game::Level &level)
{
    QTreeWidgetItem *prefabsItem = new QTreeWidgetItem(this);
    prefabsItem->setText(0, "Mob Prefabs");
    prefabsItem->setData(0, Qt::UserRole, QVariant::fromValue(ObjectRef{ ObjectType::None, -1 }));

    for (int i = 0; i < level.mob_prefabs_size(); ++i) {
        const auto &prefab = level.mob_prefabs(i);
        QTreeWidgetItem *prefabItem = new QTreeWidgetItem(prefabsItem);
        prefabItem->setText(0, QString::fromStdString(prefab.name()));
        prefabItem->setData(0, Qt::UserRole,
                            QVariant::fromValue(ObjectRef{ ObjectType::MobPrefab, i }));
    }
}

void HierarchyWindow::addAttacks(const game::Level &level)
{
    QTreeWidgetItem *attacksItem = new QTreeWidgetItem(this);
    attacksItem->setText(0, "Attacks");
    attacksItem->setData(0, Qt::UserRole, QVariant::fromValue(ObjectRef{ ObjectType::None, -1 }));

    for (int i = 0; i < level.attacks_size(); ++i) {
        const auto &attack = level.attacks(i);
        QTreeWidgetItem *attackItem = new QTreeWidgetItem(attacksItem);
        attackItem->setText(0, QString::fromStdString(attack.name()));
        attackItem->setData(0, Qt::UserRole,
                            QVariant::fromValue(ObjectRef{ ObjectType::Attack, i }));
    }
}

void HierarchyWindow::addBullets(const game::Level &level)
{
    QTreeWidgetItem *bulletsItem = new QTreeWidgetItem(this);
    bulletsItem->setText(0, "Bullets");
    bulletsItem->setData(0, Qt::UserRole, QVariant::fromValue(ObjectRef{ ObjectType::None, -1 }));

    for (int i = 0; i < level.bullets_size(); ++i) {
        QTreeWidgetItem *bulletItem = new QTreeWidgetItem(bulletsItem);
        bulletItem->setText(0, QString("Bullet %1").arg(i));
        bulletItem->setData(0, Qt::UserRole,
                            QVariant::fromValue(ObjectRef{ ObjectType::Bullet, i }));
    }
}

void HierarchyWindow::addBosses(const game::Level &level)
{
    QTreeWidgetItem *bossesItem = new QTreeWidgetItem(this);
    bossesItem->setText(0, "Bosses");
    bossesItem->setData(0, Qt::UserRole, QVariant::fromValue(ObjectRef{ ObjectType::None, -1 }));

    for (int i = 0; i < level.bosses_size(); ++i) {
        const auto &boss = level.bosses(i);
        QTreeWidgetItem *bossItem = new QTreeWidgetItem(bossesItem);
        bossItem->setText(0, QString::fromStdString(boss.name()));
        bossItem->setData(0, Qt::UserRole, QVariant::fromValue(ObjectRef{ ObjectType::Boss, i }));
    }
}

void HierarchyWindow::onItemClicked(QTreeWidgetItem *item)
{
    if (!item || !m_mainWindow) {
        return;
    }

    auto ref = item->data(0, Qt::UserRole).value<ObjectRef>();
    if (ref.type == ObjectType::None) {
        return;
    }

    game::Level &level = DataManager::getLevel();

    switch (ref.type) {
    case ObjectType::MobBatch:
        if (ref.index >= 0 && ref.index < level.mob_batches_size()) {
            m_mainWindow->showMobBatchInspector(level.mutable_mob_batches(ref.index));
        }
        break;

    case ObjectType::Mob:
        if (ref.parentIndex >= 0 && ref.parentIndex < level.mob_batches_size() && ref.index >= 0
            && ref.index < level.mob_batches(ref.parentIndex).mobs_size()) {
            auto *batch = level.mutable_mob_batches(ref.parentIndex);
            m_mainWindow->showMobInspector(batch->mutable_mobs(ref.index));
        }
        break;

    case ObjectType::MobPrefab:
        if (ref.index >= 0 && ref.index < level.mob_prefabs_size()) {
            m_mainWindow->showMobPrefabInspector(level.mutable_mob_prefabs(ref.index));
        }
        break;

    case ObjectType::Attack:
        if (ref.index >= 0 && ref.index < level.attacks_size()) {
            // m_mainWindow->showAttackInspector(level.mutable_attacks(ref.index));
        }
        break;

    case ObjectType::Bullet:
        if (ref.index >= 0 && ref.index < level.bullets_size()) {
            // m_mainWindow->showBulletInspector(level.mutable_bullets(ref.index));
        }
        break;

    case ObjectType::Boss:
        if (ref.index >= 0 && ref.index < level.bosses_size()) {
            // m_mainWindow->showBossInspector(level.mutable_bosses(ref.index));
        }
        break;

    default:
        break;
    }
}
