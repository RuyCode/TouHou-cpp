// mobprefabinspector.h
#ifndef MOB_PREFAB_INSPECTOR_H
#define MOB_PREFAB_INSPECTOR_H

#include "game.pb.h"

#include <QWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QUndoStack>
#include <QComboBox>

class MobPrefabInspector : public QWidget
{
    Q_OBJECT

public:
    explicit MobPrefabInspector(QUndoStack *undoStack, QWidget *parent = nullptr);
    void setMobPrefab(game::MobPrefab *mobPrefab);
    void clear();

private:
    void setupUI();
    void updateFields();
    void updateDropFields();
    void updateColliderFields();
    void clearFields();

private slots:
    void onNameChanged(const QString &text);
    void onHealthChanged(double value);
    void onSpriteChanged(const QString &text);
    void onAttackIdChanged(int value);
    void onDropItemChanged(const QString &text);
    void onDropWeightChanged(double value);
    void onFixedAmountChanged(int state);
    void onFixedAmountValueChanged(int value);
    void onMinAmountChanged(int value);
    void onMaxAmountChanged(int value);
    void onColliderOffsetXChanged(double value);
    void onColliderOffsetYChanged(double value);
    void onColliderTypeChanged(int index);
    void onCircleRadiusChanged(double value);
    void onRectWidthChanged(double value);
    void onRectHeightChanged(double value);
    void onRectAngleChanged(double value);

private:
    // UI Elements
    QGroupBox *m_basicGroup;
    QLineEdit *m_nameEdit;
    QDoubleSpinBox *m_healthSpin;
    QLineEdit *m_spriteEdit;
    QSpinBox *m_attackIdSpin;

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

    game::MobPrefab *m_currentMobPrefab;
    QUndoStack *m_undoStack;
};

#endif // MOB_PREFAB_INSPECTOR_H
