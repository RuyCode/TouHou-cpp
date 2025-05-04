#include "mobinspector.h"

#include "command.h"
#include "datamanager.h"

namespace {
const QString kBasicGroupTitle = "Basic";
const QString kBasicGroupReadOnlyTitle = "From Prefab";
const QString kPathGroupTitle = "Path";
const QString kDropGroupTitle = "Drop";
const QString kColliderGroupTitle = "Collider (from Prefab)";

const int kMinWidth = 300;
const double kMaxHealth = 999999;
const double kMaxValue = 999;
const double kMinPositive = 0.1;
const double kMinNegative = -999;
const double kMaxPathSpin = 9999;
const double kMinPathSpin = -9999;
const double kDefaultColliderSpin = 10.0;

const double kSingleStep = 0.1;
const int kPathGroupOffset = 2; // travel time and add button

const int kDeleteButtonWidthPadding = 10;

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

MobInspector::MobInspector(QUndoStack *undoStack, QWidget *parent)
    : QWidget(parent),
      m_basicGroup(new QGroupBox(kBasicGroupTitle)),
      m_nameEdit(new QLineEdit()),
      m_mobPrefabIdSpin(new QSpinBox()),
      m_spawnDelaySpin(new QDoubleSpinBox()),
      m_basicPrefabGroup(new QGroupBox(kBasicGroupReadOnlyTitle)),
      m_healthSpin(new QDoubleSpinBox()),
      m_spriteEdit(new QLineEdit()),
      m_attackIdSpin(new QSpinBox()),
      m_pathGroup(new QGroupBox(kPathGroupTitle)),
      m_travelTimeSpin(new QDoubleSpinBox()),
      m_addPathPointButton(new QPushButton()),
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
      m_currentMob(nullptr),
      m_undoStack(undoStack)
{
    setMinimumWidth(kMinWidth);
    setBaseSize(kMinWidth, 0);

    setupUI();

    m_fixedAmountCheck->setChecked(true);
    m_colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));

    m_fixedAmountCheck->stateChanged(Qt::Checked);
}

void MobInspector::setMob(game::Mob *mob)
{
    m_currentMob = mob;
    updateFields();
}

void MobInspector::clear()
{
    m_currentMob = nullptr;
    clearFields();
}

void MobInspector::setupUI()
{
    QFormLayout *layout = new QFormLayout(this);

    // Basic fields
    QFormLayout *basicLayout = new QFormLayout(m_basicGroup);
    basicLayout->addRow("Name:", m_nameEdit);

    m_mobPrefabIdSpin->setRange(0, INT_MAX);
    basicLayout->addRow("Mob Prefab ID:", m_mobPrefabIdSpin);

    m_healthSpin->setRange(0, kMaxHealth);
    m_healthSpin->setSingleStep(kSingleStep);

    m_spawnDelaySpin->setRange(0, kMaxValue);
    m_spawnDelaySpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Spawn Delay:", m_spawnDelaySpin);

    QFormLayout *basicPrefabLayout = new QFormLayout(m_basicPrefabGroup);
    basicPrefabLayout->addRow("Health:", m_healthSpin);

    basicPrefabLayout->addRow("Sprite:", m_spriteEdit);

    m_attackIdSpin->setRange(0, INT_MAX);
    basicPrefabLayout->addRow("Attack ID:", m_attackIdSpin);

    m_basicPrefabGroup->setLayout(basicPrefabLayout);
    basicLayout->addRow(m_basicPrefabGroup);

    m_basicGroup->setLayout(basicLayout);
    layout->addRow(m_basicGroup);

    // Path section
    QFormLayout *pathLayout = new QFormLayout(m_pathGroup);

    m_travelTimeSpin->setRange(0, kMaxValue);
    m_travelTimeSpin->setSingleStep(kSingleStep);
    pathLayout->addRow("Travel Time:", m_travelTimeSpin);

    m_addPathPointButton->setText("Add key point");
    pathLayout->addRow(m_addPathPointButton);

    m_pathGroup->setLayout(pathLayout);
    layout->addRow(m_pathGroup);

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

    // Collider section (read-only, from prefab)
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

    // Make collider fields read-only since they come from prefab
    m_colliderOffsetXSpin->setReadOnly(true);
    m_colliderOffsetYSpin->setReadOnly(true);
    m_colliderTypeCombo->setEnabled(false);
    m_circleRadiusSpin->setReadOnly(true);
    m_rectWidthSpin->setReadOnly(true);
    m_rectHeightSpin->setReadOnly(true);
    m_rectAngleSpin->setReadOnly(true);

    m_healthSpin->setReadOnly(true);
    m_spriteEdit->setReadOnly(true);
    m_attackIdSpin->setReadOnly(true);

    // Connect signals
    connect(m_nameEdit, &QLineEdit::textChanged, this, &MobInspector::onNameChanged);
    connect(m_mobPrefabIdSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobInspector::onMobPrefabIdChanged);
    connect(m_spawnDelaySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobInspector::onSpawnDelayChanged);
    connect(m_travelTimeSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobInspector::onTravelTimeChanged);
    connect(m_addPathPointButton, &QPushButton::clicked, this,
            &MobInspector::onAddPathPointClicked);
    connect(m_dropItemEdit, &QLineEdit::textChanged, this, &MobInspector::onDropItemChanged);
    connect(m_dropWeightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobInspector::onDropWeightChanged);
    connect(m_fixedAmountCheck, &QCheckBox::stateChanged, this,
            &MobInspector::onFixedAmountChanged);
    connect(m_fixedAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobInspector::onFixedAmountValueChanged);
    connect(m_minAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobInspector::onMinAmountChanged);
    connect(m_maxAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &MobInspector::onMaxAmountChanged);
}

void MobInspector::updateFields()
{
    if (!m_currentMob) {
        return;
    }

    // Get prefab
    game::Level &level = DataManager::getLevel();
    if (m_currentMob->mob_prefab_id() >= level.mob_prefabs_size()) {
        return;
    }

    const auto &prefab = level.mob_prefabs(m_currentMob->mob_prefab_id());

    m_nameEdit->setText(QString::fromStdString(m_currentMob->name()));
    m_mobPrefabIdSpin->setValue(m_currentMob->mob_prefab_id());
    m_healthSpin->setValue(prefab.health());
    m_spriteEdit->setText(QString::fromStdString(prefab.sprite()));
    m_spawnDelaySpin->setValue(m_currentMob->spawn_delay());
    m_attackIdSpin->setValue(prefab.attack_id());

    // Update path
    updatePathFields();

    // Update drop
    if (m_currentMob->has_drop_item()) {
        const auto &drop = m_currentMob->drop_item();
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
    } else {
        m_dropItemEdit->clear();
        m_dropWeightSpin->setValue(1.0);
        m_fixedAmountCheck->setChecked(true);
        m_fixedAmountSpin->setValue(1);
    }

    // Update collider from prefab
    updateColliderFields();
}

void MobInspector::updatePathFields()
{
    if (!m_currentMob || !m_currentMob->has_path()) {
        qDeleteAll(m_pathPointSpins);
        m_pathPointSpins.clear();
        m_travelTimeSpin->setValue(0);
        return;
    }

    const auto &path = m_currentMob->path();
    m_travelTimeSpin->setValue(path.travel_time());

    qDeleteAll(m_pathPointSpins);
    m_pathPointSpins.clear();

    QFormLayout *pathLayout = qobject_cast<QFormLayout *>(m_pathGroup->layout());
    while (pathLayout->rowCount() > kPathGroupOffset) {
        pathLayout->removeRow(kPathGroupOffset);
    }

    for (int i = 0; i < path.key_points_size(); ++i) {
        const auto &point = path.key_points(i);

        addPathPointRow(i);
        m_pathPointSpins[i * 2]->setValue(point.x());
        m_pathPointSpins[i * 2 + 1]->setValue(point.y());
    }
}

void MobInspector::updateColliderFields()
{
    if (!m_currentMob) {
        m_colliderOffsetXSpin->setValue(0);
        m_colliderOffsetYSpin->setValue(0);
        m_colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
        m_circleRadiusSpin->setValue(0);
        m_rectWidthSpin->setValue(0);
        m_rectHeightSpin->setValue(0);
        m_rectAngleSpin->setValue(0);
        return;
    }

    // Get collider from prefab
    game::Level &level = DataManager::getLevel();
    if (m_currentMob->mob_prefab_id() >= level.mob_prefabs_size()) {
        return;
    }

    const auto &prefab = level.mob_prefabs(m_currentMob->mob_prefab_id());
    if (!prefab.has_collider()) {
        return;
    }

    const auto &collider = prefab.collider();
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

void MobInspector::clearFields()
{
    m_nameEdit->clear();
    m_mobPrefabIdSpin->setValue(0);
    m_healthSpin->setValue(0);
    m_spriteEdit->clear();
    m_spawnDelaySpin->setValue(0);
    m_attackIdSpin->setValue(0);

    qDeleteAll(m_pathPointSpins);
    m_pathPointSpins.clear();
    m_travelTimeSpin->setValue(0);

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

void MobInspector::addPathPointRow(int pointIndex)
{
    QFormLayout *pathLayout = qobject_cast<QFormLayout *>(m_pathGroup->layout());

    QLabel *pointLabel = new QLabel(QString("Point %1:").arg(pointIndex + 1));

    QDoubleSpinBox *xSpin = new QDoubleSpinBox();
    xSpin->setRange(kMinPathSpin, kMaxPathSpin);
    xSpin->setValue(0);

    QDoubleSpinBox *ySpin = new QDoubleSpinBox();
    ySpin->setRange(kMinPathSpin, kMaxPathSpin);
    ySpin->setValue(0);

    QPushButton *deleteButton = new QPushButton();
    deleteButton->setText("X");
    deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QFontMetrics fm(deleteButton->font());
    int width = fm.horizontalAdvance(deleteButton->text()) + kDeleteButtonWidthPadding;
    deleteButton->setFixedSize(width, deleteButton->sizeHint().height());

    QHBoxLayout *pointLayout = new QHBoxLayout();
    pointLayout->addWidget(xSpin);
    pointLayout->addWidget(ySpin);
    pointLayout->addWidget(deleteButton);

    pathLayout->addRow(pointLabel, pointLayout);

    m_pathPointSpins.append(xSpin);
    m_pathPointSpins.append(ySpin);

    connect(xSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, pointIndex](double value) { onPathPointChanged(pointIndex, value, true); });
    connect(ySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, pointIndex](double value) { onPathPointChanged(pointIndex, value, false); });
    connect(deleteButton, &QPushButton::clicked, this,
            [this, pointIndex]() { onDeletePathPointClicked(pointIndex); });
}

// Signal handlers implementation
void MobInspector::onNameChanged(const QString &text)
{
    if (m_currentMob) {
        m_currentMob->set_name(text.toStdString());
    }
}

void MobInspector::onMobPrefabIdChanged(int value)
{
    m_undoStack->push(new IntUndoCommand(
            value, [this]() { return m_currentMob->mob_prefab_id(); },
            [this](int value) { m_currentMob->set_mob_prefab_id(value); }, "MobPrefabId",
            [this]() { m_mobPrefabIdSpin->setValue(m_currentMob->mob_prefab_id()); }));
}

void MobInspector::onSpawnDelayChanged(double value)
{
    if (m_currentMob) {
        m_currentMob->set_spawn_delay(static_cast<float>(value));
    }
}

void MobInspector::onPathPointChanged(int pointIndex, double value, bool isX)
{
    if (!m_currentMob || !m_currentMob->has_path()
        || pointIndex >= m_currentMob->path().key_points_size()) {
        return;
    }

    auto *point = m_currentMob->mutable_path()->mutable_key_points(pointIndex);
    if (isX) {
        point->set_x(static_cast<float>(value));
    } else {
        point->set_y(static_cast<float>(value));
    }
}

void MobInspector::onTravelTimeChanged(double value)
{
    if (m_currentMob) {
        if (!m_currentMob->has_path()) {
            m_currentMob->mutable_path();
        }
        m_currentMob->mutable_path()->set_travel_time(static_cast<float>(value));
    }
}

void MobInspector::onDropItemChanged(const QString &text)
{
    if (m_currentMob) {
        if (!m_currentMob->has_drop_item()) {
            m_currentMob->mutable_drop_item();
        }
        m_currentMob->mutable_drop_item()->set_item(text.toStdString());
    }
}

void MobInspector::onDropWeightChanged(double value)
{
    if (m_currentMob) {
        if (!m_currentMob->has_drop_item()) {
            m_currentMob->mutable_drop_item();
        }
        m_currentMob->mutable_drop_item()->set_weight(value);
    }
}

void MobInspector::onFixedAmountChanged(int state)
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

    if (!m_currentMob) {
        return;
    }

    if (!m_currentMob->has_drop_item()) {
        m_currentMob->mutable_drop_item();
    }

    m_currentMob->mutable_drop_item()->set_fixed_amount(fixed);
}

void MobInspector::onFixedAmountValueChanged(int value)
{
    if (!m_currentMob) {
        return;
    }

    if (!m_currentMob->has_drop_item()) {
        m_currentMob->mutable_drop_item();
    }

    m_currentMob->mutable_drop_item()->set_fixed(value);
}

void MobInspector::onMinAmountChanged(int value)
{
    if (!m_currentMob) {
        return;
    }

    if (!m_currentMob->has_drop_item() || !m_currentMob->drop_item().has_range()) {
        m_currentMob->mutable_drop_item()->mutable_range();
    }

    m_currentMob->mutable_drop_item()->mutable_range()->set_min(value);
}

void MobInspector::onMaxAmountChanged(int value)
{
    if (!m_currentMob) {
        return;
    }

    if (!m_currentMob->has_drop_item() || !m_currentMob->drop_item().has_range()) {
        m_currentMob->mutable_drop_item()->mutable_range();
    }

    m_currentMob->mutable_drop_item()->mutable_range()->set_max(value);
}

void MobInspector::onAddPathPointClicked()
{
    addPathPointRow(static_cast<int>(m_pathPointSpins.count()) / 2);

    if (m_currentMob) {
        auto *point = m_currentMob->mutable_path()->add_key_points();
        point->set_x(0);
        point->set_y(0);
    }
}

void MobInspector::onDeletePathPointClicked(int pointIndex)
{
    if (!m_currentMob || !m_currentMob->has_path()
        || pointIndex >= m_currentMob->path().key_points_size()) {
        return;
    }

    auto *keyPoints = m_currentMob->mutable_path()->mutable_key_points();
    keyPoints->DeleteSubrange(pointIndex, 1);
    updatePathFields();
}
