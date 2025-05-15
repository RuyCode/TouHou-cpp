#ifndef ATTACKINSPECTOR_H
#define ATTACKINSPECTOR_H

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include <QStackedWidget>
#include <QUndoStack>

#include "game.pb.h"

class AttackInspector : public QWidget
{
    Q_OBJECT

public:
    explicit AttackInspector(QUndoStack *undoStack, QWidget *parent = nullptr);
    void setAttack(game::Attack *attack);
    void clear();

private:
    void setupUI();
    void updateFields();
    void clearFields();
    void updateAttackTypeFields();
    void updateAttackPatternFields();

private slots:
    void onNameChanged(const QString &text);
    void onAttackTypeChanged(int index);
    void onBulletIdChanged(int value);
    void onAttackIdChanged(int value);
    void onSpeedChanged(double value);
    void onSpawnOffsetXChanged(double value);
    void onSpawnOffsetYChanged(double value);
    void onAmountChanged(double value);
    void onAttackPatternChanged(int index);
    void onCircleRotationAngleChanged(double value);
    void onArcLockOnPlayerChanged(int state);
    void onArcDirectionAngleChanged(double value);
    void onArcSpreadAngleChanged(double value);
    void onLineLockOnPlayerChanged(int state);
    void onLineDirectionAngleChanged(double value);
    void onLineDeltaTimeChanged(double value);
    void onLineDeltaSpeedChanged(double value);
    void onLineRandomSpeedMinChanged(double value);
    void onLineRandomSpeedMaxChanged(double value);
    void onLineDeltaAmountChanged(double value);
    void onLineRandomAmountMinChanged(double value);
    void onLineRandomAmountMaxChanged(double value);
    void onLineDeltaDirectionChanged(double value);
    void onLineRandomDirectionMinChanged(double value);
    void onLineRandomDirectionMaxChanged(double value);
    void onSpiralLockOnPlayerChanged(int state);
    void onSpiralBeginAngleChanged(double value);
    void onSpiralRotationAngleChanged(double value);
    void onSpiralDeltaTimeChanged(double value);
    void onSpiralDeltaSpeedChanged(double value);
    void onSpiralRandomSpeedMinChanged(double value);
    void onSpiralRandomSpeedMaxChanged(double value);
    void onSpiralDeltaAmountChanged(double value);
    void onSpiralRandomAmountMinChanged(double value);
    void onSpiralRandomAmountMaxChanged(double value);
    void onSpiralDeltaDirectionChanged(double value);
    void onSpiralRandomDirectionMinChanged(double value);
    void onSpiralRandomDirectionMaxChanged(double value);

private:
    // UI Elements
    QGroupBox *m_basicGroup;
    QLineEdit *m_nameEdit;
    QComboBox *m_attackTypeCombo;
    QSpinBox *m_bulletIdSpin;
    QSpinBox *m_attackIdSpin;
    QDoubleSpinBox *m_speedSpin;
    QDoubleSpinBox *m_spawnOffsetXSpin;
    QDoubleSpinBox *m_spawnOffsetYSpin;
    QDoubleSpinBox *m_amountSpin;
    QStackedWidget *m_attackTypeStack;
    QComboBox *m_attackPatternCombo;

    // Circle pattern
    QStackedWidget *m_patternStack;
    QGroupBox *m_circleGroup;
    QDoubleSpinBox *m_circleRotationAngleSpin;

    // Arc pattern
    QGroupBox *m_arcGroup;
    QCheckBox *m_arcLockOnPlayerCheck;
    QDoubleSpinBox *m_arcDirectionAngleSpin;
    QDoubleSpinBox *m_arcSpreadAngleSpin;

    // Line pattern
    QGroupBox *m_lineGroup;
    QCheckBox *m_lineLockOnPlayerCheck;
    QDoubleSpinBox *m_lineDirectionAngleSpin;
    QDoubleSpinBox *m_lineDeltaTimeSpin;
    QComboBox *m_lineSpeedConfigCombo;
    QDoubleSpinBox *m_lineDeltaSpeedSpin;
    QDoubleSpinBox *m_lineRandomSpeedMinSpin;
    QDoubleSpinBox *m_lineRandomSpeedMaxSpin;
    QComboBox *m_lineAmountConfigCombo;
    QDoubleSpinBox *m_lineDeltaAmountSpin;
    QDoubleSpinBox *m_lineRandomAmountMinSpin;
    QDoubleSpinBox *m_lineRandomAmountMaxSpin;
    QComboBox *m_lineDirectionConfigCombo;
    QDoubleSpinBox *m_lineDeltaDirectionSpin;
    QDoubleSpinBox *m_lineRandomDirectionMinSpin;
    QDoubleSpinBox *m_lineRandomDirectionMaxSpin;

    // Spiral pattern
    QGroupBox *m_spiralGroup;
    QCheckBox *m_spiralLockOnPlayerCheck;
    QDoubleSpinBox *m_spiralBeginAngleSpin;
    QDoubleSpinBox *m_spiralRotationAngleSpin;
    QDoubleSpinBox *m_spiralDeltaTimeSpin;
    QComboBox *m_spiralSpeedConfigCombo;
    QDoubleSpinBox *m_spiralDeltaSpeedSpin;
    QDoubleSpinBox *m_spiralRandomSpeedMinSpin;
    QDoubleSpinBox *m_spiralRandomSpeedMaxSpin;
    QComboBox *m_spiralAmountConfigCombo;
    QDoubleSpinBox *m_spiralDeltaAmountSpin;
    QDoubleSpinBox *m_spiralRandomAmountMinSpin;
    QDoubleSpinBox *m_spiralRandomAmountMaxSpin;
    QComboBox *m_spiralDirectionConfigCombo;
    QDoubleSpinBox *m_spiralDeltaDirectionSpin;
    QDoubleSpinBox *m_spiralRandomDirectionMinSpin;
    QDoubleSpinBox *m_spiralRandomDirectionMaxSpin;

    QStackedWidget *m_lineSpeedStack;
    QStackedWidget *m_lineAmountStack;
    QStackedWidget *m_lineDirectionStack;
    QStackedWidget *m_spiralSpeedStack;
    QStackedWidget *m_spiralAmountStack;
    QStackedWidget *m_spiralDirectionStack;

    game::Attack *m_currentAttack;
    QUndoStack *m_undoStack;
};

#endif // ATTACKINSPECTOR_H