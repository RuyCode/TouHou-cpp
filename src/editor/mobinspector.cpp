#include "mobinspector.h"

namespace {
const QString kPathGroupTitle = "Path";
const QString kDropGroupTitle = "Drop";
const QString kColliderGroupTitle = "Collider";

const int kMinWidth = 300;
const double kMaxHealth = 999999;
const double kMaxValue = 999;
const double kMinPositive = 0.1;
const double kMinNegative = -999;
const double kMaxPathSpin = 9999;
const double kMinPathSpin = -9999;

const double kSingleStep = 0.1;
const double kDeaultColliderSpin = 10.0;
const int kPathGroupOffset = 2;  // travel time and add button

const int kDeleteButtonWidthPadding = 10;

const int kCircleRadiusIndex = 3;
const int kRectWidthIndex = 4;
const int kRectHeightIndex = 5;

// TODO : is it okay?
enum class ColliderType {
    Circle,
    Rectangle,
};
}  // namespace

MobInspector::MobInspector(QWidget* parent)
    : QDockWidget("Mob Inspector", parent),
      mobIdEdit(new QLineEdit()),
      healthSpin(new QDoubleSpinBox()),
      spriteEdit(new QLineEdit()),
      spawnDelaySpin(new QDoubleSpinBox()),
      attackIdEdit(new QLineEdit()),
      pathGroup(new QGroupBox(kPathGroupTitle)),
      travelTimeSpin(new QDoubleSpinBox()),
      addPathPointButton(new QPushButton()),
      dropGroup(new QGroupBox(kDropGroupTitle)),
      dropItemEdit(new QLineEdit()),
      dropWeightSpin(new QDoubleSpinBox()),
      colliderGroup(new QGroupBox(kColliderGroupTitle)),
      colliderOffsetXSpin(new QDoubleSpinBox()),
      colliderOffsetYSpin(new QDoubleSpinBox()),
      colliderTypeCombo(new QComboBox()),
      circleRadiusSpin(new QDoubleSpinBox()),
      rectWidthSpin(new QDoubleSpinBox()),
      rectHeightSpin(new QDoubleSpinBox()) {
    setupUI();
    setMinimumWidth(kMinWidth);

    colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
    colliderTypeCombo->currentIndexChanged(static_cast<int>(ColliderType::Circle));
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
    layout->addRow("Mob ID:", mobIdEdit);

    healthSpin->setRange(0, kMaxHealth);
    healthSpin->setSingleStep(kSingleStep);
    layout->addRow("Health:", healthSpin);

    layout->addRow("Sprite:", spriteEdit);

    spawnDelaySpin->setRange(0, kMaxValue);
    spawnDelaySpin->setSingleStep(kSingleStep);
    layout->addRow("Spawn Delay:", spawnDelaySpin);

    layout->addRow("Attack ID:", attackIdEdit);

    // Path section
    QFormLayout* pathLayout = new QFormLayout(pathGroup);

    travelTimeSpin->setRange(0, kMaxValue);
    travelTimeSpin->setSingleStep(kSingleStep);
    pathLayout->addRow("Travel Time:", travelTimeSpin);

    addPathPointButton->setText("Add key point");
    pathLayout->addRow(addPathPointButton);

    // Points will be added dynamically
    pathGroup->setLayout(pathLayout);
    layout->addRow(pathGroup);

    // Drop section
    QFormLayout* dropLayout = new QFormLayout(dropGroup);

    dropLayout->addRow("Item:", dropItemEdit);

    dropWeightSpin->setRange(kMinPositive, kMaxValue);
    dropWeightSpin->setSingleStep(kSingleStep);
    dropLayout->addRow("Weight:", dropWeightSpin);

    dropGroup->setLayout(dropLayout);
    layout->addRow(dropGroup);

    // Collider section
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

    colliderGroup->setLayout(colliderLayout);
    layout->addRow(colliderGroup);

    // Connect signals
    connect(mobIdEdit, &QLineEdit::textChanged, this, &MobInspector::OnMobIdChanged);
    connect(healthSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnHealthChanged);
    connect(spriteEdit, &QLineEdit::textChanged, this, &MobInspector::OnSpriteChanged);
    connect(spawnDelaySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnSpawnDelayChanged);
    connect(attackIdEdit, &QLineEdit::textChanged, this, &MobInspector::OnAttackIdChanged);
    connect(travelTimeSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnTravelTimeChanged);
    connect(addPathPointButton, &QPushButton::clicked, this, &MobInspector::OnAddPathPointClicked);
    connect(dropItemEdit, &QLineEdit::textChanged, this, &MobInspector::OnDropItemChanged);
    connect(dropWeightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnDropWeightChanged);
    connect(colliderOffsetXSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnColliderOffsetXChanged);
    connect(colliderOffsetYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnColliderOffsetYChanged);
    connect(colliderTypeCombo, &QComboBox::currentIndexChanged, this, &MobInspector::OnColliderTypeChanged);
    connect(circleRadiusSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnCircleRadiusChanged);
    connect(rectWidthSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnRectWidthChanged);
    connect(rectHeightSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::OnRectHeightChanged);

    setWidget(content);
}

void MobInspector::UpdateFields() {
    if (!currentMob) {
        return;
    }

    mobIdEdit->setText(QString::fromStdString(currentMob->mob_id()));
    healthSpin->setValue(currentMob->health());
    spriteEdit->setText(QString::fromStdString(currentMob->sprite()));
    spawnDelaySpin->setValue(currentMob->spawn_delay());
    attackIdEdit->setText(QString::fromStdString(currentMob->attack_id()));

    // Update path
    UpdatePathFields();

    // Update drop
    if (currentMob->has_drop()) {
        dropItemEdit->setText(QString::fromStdString(currentMob->drop().item()));
        dropWeightSpin->setValue(currentMob->drop().drop_weight());
    } else {
        dropItemEdit->clear();
        dropWeightSpin->setValue(1.0);
    }

    // Update collider
    UpdateColliderFields();
}

void MobInspector::UpdatePathFields() {
    if (!currentMob || !currentMob->has_path()) {
        // Clear path points
        qDeleteAll(pathPointSpins);
        pathPointSpins.clear();
        travelTimeSpin->setValue(0);
        return;
    }

    const auto& path = currentMob->path();
    travelTimeSpin->setValue(path.travel_time());

    // Clear existing point widgets
    qDeleteAll(pathPointSpins);
    pathPointSpins.clear();

    // Get the path group layout
    QFormLayout* pathLayout = qobject_cast<QFormLayout*>(pathGroup->layout());

    // Remove all rows except the first one (travel time)
    while (pathLayout->rowCount() > kPathGroupOffset) {
        pathLayout->removeRow(kPathGroupOffset);
    }

    // Add points
    for (int i = 0; i < path.key_points_size(); ++i) {
        const auto& point = path.key_points(i);

        AddPathPointRow(i);
        pathPointSpins[i * 2]->setValue(point.x());
        pathPointSpins[i * 2 + 1]->setValue(point.y());
    }
}

void MobInspector::UpdateColliderFields() {
    if (!currentMob || !currentMob->has_collider()) {
        colliderOffsetXSpin->setValue(0);
        colliderOffsetYSpin->setValue(0);
        colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
        circleRadiusSpin->setValue(kDeaultColliderSpin);
        rectWidthSpin->setValue(kDeaultColliderSpin);
        rectHeightSpin->setValue(kDeaultColliderSpin);
        return;
    }

    const auto& collider = currentMob->collider();
    colliderOffsetXSpin->setValue(collider.offset_x());
    colliderOffsetYSpin->setValue(collider.offset_y());

    QFormLayout* colliderLayout = qobject_cast<QFormLayout*>(colliderGroup->layout());

    if (collider.has_circle_collider()) {
        colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Circle));
        circleRadiusSpin->setValue(collider.circle_collider().radius());
        colliderLayout->setRowVisible(kCircleRadiusIndex, true);
        colliderLayout->setRowVisible(kRectWidthIndex, false);
        colliderLayout->setRowVisible(kRectHeightIndex, false);
    } else if (collider.has_rectangle_collider()) {
        colliderTypeCombo->setCurrentIndex(static_cast<int>(ColliderType::Rectangle));
        rectWidthSpin->setValue(collider.rectangle_collider().width());
        rectHeightSpin->setValue(collider.rectangle_collider().height());
        colliderLayout->setRowVisible(kCircleRadiusIndex, false);
        colliderLayout->setRowVisible(kRectWidthIndex, true);
        colliderLayout->setRowVisible(kRectHeightIndex, true);
    }
}

void MobInspector::ClearFields() {
    mobIdEdit->clear();
    healthSpin->setValue(0);
    spriteEdit->clear();
    spawnDelaySpin->setValue(0);
    attackIdEdit->clear();

    // Clear path
    qDeleteAll(pathPointSpins);
    pathPointSpins.clear();
    travelTimeSpin->setValue(0);

    // Clear drop
    dropItemEdit->clear();
    dropWeightSpin->setValue(1.0);

    // Clear collider
    colliderOffsetXSpin->setValue(0);
    colliderOffsetYSpin->setValue(0);
    colliderTypeCombo->setCurrentIndex(0);
    circleRadiusSpin->setValue(kDeaultColliderSpin);
    rectWidthSpin->setValue(kDeaultColliderSpin);
    rectHeightSpin->setValue(kDeaultColliderSpin);
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

    // Store references to the spin boxes
    pathPointSpins.append(xSpin);
    pathPointSpins.append(ySpin);

    // Connect signals
    connect(xSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, pointIndex](double value) { OnPathPointChanged(pointIndex, value, true); });
    connect(ySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, pointIndex](double value) { OnPathPointChanged(pointIndex, value, false); });
    connect(deleteButton, &QPushButton::clicked, this, [this, pointIndex]() { OnDeletePathPointClicked(pointIndex); });
}

void MobInspector::OnMobIdChanged(const QString& text) {
    if (!currentMob) {
        return;
    }

    currentMob->set_mob_id(text.toStdString());
}

void MobInspector::OnHealthChanged(double value) {
    if (!currentMob) {
        return;
    }

    currentMob->set_health(static_cast<float>(value));
}

void MobInspector::OnSpriteChanged(const QString& text) {
    if (!currentMob) {
        return;
    }

    currentMob->set_sprite(text.toStdString());
}

void MobInspector::OnSpawnDelayChanged(double value) {
    if (!currentMob) {
        return;
    }

    currentMob->set_spawn_delay(static_cast<float>(value));
}

void MobInspector::OnAttackIdChanged(const QString& text) {
    if (!currentMob) {
        return;
    }

    currentMob->set_attack_id(text.toStdString());
}

void MobInspector::OnPathPointChanged(int pointIndex, double value, bool isX) {
    if (!currentMob || !currentMob->has_path()) {
        return;
    }

    auto* path = currentMob->mutable_path();
    if (pointIndex >= path->key_points_size()) {
        return;
    }

    auto* point = path->mutable_key_points(pointIndex);
    if (isX) {
        point->set_x(static_cast<float>(value));
    } else {
        point->set_y(static_cast<float>(value));
    }
}

void MobInspector::OnTravelTimeChanged(double value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_path()) {
        currentMob->mutable_path();
    }

    currentMob->mutable_path()->set_travel_time(static_cast<float>(value));
}

void MobInspector::OnDropItemChanged(const QString& text) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_drop()) {
        currentMob->mutable_drop();
    }

    currentMob->mutable_drop()->set_item(text.toStdString());
}

void MobInspector::OnDropWeightChanged(double value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_drop()) {
        currentMob->mutable_drop();
    }

    currentMob->mutable_drop()->set_drop_weight(static_cast<float>(value));
}

void MobInspector::OnColliderOffsetXChanged(double value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_collider()) {
        currentMob->mutable_collider();
    }

    currentMob->mutable_collider()->set_offset_x(static_cast<float>(value));
}

void MobInspector::OnColliderOffsetYChanged(double value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_collider()) {
        currentMob->mutable_collider();
    }

    currentMob->mutable_collider()->set_offset_y(static_cast<float>(value));
}

void MobInspector::OnColliderTypeChanged(int index) {
    QFormLayout* colliderLayout = qobject_cast<QFormLayout*>(colliderGroup->layout());
    switch (static_cast<ColliderType>(index)) {
        case ColliderType::Circle: {
            colliderLayout->setRowVisible(kCircleRadiusIndex, true);
            colliderLayout->setRowVisible(kRectWidthIndex, false);
            colliderLayout->setRowVisible(kRectHeightIndex, false);
            break;
        }
        case ColliderType::Rectangle: {
            colliderLayout->setRowVisible(kCircleRadiusIndex, false);
            colliderLayout->setRowVisible(kRectWidthIndex, true);
            colliderLayout->setRowVisible(kRectHeightIndex, true);
            break;
        }
    }

    if (!currentMob) {
        return;
    }

    if (!currentMob->has_collider()) {
        currentMob->mutable_collider();
    }

    switch (static_cast<ColliderType>(index)) {
        case ColliderType::Circle: {
            if (!currentMob->mutable_collider()->has_circle_collider()) {
                currentMob->mutable_collider()->set_allocated_circle_collider(new game::CircleCollider());
            }
            break;
        }
        case ColliderType::Rectangle: {
            if (!currentMob->mutable_collider()->has_rectangle_collider()) {
                currentMob->mutable_collider()->set_allocated_rectangle_collider(new game::RectangleCollider());
            }
            break;
        }
    }
}

void MobInspector::OnCircleRadiusChanged(double value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_collider()) {
        currentMob->mutable_collider();
    }

    if (!currentMob->mutable_collider()->has_circle_collider()) {
        currentMob->mutable_collider()->set_allocated_circle_collider(new game::CircleCollider());
    }

    currentMob->mutable_collider()->mutable_circle_collider()->set_radius(static_cast<float>(value));
}

void MobInspector::OnRectWidthChanged(double value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_collider()) {
        currentMob->mutable_collider();
    }

    if (!currentMob->mutable_collider()->has_rectangle_collider()) {
        currentMob->mutable_collider()->set_allocated_rectangle_collider(new game::RectangleCollider());
    }

    currentMob->mutable_collider()->mutable_rectangle_collider()->set_width(static_cast<float>(value));
}

void MobInspector::OnRectHeightChanged(double value) {
    if (!currentMob) {
        return;
    }

    if (!currentMob->has_collider()) {
        currentMob->mutable_collider();
    }

    if (!currentMob->mutable_collider()->has_rectangle_collider()) {
        currentMob->mutable_collider()->set_allocated_rectangle_collider(new game::RectangleCollider());
    }

    currentMob->mutable_collider()->mutable_rectangle_collider()->set_height(static_cast<float>(value));
}

void MobInspector::OnAddPathPointClicked() {
    AddPathPointRow(static_cast<int>(pathPointSpins.count()) / 2);

    if (!currentMob) {
        return;
    }

    auto* path = currentMob->mutable_path();

    auto* point = path->add_key_points();
    point->set_x(0);
    point->set_y(0);
}

void MobInspector::OnDeletePathPointClicked(int pointIndex) {
    if (!currentMob || !currentMob->has_path()) {
        return;
    }

    int size = currentMob->mutable_path()->key_points_size();
    if (pointIndex >= size) {
        return;
    }

    auto* keyPoints = currentMob->mutable_path()->mutable_key_points();
    for (int i = pointIndex; i + 1 < size; ++i) {
        keyPoints->SwapElements(i, i + 1);
    }
    keyPoints->RemoveLast();

    UpdatePathFields();
}
