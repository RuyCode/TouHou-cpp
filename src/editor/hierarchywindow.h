#ifndef HIERARCHYWINDOW_H
#define HIERARCHYWINDOW_H

#include "game.pb.h"

#include <QTreeWidget>

class MainWindow;

class HierarchyWindow : public QTreeWidget
{
    Q_OBJECT

public:
    enum class ObjectType { None, MobBatch, Mob, MobPrefab, Attack, Bullet, Boss };

    struct ObjectRef
    {
        ObjectType type = ObjectType::None;
        int index = -1;
        int parentIndex = -1; // For Mob - index of parent MobBatch class
    };

    explicit HierarchyWindow(MainWindow *mainWindow, QWidget *parent = nullptr);
    void updateHierarchy(const game::Level &level);

private slots:
    void onItemClicked(QTreeWidgetItem *item);

private:
    void addMobBatches(const game::Level &level);
    void addMobPrefabs(const game::Level &level);
    void addAttacks(const game::Level &level);
    void addBullets(const game::Level &level);
    void addBosses(const game::Level &level);

    MainWindow *m_mainWindow;
};

#endif // HIERARCHYWINDOW_H
