#ifndef MOBINSPECTOR_H
#define MOBINSPECTOR_H

#include <QCheckBox>
#include <QComboBox>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include "game.pb.h"  // Assuming you've generated this from game.proto

class MobInspector : public QDockWidget {
    Q_OBJECT

public:
    MobInspector(QWidget* parent = nullptr);

    void SetMob(game::Mob* mob);

    void Clear();

private:
    game::Mob* currentMob = nullptr;

    // Form widgets
    QLineEdit* mobIdEdit;
    QDoubleSpinBox* healthSpin;
    QLineEdit* spriteEdit;
    QDoubleSpinBox* spawnDelaySpin;
    QLineEdit* attackIdEdit;

    // Path group
    QGroupBox* pathGroup;
    QList<QDoubleSpinBox*> pathPointSpins;
    QDoubleSpinBox* travelTimeSpin;

    // Drop group
    QGroupBox* dropGroup;
    QLineEdit* dropItemEdit;
    QDoubleSpinBox* dropWeightSpin;

    // Collider group
    QGroupBox* colliderGroup;
    QDoubleSpinBox* colliderOffsetXSpin;
    QDoubleSpinBox* colliderOffsetYSpin;
    QComboBox* colliderTypeCombo;
    QDoubleSpinBox* circleRadiusSpin;
    QDoubleSpinBox* rectWidthSpin;
    QDoubleSpinBox* rectHeightSpin;

    void setupUI();

    void UpdateFields();
    void UpdatePathFields();
    void UpdateColliderFields();

    void ClearFields();

private slots:
    void OnMobIdChanged(const QString& text);
    void OnHealthChanged(double value);
    void OnSpriteChanged(const QString& text);
    void OnSpawnDelayChanged(double value);
    void OnAttackIdChanged(const QString& text);
    void OnPathPointChanged(int pointIndex, double value, bool isX);
    void OnTravelTimeChanged(double value);
    void OnDropItemChanged(const QString& text);
    void OnDropWeightChanged(double value);
    void OnColliderOffsetXChanged(double value);
    void OnColliderOffsetYChanged(double value);
    void OnColliderTypeChanged(int index);
    void OnCircleRadiusChanged(double value);
    void OnRectWidthChanged(double value);
    void OnRectHeightChanged(double value);

    // ... similar slots for other fields
};

#endif
