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

class MobInspector : public QWidget {
    Q_OBJECT

public:
    explicit MobInspector(QWidget* parent = nullptr);
    void SetMob(game::Mob* mob);
    void Clear();

private:
    void SetupUI();
    void UpdateFields();
    void UpdatePathFields();
    void UpdateColliderFields();
    void ClearFields();
    void AddPathPointRow(int pointIndex);

    // UI Elements
    QGroupBox* basicGroup;
    QLineEdit* nameEdit;
    QSpinBox* mobPrefabIdSpin;
    QDoubleSpinBox* spawnDelaySpin;

    QGroupBox* basicPrefabGroup;
    QDoubleSpinBox* healthSpin;
    QLineEdit* spriteEdit;
    QSpinBox* attackIdSpin;

    QGroupBox* pathGroup;
    QDoubleSpinBox* travelTimeSpin;
    QPushButton* addPathPointButton;
    QList<QDoubleSpinBox*> pathPointSpins;

    QGroupBox* dropGroup;
    QLineEdit* dropItemEdit;
    QDoubleSpinBox* dropWeightSpin;
    QCheckBox* fixedAmountCheck;
    QSpinBox* fixedAmountSpin;
    QSpinBox* minAmountSpin;
    QSpinBox* maxAmountSpin;

    QGroupBox* colliderGroup;
    QDoubleSpinBox* colliderOffsetXSpin;
    QDoubleSpinBox* colliderOffsetYSpin;
    QComboBox* colliderTypeCombo;
    QDoubleSpinBox* circleRadiusSpin;
    QDoubleSpinBox* rectWidthSpin;
    QDoubleSpinBox* rectHeightSpin;
    QDoubleSpinBox* rectAngleSpin;

    game::Mob* currentMob;

private slots:
    void OnNameChanged(const QString& text);
    void OnMobPrefabIdChanged(int value);
    void OnSpawnDelayChanged(double value);
    void OnPathPointChanged(int pointIndex, double value, bool isX);
    void OnTravelTimeChanged(double value);
    void OnDropItemChanged(const QString& text);
    void OnDropWeightChanged(double value);
    void OnFixedAmountChanged(int state);
    void OnFixedAmountValueChanged(int value);
    void OnMinAmountChanged(int value);
    void OnMaxAmountChanged(int value);
    void OnAddPathPointClicked();
    void OnDeletePathPointClicked(int pointIndex);
};

#endif  // NEW_MOBINSPECTOR_H
