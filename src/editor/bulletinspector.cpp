// BulletInspector.cpp
#include "bulletinspector.h"

namespace {
const QString kBasicGroupTitle = "Basic";
const QString kColliderGroupTitle = "Collider";
const QString kLifeConfigTitle = "Bullet Life Config";

const int kMinWidth = 300;
const double kMaxAngle = 360;
const double kMaxValue = 9999;
const double kMinNegative = -9999;
const double kSingleStep = 0.1;
} // namespace

BulletInspector::BulletInspector(QUndoStack *undoStack, QWidget *parent)
    : QWidget(parent),
      m_basicGroup(new QGroupBox(kBasicGroupTitle)),
      m_damageSpin(new QDoubleSpinBox()),
      m_spriteEdit(new QLineEdit()),
      m_colliderGroup(new QGroupBox(kColliderGroupTitle)),
      m_offsetXSpin(new QDoubleSpinBox()),
      m_offsetYSpin(new QDoubleSpinBox()),
      m_colliderTypeCombo(new QComboBox()),
      m_colliderTypeStack(new QStackedWidget()),
      m_circleGroup(new QGroupBox("Circle Collider")),
      m_circleRadiusSpin(new QDoubleSpinBox()),
      m_rectangleGroup(new QGroupBox("Rectangle Collider")),
      m_rectangleAngleSpin(new QDoubleSpinBox()),
      m_rectangleWidthSpin(new QDoubleSpinBox()),
      m_rectangleHeightSpin(new QDoubleSpinBox()),
      m_lifeConfigCheck(new QCheckBox("Enable Life Config")),
      m_lifeConfigGroup(new QGroupBox(kLifeConfigTitle)),
      m_lifeConfigLifespanSpin(new QDoubleSpinBox()),
      m_lifeConfigAttackIdSpin(new QSpinBox()),
      m_currentBullet(nullptr),
      m_undoStack(undoStack)
{
    setMinimumWidth(kMinWidth);
    setupUI();
}

void BulletInspector::setupUI()
{
    auto *mainLayout = new QFormLayout(this);

    // --- Basic ---
    auto *basicLayout = new QFormLayout(m_basicGroup);
    m_damageSpin->setRange(0, kMaxValue);
    m_damageSpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Damage:", m_damageSpin);

    basicLayout->addRow("Sprite:", m_spriteEdit);
    m_basicGroup->setLayout(basicLayout);
    mainLayout->addRow(m_basicGroup);

    // --- Collider ---
    auto *colliderLayout = new QFormLayout(m_colliderGroup);
    // Offset
    m_offsetXSpin->setRange(kMinNegative, kMaxValue);
    m_offsetXSpin->setSingleStep(kSingleStep);
    m_offsetYSpin->setRange(kMinNegative, kMaxValue);
    m_offsetYSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Offset X:", m_offsetXSpin);
    colliderLayout->addRow("Offset Y:", m_offsetYSpin);

    // Type combo
    m_colliderTypeCombo->addItem("Circle");
    m_colliderTypeCombo->addItem("Rectangle");
    colliderLayout->addRow("Type:", m_colliderTypeCombo);

    // Circle sub-widget
    auto *circleForm = new QFormLayout(m_circleGroup);
    m_circleRadiusSpin->setRange(0, kMaxValue);
    m_circleRadiusSpin->setSingleStep(kSingleStep);
    circleForm->addRow("Radius:", m_circleRadiusSpin);
    m_circleGroup->setLayout(circleForm);

    // Rectangle sub-widget
    auto *rectForm = new QFormLayout(m_rectangleGroup);
    m_rectangleAngleSpin->setRange(0, kMaxAngle);
    m_rectangleAngleSpin->setSingleStep(kSingleStep);
    rectForm->addRow("Angle:", m_rectangleAngleSpin);
    m_rectangleWidthSpin->setRange(0, kMaxValue);
    m_rectangleWidthSpin->setSingleStep(kSingleStep);
    rectForm->addRow("Width:", m_rectangleWidthSpin);
    m_rectangleHeightSpin->setRange(0, kMaxValue);
    m_rectangleHeightSpin->setSingleStep(kSingleStep);
    rectForm->addRow("Height:", m_rectangleHeightSpin);
    m_rectangleGroup->setLayout(rectForm);

    // Stack
    m_colliderTypeStack->addWidget(m_circleGroup);
    m_colliderTypeStack->addWidget(m_rectangleGroup);
    colliderLayout->addRow(m_colliderTypeStack);

    m_colliderGroup->setLayout(colliderLayout);
    mainLayout->addRow(m_colliderGroup);

    // --- Life Config ---
    mainLayout->addRow(m_lifeConfigCheck);
    auto *lifeForm = new QFormLayout(m_lifeConfigGroup);
    m_lifeConfigLifespanSpin->setRange(0, kMaxValue);
    m_lifeConfigLifespanSpin->setSingleStep(kSingleStep);
    lifeForm->addRow("Lifespan:", m_lifeConfigLifespanSpin);
    m_lifeConfigAttackIdSpin->setRange(0, INT_MAX);
    lifeForm->addRow("Attack ID:", m_lifeConfigAttackIdSpin);
    m_lifeConfigGroup->setLayout(lifeForm);
    m_lifeConfigGroup->setVisible(false);
    mainLayout->addRow(m_lifeConfigGroup);

    // --- Connections ---
    connect(m_damageSpin, &QDoubleSpinBox::valueChanged, this, &BulletInspector::onDamageChanged);
    connect(m_spriteEdit, &QLineEdit::textChanged, this, &BulletInspector::onSpriteChanged);
    connect(m_offsetXSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &BulletInspector::onOffsetXChanged);
    connect(m_offsetYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &BulletInspector::onOffsetYChanged);
    connect(m_colliderTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &BulletInspector::onColliderTypeChanged);
    connect(m_circleRadiusSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &BulletInspector::onCircleRadiusChanged);
    connect(m_rectangleAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &BulletInspector::onRectangleAngleChanged);
    connect(m_rectangleWidthSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &BulletInspector::onRectangleWidthChanged);
    connect(m_rectangleHeightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &BulletInspector::onRectangleHeightChanged);
    connect(m_lifeConfigCheck, &QCheckBox::stateChanged, this,
            &BulletInspector::onLifeConfigToggled);
    connect(m_lifeConfigLifespanSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &BulletInspector::onLifeConfigLifespanChanged);
    connect(m_lifeConfigAttackIdSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &BulletInspector::onLifeConfigAttackIdChanged);
}

void BulletInspector::setBullet(game::Bullet *bullet)
{
    m_currentBullet = bullet;
    updateFields();
}

void BulletInspector::clear()
{
    m_currentBullet = nullptr;
    clearFields();
}

void BulletInspector::updateFields()
{
    if (!m_currentBullet) {
        return;
    }

    // Basic
    m_damageSpin->setValue(m_currentBullet->damage());
    m_spriteEdit->setText(QString::fromStdString(m_currentBullet->sprite()));

    // Collider
    const auto &col = m_currentBullet->collider();
    m_offsetXSpin->setValue(col.offset().x());
    m_offsetYSpin->setValue(col.offset().y());
    updateColliderTypeFields();

    // Life Config
    updateLifeConfigFields();
}

void BulletInspector::clearFields()
{
    // Basic
    m_damageSpin->setValue(0);
    m_spriteEdit->clear();

    // Collider
    m_offsetXSpin->setValue(0);
    m_offsetYSpin->setValue(0);
    m_colliderTypeCombo->setCurrentIndex(0);
    m_circleRadiusSpin->setValue(0);
    m_rectangleAngleSpin->setValue(0);
    m_rectangleWidthSpin->setValue(0);
    m_rectangleHeightSpin->setValue(0);

    // Life Config
    m_lifeConfigCheck->setChecked(false);
    m_lifeConfigLifespanSpin->setValue(0);
    m_lifeConfigAttackIdSpin->setValue(0);
    m_lifeConfigGroup->setVisible(false);
}

void BulletInspector::updateColliderTypeFields()
{
    if (!m_currentBullet) {
        return;
    }
    const auto &col = m_currentBullet->collider();
    if (col.has_circle()) {
        m_colliderTypeCombo->setCurrentIndex(0);
        m_colliderTypeStack->setCurrentWidget(m_circleGroup);
        m_circleRadiusSpin->setValue(col.circle().radius());
    } else if (col.has_rectangle()) {
        m_colliderTypeCombo->setCurrentIndex(1);
        m_colliderTypeStack->setCurrentWidget(m_rectangleGroup);
        m_rectangleAngleSpin->setValue(col.rectangle().angle());
        m_rectangleWidthSpin->setValue(col.rectangle().width());
        m_rectangleHeightSpin->setValue(col.rectangle().height());
    }
}

void BulletInspector::updateLifeConfigFields()
{
    if (!m_currentBullet) {
        return;
    }
    if (m_currentBullet->has_bullet_life_config()) {
        m_lifeConfigCheck->setChecked(true);
        m_lifeConfigGroup->setVisible(true);
        const auto &cfg = m_currentBullet->bullet_life_config();
        m_lifeConfigLifespanSpin->setValue(cfg.lifespan());
        m_lifeConfigAttackIdSpin->setValue(cfg.attack_id());
    } else {
        m_lifeConfigCheck->setChecked(false);
        m_lifeConfigGroup->setVisible(false);
    }
}

// --- Slots implementation ---
void BulletInspector::onDamageChanged(double v)
{
    if (m_currentBullet) {
        m_currentBullet->set_damage(static_cast<float>(v));
    }
}
void BulletInspector::onSpriteChanged(const QString &t)
{
    if (m_currentBullet) {
        m_currentBullet->set_sprite(t.toStdString());
    }
}
void BulletInspector::onOffsetXChanged(double v)
{
    if (m_currentBullet) {
        m_currentBullet->mutable_collider()->mutable_offset()->set_x(static_cast<float>(v));
    }
}
void BulletInspector::onOffsetYChanged(double v)
{
    if (m_currentBullet) {
        m_currentBullet->mutable_collider()->mutable_offset()->set_y(static_cast<float>(v));
    }
}
void BulletInspector::onColliderTypeChanged(int idx)
{
    if (!m_currentBullet) {
        return;
    }
    auto *col = m_currentBullet->mutable_collider();
    col->clear_circle();
    col->clear_rectangle();
    if (idx == 0) {
        col->mutable_circle();
    } else {
        col->mutable_rectangle();
    }
    updateColliderTypeFields();
}
void BulletInspector::onCircleRadiusChanged(double v)
{
    if (m_currentBullet && m_currentBullet->collider().has_circle()) {
        m_currentBullet->mutable_collider()->mutable_circle()->set_radius(static_cast<float>(v));
    }
}
void BulletInspector::onRectangleAngleChanged(double v)
{
    if (m_currentBullet && m_currentBullet->collider().has_rectangle()) {
        m_currentBullet->mutable_collider()->mutable_rectangle()->set_angle(static_cast<float>(v));
    }
}
void BulletInspector::onRectangleWidthChanged(double v)
{
    if (m_currentBullet && m_currentBullet->collider().has_rectangle()) {
        m_currentBullet->mutable_collider()->mutable_rectangle()->set_width(static_cast<float>(v));
    }
}
void BulletInspector::onRectangleHeightChanged(double v)
{
    if (m_currentBullet && m_currentBullet->collider().has_rectangle()) {
        m_currentBullet->mutable_collider()->mutable_rectangle()->set_height(static_cast<float>(v));
    }
}
void BulletInspector::onLifeConfigToggled(int state)
{
    if (!m_currentBullet) {
        return;
    }
    if (state == Qt::Checked) {
        m_currentBullet->mutable_bullet_life_config();
        m_lifeConfigGroup->setVisible(true);
    } else {
        m_currentBullet->clear_bullet_life_config();
        m_lifeConfigGroup->setVisible(false);
    }
}
void BulletInspector::onLifeConfigLifespanChanged(double v)
{
    if (m_currentBullet && m_currentBullet->has_bullet_life_config()) {
        m_currentBullet->mutable_bullet_life_config()->set_lifespan(static_cast<float>(v));
    }
}
void BulletInspector::onLifeConfigAttackIdChanged(int v)
{
    if (m_currentBullet && m_currentBullet->has_bullet_life_config()) {
        m_currentBullet->mutable_bullet_life_config()->set_attack_id(v);
    }
}
