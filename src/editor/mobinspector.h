#ifndef NEW_MOBINSPECTOR_H
#define NEW_MOBINSPECTOR_H

#include <QCheckBox>
#include <QComboBox>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "game.pb.h"

class MobInspector : public QWidget
{
    Q_OBJECT

public:
    explicit MobInspector(QWidget *parent = nullptr);
    void setMob(game::Mob *mob);
    void clear();

private:
    void setupUI();
    void updateFields();
    void updatePathFields();
    void updateColliderFields();
    void clearFields();
    void addPathPointRow(int pointIndex);

private slots:
    void onNameChanged(const QString &text);
    void onMobPrefabIdChanged(int value);
    void onSpawnDelayChanged(double value);
    void onPathPointChanged(int pointIndex, double value, bool isX);
    void onTravelTimeChanged(double value);
    void onDropItemChanged(const QString &text);
    void onDropWeightChanged(double value);
    void onFixedAmountChanged(int state);
    void onFixedAmountValueChanged(int value);
    void onMinAmountChanged(int value);
    void onMaxAmountChanged(int value);
    void onAddPathPointClicked();
    void onDeletePathPointClicked(int pointIndex);

private:
    // UI Elements
    QGroupBox *m_basicGroup;
    QLineEdit *m_nameEdit;
    QSpinBox *m_mobPrefabIdSpin;
    QDoubleSpinBox *m_spawnDelaySpin;

    QGroupBox *m_basicPrefabGroup;
    QDoubleSpinBox *m_healthSpin;
    QLineEdit *m_spriteEdit;
    QSpinBox *m_attackIdSpin;

    QGroupBox *m_pathGroup;
    QDoubleSpinBox *m_travelTimeSpin;
    QPushButton *m_addPathPointButton;
    QList<QDoubleSpinBox *> m_pathPointSpins;

    QGroupBox *m_dropGroup;
    QLineEdit *m_dropItemEdit;
    QDoubleSpinBox *m_dropWeightSpin;
    QCheckBox *m_fixedAmountCheck;
    QSpinBox *m_fixedAmountSpin;
    QSpinBox *m_minAmountSpin;
    QSpinBox *m_maxAmountSpin;

    QGroupBox *m_colliderGroup;
    QDoubleSpinBox *m_colliderOffsetXSpin;
    QDoubleSpinBox *m_colliderOffsetYSpin;
    QComboBox *m_colliderTypeCombo;
    QDoubleSpinBox *m_circleRadiusSpin;
    QDoubleSpinBox *m_rectWidthSpin;
    QDoubleSpinBox *m_rectHeightSpin;
    QDoubleSpinBox *m_rectAngleSpin;

    game::Mob *m_currentMob;
};

#endif // NEW_MOBINSPECTOR_H
