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
    MobInspector(QWidget* parent = nullptr) : QDockWidget("Mob Inspector", parent) {
        setupUI();
        setMinimumWidth(300);
    }

    void setMob(game::Mob* mob) {
        currentMob = mob;
        updateFields();
    }

    void clear() {
        currentMob = nullptr;
        clearFields();
    }

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

    void setupUI() {
        QWidget* content = new QWidget(this);
        QFormLayout* layout = new QFormLayout(content);

        // Basic fields
        mobIdEdit = new QLineEdit();
        layout->addRow("Mob ID:", mobIdEdit);

        healthSpin = new QDoubleSpinBox();
        healthSpin->setRange(0, 999999);
        healthSpin->setSingleStep(0.1);
        layout->addRow("Health:", healthSpin);

        spriteEdit = new QLineEdit();
        layout->addRow("Sprite:", spriteEdit);

        spawnDelaySpin = new QDoubleSpinBox();
        spawnDelaySpin->setRange(0, 999);
        spawnDelaySpin->setSingleStep(0.1);
        layout->addRow("Spawn Delay:", spawnDelaySpin);

        attackIdEdit = new QLineEdit();
        layout->addRow("Attack ID:", attackIdEdit);

        // Path section
        pathGroup = new QGroupBox("Path");
        QFormLayout* pathLayout = new QFormLayout(pathGroup);

        travelTimeSpin = new QDoubleSpinBox();
        travelTimeSpin->setRange(0, 999);
        travelTimeSpin->setSingleStep(0.1);
        pathLayout->addRow("Travel Time:", travelTimeSpin);

        // Points will be added dynamically
        pathGroup->setLayout(pathLayout);
        layout->addRow(pathGroup);

        // Drop section
        dropGroup = new QGroupBox("Drop");
        QFormLayout* dropLayout = new QFormLayout(dropGroup);

        dropItemEdit = new QLineEdit();
        dropLayout->addRow("Item:", dropItemEdit);

        dropWeightSpin = new QDoubleSpinBox();
        dropWeightSpin->setRange(0.1, 999);
        dropWeightSpin->setSingleStep(0.1);
        dropLayout->addRow("Weight:", dropWeightSpin);

        dropGroup->setLayout(dropLayout);
        layout->addRow(dropGroup);

        // Collider section
        colliderGroup = new QGroupBox("Collider");
        QFormLayout* colliderLayout = new QFormLayout(colliderGroup);

        colliderOffsetXSpin = new QDoubleSpinBox();
        colliderOffsetXSpin->setRange(-999, 999);
        colliderLayout->addRow("Offset X:", colliderOffsetXSpin);

        colliderOffsetYSpin = new QDoubleSpinBox();
        colliderOffsetYSpin->setRange(-999, 999);
        colliderLayout->addRow("Offset Y:", colliderOffsetYSpin);

        colliderTypeCombo = new QComboBox();
        colliderTypeCombo->addItem("Circle");
        colliderTypeCombo->addItem("Rectangle");
        colliderLayout->addRow("Type:", colliderTypeCombo);

        circleRadiusSpin = new QDoubleSpinBox();
        circleRadiusSpin->setRange(0.1, 999);
        circleRadiusSpin->setSingleStep(0.1);
        colliderLayout->addRow("Radius:", circleRadiusSpin);

        rectWidthSpin = new QDoubleSpinBox();
        rectWidthSpin->setRange(0.1, 999);
        rectWidthSpin->setSingleStep(0.1);
        colliderLayout->addRow("Width:", rectWidthSpin);

        rectHeightSpin = new QDoubleSpinBox();
        rectHeightSpin->setRange(0.1, 999);
        rectHeightSpin->setSingleStep(0.1);
        colliderLayout->addRow("Height:", rectHeightSpin);

        colliderGroup->setLayout(colliderLayout);
        layout->addRow(colliderGroup);

        // Connect signals
        connect(mobIdEdit, &QLineEdit::textChanged, this, &MobInspector::onMobIdChanged);
        connect(healthSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MobInspector::onHealthChanged);
        // ... connect other fields similarly

        setWidget(content);
    }

    void updateFields() {
        if (!currentMob)
            return;

        mobIdEdit->setText(QString::fromStdString(currentMob->mob_id()));
        healthSpin->setValue(currentMob->health());
        spriteEdit->setText(QString::fromStdString(currentMob->sprite()));
        spawnDelaySpin->setValue(currentMob->spawn_delay());
        attackIdEdit->setText(QString::fromStdString(currentMob->attack_id()));

        // Update path
        updatePathFields();

        // Update drop
        if (currentMob->has_drop()) {
            dropItemEdit->setText(QString::fromStdString(currentMob->drop().item()));
            dropWeightSpin->setValue(currentMob->drop().drop_weight());
        } else {
            dropItemEdit->clear();
            dropWeightSpin->setValue(1.0);
        }

        // Update collider
        updateColliderFields();
    }

    void updatePathFields() {
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
        while (pathLayout->rowCount() > 1) {
            pathLayout->removeRow(1);
        }

        // Add points
        for (int i = 0; i < path.key_points_size(); i++) {
            const auto& point = path.key_points(i);

            QLabel* pointLabel = new QLabel(QString("Point %1:").arg(i + 1));
            QDoubleSpinBox* xSpin = new QDoubleSpinBox();
            xSpin->setRange(-9999, 9999);
            xSpin->setValue(point.x());
            QDoubleSpinBox* ySpin = new QDoubleSpinBox();
            ySpin->setRange(-9999, 9999);
            ySpin->setValue(point.y());

            QHBoxLayout* pointLayout = new QHBoxLayout();
            pointLayout->addWidget(xSpin);
            pointLayout->addWidget(ySpin);

            pathLayout->addRow(pointLabel, pointLayout);

            // Store references to the spin boxes
            pathPointSpins.append(xSpin);
            pathPointSpins.append(ySpin);

            // Connect signals
            connect(xSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                    [this, i](double value) { onPathPointChanged(i, value, true); });
            connect(ySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                    [this, i](double value) { onPathPointChanged(i, value, false); });
        }
    }

    void updateColliderFields() {
        if (!currentMob || !currentMob->has_collider()) {
            colliderOffsetXSpin->setValue(0);
            colliderOffsetYSpin->setValue(0);
            colliderTypeCombo->setCurrentIndex(0);
            circleRadiusSpin->setValue(10.0);
            rectWidthSpin->setValue(10.0);
            rectHeightSpin->setValue(10.0);
            return;
        }

        const auto& collider = currentMob->collider();
        colliderOffsetXSpin->setValue(collider.offset_x());
        colliderOffsetYSpin->setValue(collider.offset_y());

        if (collider.has_circle_collider()) {
            colliderTypeCombo->setCurrentIndex(0);
            circleRadiusSpin->setValue(collider.circle_collider().radius());
            rectWidthSpin->setValue(10.0);
            rectHeightSpin->setValue(10.0);
        } else if (collider.has_rectangle_collider()) {
            colliderTypeCombo->setCurrentIndex(1);
            rectWidthSpin->setValue(collider.rectangle_collider().width());
            rectHeightSpin->setValue(collider.rectangle_collider().height());
            circleRadiusSpin->setValue(10.0);
        }
    }

    void clearFields() {
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
        circleRadiusSpin->setValue(10.0);
        rectWidthSpin->setValue(10.0);
        rectHeightSpin->setValue(10.0);
    }

private slots:
    void onMobIdChanged(const QString& text) {
        if (currentMob) {
            std::cerr << text.toStdString() << std::endl;
            currentMob->set_mob_id(text.toStdString());
            qDebug() << 1;
        }
    }

    void onHealthChanged(double value) {
        if (currentMob) {
            currentMob->set_health(value);
            qDebug() << 2;
        }
    }

    void onPathPointChanged(int pointIndex, double value, bool isX) {
        if (!currentMob || !currentMob->has_path())
            return;

        auto* path = currentMob->mutable_path();
        if (pointIndex >= path->key_points_size())
            return;

        auto* point = path->mutable_key_points(pointIndex);
        if (isX) {
            point->set_x(value);
        } else {
            point->set_y(value);
        }
    }

    // ... similar slots for other fields
};

#endif
