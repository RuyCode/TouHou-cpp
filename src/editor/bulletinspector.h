// BulletInspector.h
#ifndef BULLETINSPECTOR_H
#define BULLETINSPECTOR_H

#include <QWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QStackedWidget>
#include <QUndoStack>

#include "game.pb.h"

class BulletInspector : public QWidget
{
    Q_OBJECT
public:
    explicit BulletInspector(QUndoStack *undoStack, QWidget *parent = nullptr);
    void setBullet(game::Bullet *bullet);
    void clear();

private:
    void setupUI();
    void updateFields();
    void clearFields();
    void updateColliderTypeFields();
    void updateLifeConfigFields();

private slots:
    void onDamageChanged(double value);
    void onSpriteChanged(const QString &text);
    void onColliderTypeChanged(int index);
    void onOffsetXChanged(double value);
    void onOffsetYChanged(double value);
    void onCircleRadiusChanged(double value);
    void onRectangleAngleChanged(double value);
    void onRectangleWidthChanged(double value);
    void onRectangleHeightChanged(double value);
    void onLifeConfigToggled(int state);
    void onLifeConfigLifespanChanged(double value);
    void onLifeConfigAttackIdChanged(int value);

private:
    // Basic
    QGroupBox *m_basicGroup;
    QDoubleSpinBox *m_damageSpin;
    QLineEdit *m_spriteEdit;

    // Collider
    QGroupBox *m_colliderGroup;
    QDoubleSpinBox *m_offsetXSpin;
    QDoubleSpinBox *m_offsetYSpin;
    QComboBox *m_colliderTypeCombo;
    QStackedWidget *m_colliderTypeStack;

    // CircleCollider
    QGroupBox *m_circleGroup;
    QDoubleSpinBox *m_circleRadiusSpin;

    // RectangleCollider
    QGroupBox *m_rectangleGroup;
    QDoubleSpinBox *m_rectangleAngleSpin;
    QDoubleSpinBox *m_rectangleWidthSpin;
    QDoubleSpinBox *m_rectangleHeightSpin;

    // BulletLifeConfig (optional)
    QCheckBox *m_lifeConfigCheck;
    QGroupBox *m_lifeConfigGroup;
    QDoubleSpinBox *m_lifeConfigLifespanSpin;
    QSpinBox *m_lifeConfigAttackIdSpin;

    game::Bullet *m_currentBullet;
    QUndoStack *m_undoStack;
};

#endif // BULLETINSPECTOR_H
