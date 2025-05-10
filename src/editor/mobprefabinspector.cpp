// mobprefabinspector.cpp
#include "mobprefabinspector.h"

#include <QLabel>

namespace {
const QString kBasicGroupTitle = "Basic";
const QString kDropGroupTitle = "Drop";
const QString kColliderGroupTitle = "Collider";

const int kMinWidth = 300;
const double kMaxHealth = 999999;
const double kMaxValue = 999;
const double kMinPositive = 0.1;
const double kMinNegative = -999;
const double kDefaultColliderSpin = 10.0;
const double kSingleStep = 0.1;

const int kDropFixedAmountIndex = 3;
const int kDropRangeIndex = 4;

const int kCircleRadiusIndex = 3;
const int kRectWidthIndex = 4;
const int kRectHeightIndex = 5;
const int kRectAngleIndex = 6;

enum class ColliderType {
    Circle,
    Rectangle,
};
} // namespace

MobPrefabInspector::MobPrefabInspector(QUndoStack *undoStack, QWidget *parent)
    : QWidget(parent),
      m_basicGroup(new QGroupBox(kBasicGroupTitle)),
      m_nameEdit(new QLineEdit()),
      m_healthSpin(new QDoubleSpinBox()),
      m_spriteEdit(new QLineEdit()),
      m_attackIdSpin(new QSpinBox()),
      m_dropGroup(new QGroupBox(kDropGroupTitle)),
      m_dropItemEdit(new QLineEdit()),
      m_dropWeightSpin(new QDoubleSpinBox()),
      m_fixedAmountCheck(new QCheckBox("Fixed amount")),
      m_fixedAmountSpin(new QSpinBox()),
      m_minAmountSpin(new QSpinBox()),
      m_maxAmountSpin(new QSpinBox()),
      m_colliderGroup(new QGroupBox(kColliderGroupTitle)),
      m_colliderOffsetXSpin(new QDoubleSpinBox()),
      m_colliderOffsetYSpin(new QDoubleSpinBox()),
      m_colliderTypeCombo(new QComboBox()),
      m_circleRadiusSpin(new QDoubleSpinBox()),
      m_rectWidthSpin(new QDoubleSpinBox()),
      m_rectHeightSpin(new QDoubleSpinBox()),
      m_rectAngleSpin(new QDoubleSpinBox()),
      m_currentMobPrefab(nullptr),
      m_undoStack(undoStack)
{
    setMinimumWidth(kMinWidth);
    setupUI();

    m_fixedAmountCheck->setChecked(true);
    m_colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
}

void MobPrefabInspector::setMobPrefab(game::MobPrefab *mobPrefab)
{
    m_currentMobPrefab = mobPrefab;
    updateFields();
}

void MobPrefabInspector::clear()
{
    m_currentMobPrefab = nullptr;
    clearFields();
}

void MobPrefabInspector::setupUI()
{
    QFormLayout *layout = new QFormLayout(this);

    // Basic fields
    QFormLayout *basicLayout = new QFormLayout(m_basicGroup);
    basicLayout->addRow("Name:", m_nameEdit);

    m_healthSpin->setRange(0, kMaxHealth);
    m_healthSpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Health:", m_healthSpin);

    basicLayout->addRow("Sprite:", m_spriteEdit);

    m_attackIdSpin->setRange(0, INT_MAX);
    basicLayout->addRow("Attack ID:", m_attackIdSpin);

    m_basicGroup->setLayout(basicLayout);
    layout->addRow(m_basicGroup);

    // Drop section
    QFormLayout *dropLayout = new QFormLayout(m_dropGroup);

    dropLayout->addRow("Item:", m_dropItemEdit);

    m_dropWeightSpin->setRange(kMinPositive, kMaxValue);
    m_dropWeightSpin->setSingleStep(kSingleStep);
    dropLayout->addRow("Weight:", m_dropWeightSpin);

    m_fixedAmountCheck->setChecked(true);
    dropLayout->addRow(m_fixedAmountCheck);

    m_fixedAmountSpin->setRange(1, INT_MAX);
    dropLayout->addRow("Fixed amount:", m_fixedAmountSpin);

    m_minAmountSpin->setRange(1, INT_MAX);
    m_maxAmountSpin->setRange(1, INT_MAX);
    QHBoxLayout *rangeLayout = new QHBoxLayout();
    rangeLayout->addWidget(new QLabel("Min:"));
    rangeLayout->addWidget(m_minAmountSpin);
    rangeLayout->addWidget(new QLabel("Max:"));
    rangeLayout->addWidget(m_maxAmountSpin);
    dropLayout->addRow("Range amount:", rangeLayout);

    m_dropGroup->setLayout(dropLayout);
    layout->addRow(m_dropGroup);

    // Collider section
    QFormLayout *colliderLayout = new QFormLayout(m_colliderGroup);

    m_colliderOffsetXSpin->setRange(kMinNegative, kMaxValue);
    colliderLayout->addRow("Offset X:", m_colliderOffsetXSpin);

    m_colliderOffsetYSpin->setRange(kMinNegative, kMaxValue);
    colliderLayout->addRow("Offset Y:", m_colliderOffsetYSpin);

    m_colliderTypeCombo->addItem("Circle");
    m_colliderTypeCombo->addItem("Rectangle");
    colliderLayout->addRow("Type:", m_colliderTypeCombo);

    m_circleRadiusSpin->setRange(kMinPositive, kMaxValue);
    m_circleRadiusSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Radius:", m_circleRadiusSpin);

    m_rectWidthSpin->setRange(kMinPositive, kMaxValue);
    m_rectWidthSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Width:", m_rectWidthSpin);

    m_rectHeightSpin->setRange(kMinPositive, kMaxValue);
    m_rectHeightSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Height:", m_rectHeightSpin);

    m_rectAngleSpin->setRange(kMinNegative, kMaxValue);
    m_rectAngleSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Angle:", m_rectAngleSpin);

    m_colliderGroup->setLayout(colliderLayout);
    layout->addRow(m_colliderGroup);

    // Connect signals
    connect(m_nameEdit, &QLineEdit::textChanged, this, &MobPrefabInspector::onNameChanged);
    connect(m_healthSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onHealthChanged);
    connect(m_spriteEdit, &QLineEdit::textChanged, this, &MobPrefabInspector::onSpriteChanged);
    connect(m_attackIdSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobPrefabInspector::onAttackIdChanged);
    connect(m_dropItemEdit, &QLineEdit::textChanged, this, &MobPrefabInspector::onDropItemChanged);
    connect(m_dropWeightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onDropWeightChanged);
    connect(m_fixedAmountCheck, &QCheckBox::stateChanged, this,
            &MobPrefabInspector::onFixedAmountChanged);
    connect(m_fixedAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobPrefabInspector::onFixedAmountValueChanged);
    connect(m_minAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobPrefabInspector::onMinAmountChanged);
    connect(m_maxAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobPrefabInspector::onMaxAmountChanged);
    connect(m_colliderOffsetXSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onColliderOffsetXChanged);
    connect(m_colliderOffsetYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onColliderOffsetYChanged);
    connect(m_colliderTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MobPrefabInspector::onColliderTypeChanged);
    connect(m_circleRadiusSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onCircleRadiusChanged);
    connect(m_rectWidthSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onRectWidthChanged);
    connect(m_rectHeightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onRectHeightChanged);
    connect(m_rectAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobPrefabInspector::onRectAngleChanged);
}

void MobPrefabInspector::updateFields()
{
    if (!m_currentMobPrefab) {
        return;
    }

    m_nameEdit->setText(QString::fromStdString(m_currentMobPrefab->name()));
    m_healthSpin->setValue(m_currentMobPrefab->health());
    m_spriteEdit->setText(QString::fromStdString(m_currentMobPrefab->sprite()));
    m_attackIdSpin->setValue(m_currentMobPrefab->attack_id());

    updateDropFields();
    updateColliderFields();
}

void MobPrefabInspector::updateDropFields()
{
    if (!m_currentMobPrefab || !m_currentMobPrefab->has_drop_item()) {
        m_dropItemEdit->clear();
        m_dropWeightSpin->setValue(1.0);
        m_fixedAmountCheck->setChecked(true);
        m_fixedAmountSpin->setValue(1);
        return;
    }

    const auto &drop = m_currentMobPrefab->drop_item();
    m_dropItemEdit->setText(QString::fromStdString(drop.item()));
    m_dropWeightSpin->setValue(drop.weight());

    if (drop.fixed_amount()) {
        m_fixedAmountCheck->setChecked(true);
        if (drop.has_fixed()) {
            m_fixedAmountSpin->setValue(drop.fixed());
        }
    } else {
        m_fixedAmountCheck->setChecked(false);
        if (drop.has_range()) {
            m_minAmountSpin->setValue(drop.range().min());
            m_maxAmountSpin->setValue(drop.range().max());
        }
    }
}

void MobPrefabInspector::updateColliderFields()
{
    if (!m_currentMobPrefab || !m_currentMobPrefab->has_collider()) {
        m_colliderOffsetXSpin->setValue(0);
        m_colliderOffsetYSpin->setValue(0);
        m_colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
        m_circleRadiusSpin->setValue(kDefaultColliderSpin);
        m_rectWidthSpin->setValue(kDefaultColliderSpin);
        m_rectHeightSpin->setValue(kDefaultColliderSpin);
        m_rectAngleSpin->setValue(0);
        return;
    }

    const auto &collider = m_currentMobPrefab->collider();
    m_colliderOffsetXSpin->setValue(collider.offset().x());
    m_colliderOffsetYSpin->setValue(collider.offset().y());

    QFormLayout *colliderLayout = qobject_cast<QFormLayout *>(m_colliderGroup->layout());

    if (collider.has_circle()) {
        m_colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
        m_circleRadiusSpin->setValue(collider.circle().radius());
        colliderLayout->setRowVisible(kCircleRadiusIndex, true);
        colliderLayout->setRowVisible(kRectWidthIndex, false);
        colliderLayout->setRowVisible(kRectHeightIndex, false);
        colliderLayout->setRowVisible(kRectAngleIndex, false);
    } else if (collider.has_rectangle()) {
        m_colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Rectangle));
        m_rectWidthSpin->setValue(collider.rectangle().width());
        m_rectHeightSpin->setValue(collider.rectangle().height());
        m_rectAngleSpin->setValue(collider.rectangle().angle());
        colliderLayout->setRowVisible(kCircleRadiusIndex, false);
        colliderLayout->setRowVisible(kRectWidthIndex, true);
        colliderLayout->setRowVisible(kRectHeightIndex, true);
        colliderLayout->setRowVisible(kRectAngleIndex, true);
    }
}

void MobPrefabInspector::clearFields()
{
    m_nameEdit->clear();
    m_healthSpin->setValue(0);
    m_spriteEdit->clear();
    m_attackIdSpin->setValue(0);

    m_dropItemEdit->clear();
    m_dropWeightSpin->setValue(1.0);
    m_fixedAmountCheck->setChecked(true);
    m_fixedAmountSpin->setValue(1);
    m_minAmountSpin->setValue(1);
    m_maxAmountSpin->setValue(1);

    m_colliderOffsetXSpin->setValue(0);
    m_colliderOffsetYSpin->setValue(0);
    m_colliderTypeCombo->setCurrentIndex(0);
    m_circleRadiusSpin->setValue(kDefaultColliderSpin);
    m_rectWidthSpin->setValue(kDefaultColliderSpin);
    m_rectHeightSpin->setValue(kDefaultColliderSpin);
    m_rectAngleSpin->setValue(0);
}

// Signal handlers implementation
void MobPrefabInspector::onNameChanged(const QString &text)
{
    if (m_currentMobPrefab) {
        m_currentMobPrefab->set_name(text.toStdString());
    }
}

void MobPrefabInspector::onHealthChanged(double value)
{
    if (m_currentMobPrefab) {
        m_currentMobPrefab->set_health(value);
    }
}

void MobPrefabInspector::onSpriteChanged(const QString &text)
{
    if (m_currentMobPrefab) {
        m_currentMobPrefab->set_sprite(text.toStdString());
    }
}

void MobPrefabInspector::onAttackIdChanged(int value)
{
    if (m_currentMobPrefab) {
        m_currentMobPrefab->set_attack_id(value);
    }
}

void MobPrefabInspector::onDropItemChanged(const QString &text)
{
    if (m_currentMobPrefab) {
        if (!m_currentMobPrefab->has_drop_item()) {
            m_currentMobPrefab->mutable_drop_item();
        }
        m_currentMobPrefab->mutable_drop_item()->set_item(text.toStdString());
    }
}

void MobPrefabInspector::onDropWeightChanged(double value)
{
    if (m_currentMobPrefab) {
        if (!m_currentMobPrefab->has_drop_item()) {
            m_currentMobPrefab->mutable_drop_item();
        }
        m_currentMobPrefab->mutable_drop_item()->set_weight(value);
    }
}

void MobPrefabInspector::onFixedAmountChanged(int state)
{
    bool fixed = state == Qt::Checked;
    auto layout = qobject_cast<QFormLayout *>(m_dropGroup->layout());

    if (fixed) {
        layout->setRowVisible(kDropFixedAmountIndex, true);
        layout->setRowVisible(kDropRangeIndex, false);
    } else {
        layout->setRowVisible(kDropFixedAmountIndex, false);
        layout->setRowVisible(kDropRangeIndex, true);
    }

    if (!m_currentMobPrefab) {
        return;
    }

    if (!m_currentMobPrefab->has_drop_item()) {
        m_currentMobPrefab->mutable_drop_item();
    }

    m_currentMobPrefab->mutable_drop_item()->set_fixed_amount(fixed);
}

void MobPrefabInspector::onFixedAmountValueChanged(int value)
{
    if (!m_currentMobPrefab) {
        return;
    }

    if (!m_currentMobPrefab->has_drop_item()) {
        m_currentMobPrefab->mutable_drop_item();
    }

    m_currentMobPrefab->mutable_drop_item()->set_fixed(value);
}

void MobPrefabInspector::onMinAmountChanged(int value)
{
    if (!m_currentMobPrefab) {
        return;
    }

    if (!m_currentMobPrefab->has_drop_item() || !m_currentMobPrefab->drop_item().has_range()) {
        m_currentMobPrefab->mutable_drop_item()->mutable_range();
    }

    m_currentMobPrefab->mutable_drop_item()->mutable_range()->set_min(value);
}

void MobPrefabInspector::onMaxAmountChanged(int value)
{
    if (!m_currentMobPrefab) {
        return;
    }

    if (!m_currentMobPrefab->has_drop_item() || !m_currentMobPrefab->drop_item().has_range()) {
        m_currentMobPrefab->mutable_drop_item()->mutable_range();
    }

    m_currentMobPrefab->mutable_drop_item()->mutable_range()->set_max(value);
}

void MobPrefabInspector::onColliderOffsetXChanged(double value)
{
    if (m_currentMobPrefab) {
        if (!m_currentMobPrefab->has_collider()) {
            m_currentMobPrefab->mutable_collider();
        }
        m_currentMobPrefab->mutable_collider()->mutable_offset()->set_x(static_cast<float>(value));
    }
}

void MobPrefabInspector::onColliderOffsetYChanged(double value)
{
    if (m_currentMobPrefab) {
        if (!m_currentMobPrefab->has_collider()) {
            m_currentMobPrefab->mutable_collider();
        }
        m_currentMobPrefab->mutable_collider()->mutable_offset()->set_y(static_cast<float>(value));
    }
}

void MobPrefabInspector::onColliderTypeChanged(int index)
{
    if (!m_currentMobPrefab) {
        return;
    }

    if (!m_currentMobPrefab->has_collider()) {
        m_currentMobPrefab->mutable_collider();
    }

    auto *collider = m_currentMobPrefab->mutable_collider();
    if (index == static_cast<int>(ColliderType::Circle)) {
        if (!collider->has_circle()) {
            collider->mutable_circle()->set_radius(static_cast<float>(kDefaultColliderSpin));
        }
        collider->clear_rectangle();
    } else {
        if (!collider->has_rectangle()) {
            collider->mutable_rectangle()->set_width(static_cast<float>(kDefaultColliderSpin));
            collider->mutable_rectangle()->set_height(static_cast<float>(kDefaultColliderSpin));
            collider->mutable_rectangle()->set_angle(0);
        }
        collider->clear_circle();
    }

    updateColliderFields();
}

void MobPrefabInspector::onCircleRadiusChanged(double value)
{
    if (m_currentMobPrefab && m_currentMobPrefab->has_collider()
        && m_currentMobPrefab->collider().has_circle()) {
        m_currentMobPrefab->mutable_collider()->mutable_circle()->set_radius(
                static_cast<float>(value));
    }
}

void MobPrefabInspector::onRectWidthChanged(double value)
{
    if (m_currentMobPrefab && m_currentMobPrefab->has_collider()
        && m_currentMobPrefab->collider().has_rectangle()) {
        m_currentMobPrefab->mutable_collider()->mutable_rectangle()->set_width(
                static_cast<float>(value));
    }
}

void MobPrefabInspector::onRectHeightChanged(double value)
{
    if (m_currentMobPrefab && m_currentMobPrefab->has_collider()
        && m_currentMobPrefab->collider().has_rectangle()) {
        m_currentMobPrefab->mutable_collider()->mutable_rectangle()->set_height(
                static_cast<float>(value));
    }
}

void MobPrefabInspector::onRectAngleChanged(double value)
{
    if (m_currentMobPrefab && m_currentMobPrefab->has_collider()
        && m_currentMobPrefab->collider().has_rectangle()) {
        m_currentMobPrefab->mutable_collider()->mutable_rectangle()->set_angle(
                static_cast<float>(value));
    }
}
