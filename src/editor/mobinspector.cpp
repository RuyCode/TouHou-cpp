#include "mobinspector.h"

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
const int kPathGroupOffset = 2;  // travel time and add button

const int kDeleteButtonWidthPadding = 10;

const int kCircleRadiusIndex = 3;
const int kRectWidthIndex = 4;
const int kRectHeightIndex = 5;
const int kRectAngleIndex = 6;

enum class ColliderType {
    Circle,
    Rectangle,
};
}  // namespace

MobInspector::MobInspector(QWidget* parent)
    : QDockWidget("Mob Inspector", parent),
      basicGroup(new QGroupBox(kBasicGroupTitle)),
      nameEdit(new QLineEdit()),
      mobPrefabIdSpin(new QSpinBox()),
      spawnDelaySpin(new QDoubleSpinBox()),
      basicPrefabGroup(new QGroupBox(kBasicGroupReadOnlyTitle)),
      healthSpin(new QDoubleSpinBox()),
      spriteEdit(new QLineEdit()),
      attackIdSpin(new QSpinBox()),
      pathGroup(new QGroupBox(kPathGroupTitle)),
      travelTimeSpin(new QDoubleSpinBox()),
      addPathPointButton(new QPushButton()),
      dropGroup(new QGroupBox(kDropGroupTitle)),
      dropItemEdit(new QLineEdit()),
      dropWeightSpin(new QDoubleSpinBox()),
      fixedAmountCheck(new QCheckBox("Fixed amount")),
      fixedAmountSpin(new QSpinBox()),
      minAmountSpin(new QSpinBox()),
      maxAmountSpin(new QSpinBox()),
      colliderGroup(new QGroupBox(kColliderGroupTitle)),
      colliderOffsetXSpin(new QDoubleSpinBox()),
      colliderOffsetYSpin(new QDoubleSpinBox()),
      colliderTypeCombo(new QComboBox()),
      circleRadiusSpin(new QDoubleSpinBox()),
      rectWidthSpin(new QDoubleSpinBox()),
      rectHeightSpin(new QDoubleSpinBox()),
      rectAngleSpin(new QDoubleSpinBox()),
      currentMob(nullptr) {
    setupUI();
    setMinimumWidth(kMinWidth);

    // Make collider fields read-only since they come from prefab
    colliderOffsetXSpin->setReadOnly(true);
    colliderOffsetYSpin->setReadOnly(true);
    colliderTypeCombo->setEnabled(false);
    circleRadiusSpin->setReadOnly(true);
    rectWidthSpin->setReadOnly(true);
    rectHeightSpin->setReadOnly(true);
    rectAngleSpin->setReadOnly(true);

    healthSpin->setReadOnly(true);
    spriteEdit->setReadOnly(true);
    attackIdSpin->setReadOnly(true);

    colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
}

void MobInspector::SetMob(game::Mob* mob) {
    currentMob = mob;
    UpdateFields();
}

void MobInspector::Clear() {
    currentMob = nullptr;
    ClearFields();
}

void MobInspector::setupUI() {
    QWidget* content = new QWidget(this);
    QFormLayout* layout = new QFormLayout(content);

    // Basic fields
    QFormLayout* basicLayout = new QFormLayout(basicGroup);
    basicLayout->addRow("Name:", nameEdit);

    mobPrefabIdSpin->setRange(0, INT_MAX);
    basicLayout->addRow("Mob Prefab ID:", mobPrefabIdSpin);

    healthSpin->setRange(0, kMaxHealth);
    healthSpin->setSingleStep(kSingleStep);

    spawnDelaySpin->setRange(0, kMaxValue);
    spawnDelaySpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Spawn Delay:", spawnDelaySpin);

    QFormLayout* basicPrefabLayout = new QFormLayout(basicPrefabGroup);
    basicPrefabLayout->addRow("Health:", healthSpin);

    basicPrefabLayout->addRow("Sprite:", spriteEdit);

    attackIdSpin->setRange(0, INT_MAX);
    basicPrefabLayout->addRow("Attack ID:", attackIdSpin);

    basicPrefabGroup->setLayout(basicPrefabLayout);
    basicLayout->addRow(basicPrefabGroup);

    basicGroup->setLayout(basicLayout);
    layout->addRow(basicGroup);

    // Path section
    QFormLayout* pathLayout = new QFormLayout(pathGroup);

    travelTimeSpin->setRange(0, kMaxValue);
    travelTimeSpin->setSingleStep(kSingleStep);
    pathLayout->addRow("Travel Time:", travelTimeSpin);

    addPathPointButton->setText("Add key point");
    pathLayout->addRow(addPathPointButton);

    pathGroup->setLayout(pathLayout);
    layout->addRow(pathGroup);

    // Drop section
    QFormLayout* dropLayout = new QFormLayout(dropGroup);

    dropLayout->addRow("Item:", dropItemEdit);

    dropWeightSpin->setRange(kMinPositive, kMaxValue);
    dropWeightSpin->setSingleStep(kSingleStep);
    dropLayout->addRow("Weight:", dropWeightSpin);

    fixedAmountCheck->setChecked(true);
    dropLayout->addRow(fixedAmountCheck);

    fixedAmountSpin->setRange(1, INT_MAX);
    dropLayout->addRow("Fixed amount:", fixedAmountSpin);

    minAmountSpin->setRange(1, INT_MAX);
    maxAmountSpin->setRange(1, INT_MAX);
    QHBoxLayout* rangeLayout = new QHBoxLayout();
    rangeLayout->addWidget(new QLabel("Min:"));
    rangeLayout->addWidget(minAmountSpin);
    rangeLayout->addWidget(new QLabel("Max:"));
    rangeLayout->addWidget(maxAmountSpin);
    dropLayout->addRow("Range amount:", rangeLayout);

    dropGroup->setLayout(dropLayout);
    layout->addRow(dropGroup);

    // Collider section (read-only, from prefab)
    QFormLayout* colliderLayout = new QFormLayout(colliderGroup);

    colliderOffsetXSpin->setRange(kMinNegative, kMaxValue);
    colliderLayout->addRow("Offset X:", colliderOffsetXSpin);

    colliderOffsetYSpin->setRange(kMinNegative, kMaxValue);
    colliderLayout->addRow("Offset Y:", colliderOffsetYSpin);

    colliderTypeCombo->addItem("Circle");
    colliderTypeCombo->addItem("Rectangle");
    colliderLayout->addRow("Type:", colliderTypeCombo);

    circleRadiusSpin->setRange(kMinPositive, kMaxValue);
    circleRadiusSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Radius:", circleRadiusSpin);

    rectWidthSpin->setRange(kMinPositive, kMaxValue);
    rectWidthSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Width:", rectWidthSpin);

    rectHeightSpin->setRange(kMinPositive, kMaxValue);
    rectHeightSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Height:", rectHeightSpin);

    rectAngleSpin->setRange(kMinNegative, kMaxValue);
    rectAngleSpin->setSingleStep(kSingleStep);
    colliderLayout->addRow("Angle:", rectAngleSpin);

    colliderGroup->setLayout(colliderLayout);
    layout->addRow(colliderGroup);

    // Connect signals
    connect(nameEdit, &QLineEdit::textChanged, this, &MobInspector::OnNameChanged);
    connect(mobPrefabIdSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MobInspector::OnMobPrefabIdChanged);
    connect(spawnDelaySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnSpawnDelayChanged);
    connect(travelTimeSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnTravelTimeChanged);
    connect(addPathPointButton, &QPushButton::clicked, this, &MobInspector::OnAddPathPointClicked);
    connect(dropItemEdit, &QLineEdit::textChanged, this, &MobInspector::OnDropItemChanged);
    connect(dropWeightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnDropWeightChanged);
    connect(fixedAmountCheck, &QCheckBox::stateChanged, this, &MobInspector::OnFixedAmountChanged);
    connect(fixedAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MobInspector::OnFixedAmountValueChanged);
    connect(minAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MobInspector::OnMinAmountChanged);
    connect(maxAmountSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MobInspector::OnMaxAmountChanged);

    setWidget(content);
}

void MobInspector::UpdateFields() {
    if (!currentMob) {
        return;
    }

    // Get prefab
    game::Level& level = DataManager::level;
    if (currentMob->mob_prefab_id() >= level.mob_prefabs_size()) {
        return;
    }

    const auto& prefab = level.mob_prefabs(currentMob->mob_prefab_id());

    nameEdit->setText(QString::fromStdString(currentMob->name()));
    mobPrefabIdSpin->setValue(currentMob->mob_prefab_id());
    healthSpin->setValue(prefab.health());
    spriteEdit->setText(QString::fromStdString(prefab.sprite()));
    spawnDelaySpin->setValue(currentMob->spawn_delay());
    attackIdSpin->setValue(prefab.attack_id());

    // Update path
    UpdatePathFields();

    // Update drop
    if (currentMob->has_drop_item()) {
        const auto& drop = currentMob->drop_item();
        dropItemEdit->setText(QString::fromStdString(drop.item()));
        dropWeightSpin->setValue(drop.weight());

        if (drop.fixed_amount()) {
            fixedAmountCheck->setChecked(true);
            if (drop.has_fixed()) {
                fixedAmountSpin->setValue(drop.fixed());
            }
        } else {
            fixedAmountCheck->setChecked(false);
            if (drop.has_range()) {
                minAmountSpin->setValue(drop.range().min());
                maxAmountSpin->setValue(drop.range().max());
            }
        }
    } else {
        dropItemEdit->clear();
        dropWeightSpin->setValue(1.0);
        fixedAmountCheck->setChecked(true);
        fixedAmountSpin->setValue(1);
    }

    // Update collider from prefab
    UpdateColliderFields();
}

void MobInspector::UpdatePathFields() {
    if (!currentMob || !currentMob->has_path()) {
        qDeleteAll(pathPointSpins);
        pathPointSpins.clear();
        travelTimeSpin->setValue(0);
        return;
    }

    const auto& path = currentMob->path();
    travelTimeSpin->setValue(path.travel_time());

    qDeleteAll(pathPointSpins);
    pathPointSpins.clear();

    QFormLayout* pathLayout = qobject_cast<QFormLayout*>(pathGroup->layout());
    while (pathLayout->rowCount() > kPathGroupOffset) {
        pathLayout->removeRow(kPathGroupOffset);
    }

    for (int i = 0; i < path.key_points_size(); ++i) {
        const auto& point = path.key_points(i);

        AddPathPointRow(i);
        pathPointSpins[i * 2]->setValue(point.x());
        pathPointSpins[i * 2 + 1]->setValue(point.y());
    }
}

void MobInspector::UpdateColliderFields() {
    if (!currentMob) {
        colliderOffsetXSpin->setValue(0);
        colliderOffsetYSpin->setValue(0);
        colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
        circleRadiusSpin->setValue(0);
        rectWidthSpin->setValue(0);
        rectHeightSpin->setValue(0);
        rectAngleSpin->setValue(0);
        return;
    }

    // Get collider from prefab
    game::Level& level = DataManager::level;
    if (currentMob->mob_prefab_id() >= level.mob_prefabs_size()) {
        return;
    }

    const auto& prefab = level.mob_prefabs(currentMob->mob_prefab_id());
    if (!prefab.has_collider()) {
        return;
    }

    const auto& collider = prefab.collider();
    colliderOffsetXSpin->setValue(collider.offset().x());
    colliderOffsetYSpin->setValue(collider.offset().y());

    QFormLayout* colliderLayout = qobject_cast<QFormLayout*>(colliderGroup->layout());

    if (collider.has_circle()) {
        colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
        circleRadiusSpin->setValue(collider.circle().radius());
        colliderLayout->setRowVisible(kCircleRadiusIndex, true);
        colliderLayout->setRowVisible(kRectWidthIndex, false);
        colliderLayout->setRowVisible(kRectHeightIndex, false);
        colliderLayout->setRowVisible(kRectAngleIndex, false);
    } else if (collider.has_rectangle()) {
        colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Rectangle));
        rectWidthSpin->setValue(collider.rectangle().width());
        rectHeightSpin->setValue(collider.rectangle().height());
        rectAngleSpin->setValue(collider.rectangle().angle());
        colliderLayout->setRowVisible(kCircleRadiusIndex, false);
        colliderLayout->setRowVisible(kRectWidthIndex, true);
        colliderLayout->setRowVisible(kRectHeightIndex, true);
        colliderLayout->setRowVisible(kRectAngleIndex, true);
    }
}

void MobInspector::ClearFields() {
    nameEdit->clear();
    mobPrefabIdSpin->setValue(0);
    healthSpin->setValue(0);
    spriteEdit->clear();
    spawnDelaySpin->setValue(0);
    attackIdSpin->setValue(0);

    qDeleteAll(pathPointSpins);
    pathPointSpins.clear();
    travelTimeSpin->setValue(0);

    dropItemEdit->clear();
    dropWeightSpin->setValue(1.0);
    fixedAmountCheck->setChecked(true);
    fixedAmountSpin->setValue(1);
    minAmountSpin->setValue(1);
    maxAmountSpin->setValue(1);

    colliderOffsetXSpin->setValue(0);
    colliderOffsetYSpin->setValue(0);
    colliderTypeCombo->setCurrentIndex(0);
    circleRadiusSpin->setValue(kDefaultColliderSpin);
    rectWidthSpin->setValue(kDefaultColliderSpin);
    rectHeightSpin->setValue(kDefaultColliderSpin);
    rectAngleSpin->setValue(0);
}

void MobInspector::AddPathPointRow(int pointIndex) {
    QFormLayout* pathLayout = qobject_cast<QFormLayout*>(pathGroup->layout());

    QLabel* pointLabel = new QLabel(QString("Point %1:").arg(pointIndex + 1));

    QDoubleSpinBox* xSpin = new QDoubleSpinBox();
    xSpin->setRange(kMinPathSpin, kMaxPathSpin);
    xSpin->setValue(0);

    QDoubleSpinBox* ySpin = new QDoubleSpinBox();
    ySpin->setRange(kMinPathSpin, kMaxPathSpin);
    ySpin->setValue(0);

    QPushButton* deleteButton = new QPushButton();
    deleteButton->setText("X");
    deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QFontMetrics fm(deleteButton->font());
    int width = fm.horizontalAdvance(deleteButton->text()) + kDeleteButtonWidthPadding;
    deleteButton->setFixedSize(width, deleteButton->sizeHint().height());

    QHBoxLayout* pointLayout = new QHBoxLayout();
    pointLayout->addWidget(xSpin);
    pointLayout->addWidget(ySpin);
    pointLayout->addWidget(deleteButton);

    pathLayout->addRow(pointLabel, pointLayout);

    pathPointSpins.append(xSpin);
    pathPointSpins.append(ySpin);

    connect(xSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, pointIndex](double value) { OnPathPointChanged(pointIndex, value, true); });
    connect(ySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, pointIndex](double value) { OnPathPointChanged(pointIndex, value, false); });
    connect(deleteButton, &QPushButton::clicked, this, [this, pointIndex]() { OnDeletePathPointClicked(pointIndex); });
}

// Signal handlers implementation
void MobInspector::OnNameChanged(const QString& text) {
    if (currentMob) {
        currentMob->set_name(text.toStdString());
    }
}

void MobInspector::OnMobPrefabIdChanged(int value) {
    if (currentMob) {
        currentMob->set_mob_prefab_id(value);
    }
}

void MobInspector::OnSpawnDelayChanged(double value) {
    if (currentMob) {
        currentMob->set_spawn_delay(static_cast<float>(value));
    }
}

void MobInspector::OnPathPointChanged(int pointIndex, double value, bool isX) {
    if (!currentMob || !currentMob->has_path() || pointIndex >= currentMob->path().key_points_size()) {
        return;
    }

    auto* point = currentMob->mutable_path()->mutable_key_points(pointIndex);
    if (isX) {
        point->set_x(static_cast<float>(value));
    } else {
        point->set_y(static_cast<float>(value));
    }
}

void MobInspector::OnTravelTimeChanged(double value) {
    if (currentMob) {
        if (!currentMob->has_path()) {
            currentMob->mutable_path();
        }
        currentMob->mutable_path()->set_travel_time(static_cast<float>(value));
    }
}

void MobInspector::OnDropItemChanged(const QString& text) {
    if (currentMob) {
        if (!currentMob->has_drop_item()) {
            currentMob->mutable_drop_item();
        }
        currentMob->mutable_drop_item()->set_item(text.toStdString());
    }
}

void MobInspector::OnDropWeightChanged(double value) {
    if (currentMob) {
        if (!currentMob->has_drop_item()) {
            currentMob->mutable_drop_item();
        }
        currentMob->mutable_drop_item()->set_weight(value);
    }
}

void MobInspector::OnFixedAmountChanged(int state) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_drop_item()) {
        currentMob->mutable_drop_item();
    }

    currentMob->mutable_drop_item()->set_fixed_amount(state == Qt::Checked);
}

void MobInspector::OnFixedAmountValueChanged(int value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_drop_item()) {
        currentMob->mutable_drop_item();
    }

    currentMob->mutable_drop_item()->set_fixed(value);
}

void MobInspector::OnMinAmountChanged(int value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_drop_item() || !currentMob->drop_item().has_range()) {
        currentMob->mutable_drop_item()->mutable_range();
    }

    currentMob->mutable_drop_item()->mutable_range()->set_min(value);
}

void MobInspector::OnMaxAmountChanged(int value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_drop_item() || !currentMob->drop_item().has_range()) {
        currentMob->mutable_drop_item()->mutable_range();
    }

    currentMob->mutable_drop_item()->mutable_range()->set_max(value);
}

void MobInspector::OnAddPathPointClicked() {
    AddPathPointRow(static_cast<int>(pathPointSpins.count()) / 2);

    if (currentMob) {
        auto* point = currentMob->mutable_path()->add_key_points();
        point->set_x(0);
        point->set_y(0);
    }
}

void MobInspector::OnDeletePathPointClicked(int pointIndex) {
    if (!currentMob || !currentMob->has_path() || pointIndex >= currentMob->path().key_points_size()) {
        return;
    }

    auto* keyPoints = currentMob->mutable_path()->mutable_key_points();
    keyPoints->DeleteSubrange(pointIndex, 1);
    UpdatePathFields();
}
