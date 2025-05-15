#include "attackinspector.h"

#include "datamanager.h"

namespace {
const QString kBasicGroupTitle = "Basic";
const QString kPatternGroupTitle = "Attack Pattern";

const int kMinWidth = 300;
const double kMaxValue = 999;
const double kMinNegative = -999;
const double kMaxAngle = 360;
const double kSingleStep = 0.1;

const int kBulletIdIndex = 2;
const int kAttackIdIndex = 3;

enum class AttackType {
    BulletId,
    AttackId,
};

enum class AttackPattern {
    Circle,
    Arc,
    Line,
    Spiral,
};

enum class ConfigType {
    Delta,
    Random,
};
} // namespace

AttackInspector::AttackInspector(QUndoStack *undoStack, QWidget *parent)
    : QWidget(parent),
      m_basicGroup(new QGroupBox(kBasicGroupTitle)),
      m_nameEdit(new QLineEdit()),
      m_attackTypeCombo(new QComboBox()),
      m_bulletIdSpin(new QSpinBox()),
      m_attackIdSpin(new QSpinBox()),
      m_speedSpin(new QDoubleSpinBox()),
      m_spawnOffsetXSpin(new QDoubleSpinBox()),
      m_spawnOffsetYSpin(new QDoubleSpinBox()),
      m_amountSpin(new QDoubleSpinBox()),
      m_attackPatternCombo(new QComboBox()),
      m_circleGroup(new QGroupBox("Circle Pattern")),
      m_circleRotationAngleSpin(new QDoubleSpinBox()),
      m_arcGroup(new QGroupBox("Arc Pattern")),
      m_arcLockOnPlayerCheck(new QCheckBox("Lock on player")),
      m_arcDirectionAngleSpin(new QDoubleSpinBox()),
      m_arcSpreadAngleSpin(new QDoubleSpinBox()),
      m_lineGroup(new QGroupBox("Line Pattern")),
      m_lineLockOnPlayerCheck(new QCheckBox("Lock on player")),
      m_lineDirectionAngleSpin(new QDoubleSpinBox()),
      m_lineDeltaTimeSpin(new QDoubleSpinBox()),
      m_lineSpeedConfigCombo(new QComboBox()),
      m_lineDeltaSpeedSpin(new QDoubleSpinBox()),
      m_lineRandomSpeedMinSpin(new QDoubleSpinBox()),
      m_lineRandomSpeedMaxSpin(new QDoubleSpinBox()),
      m_lineAmountConfigCombo(new QComboBox()),
      m_lineDeltaAmountSpin(new QDoubleSpinBox()),
      m_lineRandomAmountMinSpin(new QDoubleSpinBox()),
      m_lineRandomAmountMaxSpin(new QDoubleSpinBox()),
      m_lineDirectionConfigCombo(new QComboBox()),
      m_lineDeltaDirectionSpin(new QDoubleSpinBox()),
      m_lineRandomDirectionMinSpin(new QDoubleSpinBox()),
      m_lineRandomDirectionMaxSpin(new QDoubleSpinBox()),
      m_spiralGroup(new QGroupBox("Spiral Pattern")),
      m_spiralLockOnPlayerCheck(new QCheckBox("Lock on player")),
      m_spiralBeginAngleSpin(new QDoubleSpinBox()),
      m_spiralRotationAngleSpin(new QDoubleSpinBox()),
      m_spiralDeltaTimeSpin(new QDoubleSpinBox()),
      m_spiralSpeedConfigCombo(new QComboBox()),
      m_spiralDeltaSpeedSpin(new QDoubleSpinBox()),
      m_spiralRandomSpeedMinSpin(new QDoubleSpinBox()),
      m_spiralRandomSpeedMaxSpin(new QDoubleSpinBox()),
      m_spiralAmountConfigCombo(new QComboBox()),
      m_spiralDeltaAmountSpin(new QDoubleSpinBox()),
      m_spiralRandomAmountMinSpin(new QDoubleSpinBox()),
      m_spiralRandomAmountMaxSpin(new QDoubleSpinBox()),
      m_spiralDirectionConfigCombo(new QComboBox()),
      m_spiralDeltaDirectionSpin(new QDoubleSpinBox()),
      m_spiralRandomDirectionMinSpin(new QDoubleSpinBox()),
      m_spiralRandomDirectionMaxSpin(new QDoubleSpinBox()),
      m_currentAttack(nullptr),
      m_undoStack(undoStack)
{
    setMinimumWidth(kMinWidth);
    setupUI();
}

void AttackInspector::setAttack(game::Attack *attack)
{
    m_currentAttack = attack;
    updateFields();
}

void AttackInspector::clear()
{
    m_currentAttack = nullptr;
    clearFields();
}

void AttackInspector::setupUI()
{
    QFormLayout *layout = new QFormLayout(this);

    // Basic fields
    QFormLayout *basicLayout = new QFormLayout(m_basicGroup);
    basicLayout->addRow("Name:", m_nameEdit);

    m_attackTypeCombo->addItem("Bullet ID");
    m_attackTypeCombo->addItem("Attack ID");
    basicLayout->addRow("Attack Type:", m_attackTypeCombo);

    m_bulletIdSpin->setRange(0, INT_MAX);
    basicLayout->addRow("Bullet ID:", m_bulletIdSpin);

    m_attackIdSpin->setRange(0, INT_MAX);
    basicLayout->addRow("Attack ID:", m_attackIdSpin);

    m_speedSpin->setRange(0, kMaxValue);
    m_speedSpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Speed:", m_speedSpin);

    m_spawnOffsetXSpin->setRange(kMinNegative, kMaxValue);
    m_spawnOffsetXSpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Spawn Offset X:", m_spawnOffsetXSpin);

    m_spawnOffsetYSpin->setRange(kMinNegative, kMaxValue);
    m_spawnOffsetYSpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Spawn Offset Y:", m_spawnOffsetYSpin);

    m_amountSpin->setRange(1, kMaxValue);
    m_amountSpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Amount:", m_amountSpin);

    m_attackPatternCombo->addItem("Circle");
    m_attackPatternCombo->addItem("Arc");
    m_attackPatternCombo->addItem("Line");
    m_attackPatternCombo->addItem("Spiral");
    basicLayout->addRow("Attack Pattern:", m_attackPatternCombo);

    m_basicGroup->setLayout(basicLayout);
    layout->addRow(m_basicGroup);

    // Circle pattern
    QFormLayout *circleLayout = new QFormLayout(m_circleGroup);
    m_circleRotationAngleSpin->setRange(kMinNegative, kMaxValue);
    m_circleRotationAngleSpin->setSingleStep(kSingleStep);
    circleLayout->addRow("Rotation Angle:", m_circleRotationAngleSpin);
    m_circleGroup->setLayout(circleLayout);
    layout->addRow(m_circleGroup);

    // Arc pattern
    QFormLayout *arcLayout = new QFormLayout(m_arcGroup);
    arcLayout->addRow(m_arcLockOnPlayerCheck);
    m_arcDirectionAngleSpin->setRange(kMinNegative, kMaxValue);
    m_arcDirectionAngleSpin->setSingleStep(kSingleStep);
    arcLayout->addRow("Direction Angle:", m_arcDirectionAngleSpin);
    m_arcSpreadAngleSpin->setRange(0, kMaxAngle);
    m_arcSpreadAngleSpin->setSingleStep(kSingleStep);
    arcLayout->addRow("Spread Angle:", m_arcSpreadAngleSpin);
    m_arcGroup->setLayout(arcLayout);
    layout->addRow(m_arcGroup);

    // Line pattern
    QFormLayout *lineLayout = new QFormLayout(m_lineGroup);
    lineLayout->addRow(m_lineLockOnPlayerCheck);
    m_lineDirectionAngleSpin->setRange(kMinNegative, kMaxValue);
    m_lineDirectionAngleSpin->setSingleStep(kSingleStep);
    lineLayout->addRow("Direction Angle:", m_lineDirectionAngleSpin);
    m_lineDeltaTimeSpin->setRange(0, kMaxValue);
    m_lineDeltaTimeSpin->setSingleStep(kSingleStep);
    lineLayout->addRow("Delta Time:", m_lineDeltaTimeSpin);

    m_lineSpeedConfigCombo->addItem("Delta Speed");
    m_lineSpeedConfigCombo->addItem("Random Speed");
    lineLayout->addRow("Speed Config:", m_lineSpeedConfigCombo);
    m_lineDeltaSpeedSpin->setRange(kMinNegative, kMaxValue);
    m_lineDeltaSpeedSpin->setSingleStep(kSingleStep);
    lineLayout->addRow("Delta Speed:", m_lineDeltaSpeedSpin);
    m_lineRandomSpeedMinSpin->setRange(kMinNegative, kMaxValue);
    m_lineRandomSpeedMinSpin->setSingleStep(kSingleStep);
    m_lineRandomSpeedMaxSpin->setRange(kMinNegative, kMaxValue);
    m_lineRandomSpeedMaxSpin->setSingleStep(kSingleStep);
    QHBoxLayout *randomSpeedLayout = new QHBoxLayout();
    randomSpeedLayout->addWidget(new QLabel("Min:"));
    randomSpeedLayout->addWidget(m_lineRandomSpeedMinSpin);
    randomSpeedLayout->addWidget(new QLabel("Max:"));
    randomSpeedLayout->addWidget(m_lineRandomSpeedMaxSpin);
    lineLayout->addRow("Random Speed:", randomSpeedLayout);

    m_lineAmountConfigCombo->addItem("Delta Amount");
    m_lineAmountConfigCombo->addItem("Random Amount");
    lineLayout->addRow("Amount Config:", m_lineAmountConfigCombo);
    m_lineDeltaAmountSpin->setRange(kMinNegative, kMaxValue);
    m_lineDeltaAmountSpin->setSingleStep(kSingleStep);
    lineLayout->addRow("Delta Amount:", m_lineDeltaAmountSpin);
    m_lineRandomAmountMinSpin->setRange(1, kMaxValue);
    m_lineRandomAmountMinSpin->setSingleStep(kSingleStep);
    m_lineRandomAmountMaxSpin->setRange(1, kMaxValue);
    m_lineRandomAmountMaxSpin->setSingleStep(kSingleStep);
    QHBoxLayout *randomAmountLayout = new QHBoxLayout();
    randomAmountLayout->addWidget(new QLabel("Min:"));
    randomAmountLayout->addWidget(m_lineRandomAmountMinSpin);
    randomAmountLayout->addWidget(new QLabel("Max:"));
    randomAmountLayout->addWidget(m_lineRandomAmountMaxSpin);
    lineLayout->addRow("Random Amount:", randomAmountLayout);

    m_lineDirectionConfigCombo->addItem("Delta Direction");
    m_lineDirectionConfigCombo->addItem("Random Direction");
    lineLayout->addRow("Direction Config:", m_lineDirectionConfigCombo);
    m_lineDeltaDirectionSpin->setRange(kMinNegative, kMaxValue);
    m_lineDeltaDirectionSpin->setSingleStep(kSingleStep);
    lineLayout->addRow("Delta Direction:", m_lineDeltaDirectionSpin);
    m_lineRandomDirectionMinSpin->setRange(kMinNegative, kMaxValue);
    m_lineRandomDirectionMinSpin->setSingleStep(kSingleStep);
    m_lineRandomDirectionMaxSpin->setRange(kMinNegative, kMaxValue);
    m_lineRandomDirectionMaxSpin->setSingleStep(kSingleStep);
    QHBoxLayout *randomDirectionLayout = new QHBoxLayout();
    randomDirectionLayout->addWidget(new QLabel("Min:"));
    randomDirectionLayout->addWidget(m_lineRandomDirectionMinSpin);
    randomDirectionLayout->addWidget(new QLabel("Max:"));
    randomDirectionLayout->addWidget(m_lineRandomDirectionMaxSpin);
    lineLayout->addRow("Random Direction:", randomDirectionLayout);

    m_lineGroup->setLayout(lineLayout);
    layout->addRow(m_lineGroup);

    // Spiral pattern (similar to Line pattern)
    QFormLayout *spiralLayout = new QFormLayout(m_spiralGroup);
    spiralLayout->addRow(m_spiralLockOnPlayerCheck);
    m_spiralBeginAngleSpin->setRange(kMinNegative, kMaxValue);
    m_spiralBeginAngleSpin->setSingleStep(kSingleStep);
    spiralLayout->addRow("Begin Angle:", m_spiralBeginAngleSpin);
    m_spiralRotationAngleSpin->setRange(kMinNegative, kMaxValue);
    m_spiralRotationAngleSpin->setSingleStep(kSingleStep);
    spiralLayout->addRow("Rotation Angle:", m_spiralRotationAngleSpin);
    m_spiralDeltaTimeSpin->setRange(0, kMaxValue);
    m_spiralDeltaTimeSpin->setSingleStep(kSingleStep);
    spiralLayout->addRow("Delta Time:", m_spiralDeltaTimeSpin);

    m_spiralSpeedConfigCombo->addItem("Delta Speed");
    m_spiralSpeedConfigCombo->addItem("Random Speed");
    spiralLayout->addRow("Speed Config:", m_spiralSpeedConfigCombo);
    m_spiralDeltaSpeedSpin->setRange(kMinNegative, kMaxValue);
    m_spiralDeltaSpeedSpin->setSingleStep(kSingleStep);
    spiralLayout->addRow("Delta Speed:", m_spiralDeltaSpeedSpin);
    m_spiralRandomSpeedMinSpin->setRange(kMinNegative, kMaxValue);
    m_spiralRandomSpeedMinSpin->setSingleStep(kSingleStep);
    m_spiralRandomSpeedMaxSpin->setRange(kMinNegative, kMaxValue);
    m_spiralRandomSpeedMaxSpin->setSingleStep(kSingleStep);
    QHBoxLayout *spiralRandomSpeedLayout = new QHBoxLayout();
    spiralRandomSpeedLayout->addWidget(new QLabel("Min:"));
    spiralRandomSpeedLayout->addWidget(m_spiralRandomSpeedMinSpin);
    spiralRandomSpeedLayout->addWidget(new QLabel("Max:"));
    spiralRandomSpeedLayout->addWidget(m_spiralRandomSpeedMaxSpin);
    spiralLayout->addRow("Random Speed:", spiralRandomSpeedLayout);

    m_spiralAmountConfigCombo->addItem("Delta Amount");
    m_spiralAmountConfigCombo->addItem("Random Amount");
    spiralLayout->addRow("Amount Config:", m_spiralAmountConfigCombo);
    m_spiralDeltaAmountSpin->setRange(kMinNegative, kMaxValue);
    m_spiralDeltaAmountSpin->setSingleStep(kSingleStep);
    spiralLayout->addRow("Delta Amount:", m_spiralDeltaAmountSpin);
    m_spiralRandomAmountMinSpin->setRange(1, kMaxValue);
    m_spiralRandomAmountMinSpin->setSingleStep(kSingleStep);
    m_spiralRandomAmountMaxSpin->setRange(1, kMaxValue);
    m_spiralRandomAmountMaxSpin->setSingleStep(kSingleStep);
    QHBoxLayout *spiralRandomAmountLayout = new QHBoxLayout();
    spiralRandomAmountLayout->addWidget(new QLabel("Min:"));
    spiralRandomAmountLayout->addWidget(m_spiralRandomAmountMinSpin);
    spiralRandomAmountLayout->addWidget(new QLabel("Max:"));
    spiralRandomAmountLayout->addWidget(m_spiralRandomAmountMaxSpin);
    spiralLayout->addRow("Random Amount:", spiralRandomAmountLayout);

    m_spiralDirectionConfigCombo->addItem("Delta Direction");
    m_spiralDirectionConfigCombo->addItem("Random Direction");
    spiralLayout->addRow("Direction Config:", m_spiralDirectionConfigCombo);
    m_spiralDeltaDirectionSpin->setRange(kMinNegative, kMaxValue);
    m_spiralDeltaDirectionSpin->setSingleStep(kSingleStep);
    spiralLayout->addRow("Delta Direction:", m_spiralDeltaDirectionSpin);
    m_spiralRandomDirectionMinSpin->setRange(kMinNegative, kMaxValue);
    m_spiralRandomDirectionMinSpin->setSingleStep(kSingleStep);
    m_spiralRandomDirectionMaxSpin->setRange(kMinNegative, kMaxValue);
    m_spiralRandomDirectionMaxSpin->setSingleStep(kSingleStep);
    QHBoxLayout *spiralRandomDirectionLayout = new QHBoxLayout();
    spiralRandomDirectionLayout->addWidget(new QLabel("Min:"));
    spiralRandomDirectionLayout->addWidget(m_spiralRandomDirectionMinSpin);
    spiralRandomDirectionLayout->addWidget(new QLabel("Max:"));
    spiralRandomDirectionLayout->addWidget(m_spiralRandomDirectionMaxSpin);
    spiralLayout->addRow("Random Direction:", spiralRandomDirectionLayout);

    m_spiralGroup->setLayout(spiralLayout);
    layout->addRow(m_spiralGroup);

    // Connect signals
    connect(m_nameEdit, &QLineEdit::textChanged, this, &AttackInspector::onNameChanged);
    connect(m_attackTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &AttackInspector::onAttackTypeChanged);
    connect(m_bulletIdSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &AttackInspector::onBulletIdChanged);
    connect(m_attackIdSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &AttackInspector::onAttackIdChanged);
    connect(m_speedSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpeedChanged);
    connect(m_spawnOffsetXSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpawnOffsetXChanged);
    connect(m_spawnOffsetYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpawnOffsetYChanged);
    connect(m_amountSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onAmountChanged);
    connect(m_attackPatternCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &AttackInspector::onAttackPatternChanged);

    // Connect Circle Pattern group to appropriate slots
    connect(m_circleRotationAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onCircleRotationAngleChanged);

    // Conncet Arc Pattern group to appropriate slots
    connect(m_arcLockOnPlayerCheck, &QCheckBox::stateChanged, this,
            &AttackInspector::onArcLockOnPlayerChanged);
    connect(m_arcDirectionAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onArcDirectionAngleChanged);
    connect(m_arcSpreadAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onArcSpreadAngleChanged);

    // Connect Line Pattern group signals to appropriate slots
    connect(m_lineLockOnPlayerCheck, &QCheckBox::stateChanged, this,
            &AttackInspector::onLineLockOnPlayerChanged);
    connect(m_lineDirectionAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onLineDirectionAngleChanged);
    connect(m_lineDeltaTimeSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onLineDeltaTimeChanged);
    connect(m_lineSpeedConfigCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &AttackInspector::onLineDeltaSpeedChanged);
    connect(m_lineRandomSpeedMinSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onLineRandomSpeedMinChanged);
    connect(m_lineRandomSpeedMaxSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onLineRandomSpeedMaxChanged);
    connect(m_lineDeltaAmountSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onLineDeltaAmountChanged);
    connect(m_lineRandomAmountMinSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onLineRandomAmountMinChanged);
    connect(m_lineRandomAmountMaxSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onLineRandomAmountMaxChanged);
    connect(m_lineDeltaDirectionSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onLineDeltaDirectionChanged);
    connect(m_lineRandomDirectionMinSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onLineRandomDirectionMinChanged);
    connect(m_lineRandomDirectionMaxSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onLineRandomDirectionMaxChanged);

    // Connect Spiral Pattern group signals to appropriate slots
    connect(m_spiralLockOnPlayerCheck, &QCheckBox::stateChanged, this,
            &AttackInspector::onSpiralLockOnPlayerChanged);
    connect(m_spiralBeginAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpiralBeginAngleChanged);
    connect(m_spiralRotationAngleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpiralRotationAngleChanged);
    connect(m_spiralDeltaTimeSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onSpiralDeltaTimeChanged);
    connect(m_spiralDeltaSpeedSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpiralDeltaSpeedChanged);
    connect(m_spiralRandomSpeedMinSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onSpiralRandomSpeedMinChanged);
    connect(m_spiralRandomSpeedMaxSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onSpiralRandomSpeedMaxChanged);
    connect(m_spiralDeltaAmountSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpiralDeltaAmountChanged);
    connect(m_spiralRandomAmountMinSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onSpiralRandomAmountMinChanged);
    connect(m_spiralRandomAmountMaxSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onSpiralRandomAmountMaxChanged);
    connect(m_spiralDeltaDirectionSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &AttackInspector::onSpiralDeltaDirectionChanged);
    connect(m_spiralRandomDirectionMinSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onSpiralRandomDirectionMinChanged);
    connect(m_spiralRandomDirectionMaxSpin, &QDoubleSpinBox::valueChanged, this,
            &AttackInspector::onSpiralRandomDirectionMaxChanged);
}

void AttackInspector::updateFields()
{
    if (!m_currentAttack) {
        return;
    }

    m_nameEdit->setText(QString::fromStdString(m_currentAttack->name()));
    m_speedSpin->setValue(m_currentAttack->speed());
    m_spawnOffsetXSpin->setValue(m_currentAttack->spawn_position_offset().x());
    m_spawnOffsetYSpin->setValue(m_currentAttack->spawn_position_offset().y());
    m_amountSpin->setValue(m_currentAttack->amount());

    // Update attack type
    if (m_currentAttack->has_bullet_id()) {
        m_attackTypeCombo->setCurrentIndex(static_cast<int>(AttackType::BulletId));
        m_bulletIdSpin->setValue(m_currentAttack->bullet_id());
    } else if (m_currentAttack->has_attack_id()) {
        m_attackTypeCombo->setCurrentIndex(static_cast<int>(AttackType::AttackId));
        m_attackIdSpin->setValue(m_currentAttack->attack_id());
    }

    // Update attack pattern
    updateAttackPatternFields();
    updateAttackTypeFields();
}

void AttackInspector::updateAttackPatternFields()
{
    if (!m_currentAttack) {
        return;
    }

    // Hide all pattern groups first
    m_circleGroup->hide();
    m_arcGroup->hide();
    m_lineGroup->hide();
    m_spiralGroup->hide();

    if (m_currentAttack->has_circle()) {
        m_attackPatternCombo->setCurrentIndex(static_cast<int>(AttackPattern::Circle));
        m_circleGroup->show();
        m_circleRotationAngleSpin->setValue(m_currentAttack->circle().rotation_angle());
    } else if (m_currentAttack->has_arc()) {
        m_attackPatternCombo->setCurrentIndex(static_cast<int>(AttackPattern::Arc));
        m_arcGroup->show();
        m_arcLockOnPlayerCheck->setChecked(m_currentAttack->arc().lock_on_player());
        m_arcDirectionAngleSpin->setValue(m_currentAttack->arc().direction_angle());
        m_arcSpreadAngleSpin->setValue(m_currentAttack->arc().spread_angle());
    } else if (m_currentAttack->has_line()) {
        m_attackPatternCombo->setCurrentIndex(static_cast<int>(AttackPattern::Line));
        m_lineGroup->show();
        const auto &line = m_currentAttack->line();
        m_lineLockOnPlayerCheck->setChecked(line.lock_on_player());
        m_lineDirectionAngleSpin->setValue(line.direction_angle());
        m_lineDeltaTimeSpin->setValue(line.delta_time());

        if (line.has_delta_speed()) {
            m_lineSpeedConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Delta));
            m_lineDeltaSpeedSpin->setValue(line.delta_speed());
        } else if (line.has_random_speed()) {
            m_lineSpeedConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Random));
            m_lineRandomSpeedMinSpin->setValue(line.random_speed().min_value());
            m_lineRandomSpeedMaxSpin->setValue(line.random_speed().max_value());
        }

        if (line.has_delta_amount()) {
            m_lineAmountConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Delta));
            m_lineDeltaAmountSpin->setValue(line.delta_amount());
        } else if (line.has_random_amount()) {
            m_lineAmountConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Random));
            m_lineRandomAmountMinSpin->setValue(line.random_amount().min_value());
            m_lineRandomAmountMaxSpin->setValue(line.random_amount().max_value());
        }

        if (line.has_delta_direction()) {
            m_lineDirectionConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Delta));
            m_lineDeltaDirectionSpin->setValue(line.delta_direction());
        } else if (line.has_random_direction()) {
            m_lineDirectionConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Random));
            m_lineRandomDirectionMinSpin->setValue(line.random_direction().min_value());
            m_lineRandomDirectionMaxSpin->setValue(line.random_direction().max_value());
        }
    } else if (m_currentAttack->has_spiral()) {
        m_attackPatternCombo->setCurrentIndex(static_cast<int>(AttackPattern::Spiral));
        m_spiralGroup->show();
        const auto &spiral = m_currentAttack->spiral();
        m_spiralLockOnPlayerCheck->setChecked(spiral.lock_on_player());
        m_spiralBeginAngleSpin->setValue(spiral.begin_angle());
        m_spiralRotationAngleSpin->setValue(spiral.rotation_angle());
        m_spiralDeltaTimeSpin->setValue(spiral.delta_time());

        if (spiral.has_delta_speed()) {
            m_spiralSpeedConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Delta));
            m_spiralDeltaSpeedSpin->setValue(spiral.delta_speed());
        } else if (spiral.has_random_speed()) {
            m_spiralSpeedConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Random));
            m_spiralRandomSpeedMinSpin->setValue(spiral.random_speed().min_value());
            m_spiralRandomSpeedMaxSpin->setValue(spiral.random_speed().max_value());
        }

        if (spiral.has_delta_amount()) {
            m_spiralAmountConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Delta));
            m_spiralDeltaAmountSpin->setValue(spiral.delta_amount());
        } else if (spiral.has_random_amount()) {
            m_spiralAmountConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Random));
            m_spiralRandomAmountMinSpin->setValue(spiral.random_amount().min_value());
            m_spiralRandomAmountMaxSpin->setValue(spiral.random_amount().max_value());
        }

        if (spiral.has_delta_direction()) {
            m_spiralDirectionConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Delta));
            m_spiralDeltaDirectionSpin->setValue(spiral.delta_direction());
        } else if (spiral.has_random_direction()) {
            m_spiralDirectionConfigCombo->setCurrentIndex(static_cast<int>(ConfigType::Random));
            m_spiralRandomDirectionMinSpin->setValue(spiral.random_direction().min_value());
            m_spiralRandomDirectionMaxSpin->setValue(spiral.random_direction().max_value());
        }
    }
}

void AttackInspector::clearFields()
{
    m_nameEdit->clear();
    m_attackTypeCombo->setCurrentIndex(0);
    m_bulletIdSpin->setValue(0);
    m_attackIdSpin->setValue(0);
    m_speedSpin->setValue(0);
    m_spawnOffsetXSpin->setValue(0);
    m_spawnOffsetYSpin->setValue(0);
    m_amountSpin->setValue(1);
    m_attackPatternCombo->setCurrentIndex(0);

    m_circleRotationAngleSpin->setValue(0);
    m_arcLockOnPlayerCheck->setChecked(false);
    m_arcDirectionAngleSpin->setValue(0);
    m_arcSpreadAngleSpin->setValue(0);
    m_lineLockOnPlayerCheck->setChecked(false);
    m_lineDirectionAngleSpin->setValue(0);
    m_lineDeltaTimeSpin->setValue(0);
    m_lineDeltaSpeedSpin->setValue(0);
    m_lineRandomSpeedMinSpin->setValue(0);
    m_lineRandomSpeedMaxSpin->setValue(0);
    m_lineDeltaAmountSpin->setValue(0);
    m_lineRandomAmountMinSpin->setValue(1);
    m_lineRandomAmountMaxSpin->setValue(1);
    m_lineDeltaDirectionSpin->setValue(0);
    m_lineRandomDirectionMinSpin->setValue(0);
    m_lineRandomDirectionMaxSpin->setValue(0);
    m_spiralLockOnPlayerCheck->setChecked(false);
    m_spiralBeginAngleSpin->setValue(0);
    m_spiralRotationAngleSpin->setValue(0);
    m_spiralDeltaTimeSpin->setValue(0);
    m_spiralDeltaSpeedSpin->setValue(0);
    m_spiralRandomSpeedMinSpin->setValue(0);
    m_spiralRandomSpeedMaxSpin->setValue(0);
    m_spiralDeltaAmountSpin->setValue(0);
    m_spiralRandomAmountMinSpin->setValue(1);
    m_spiralRandomAmountMaxSpin->setValue(1);
    m_spiralDeltaDirectionSpin->setValue(0);
    m_spiralRandomDirectionMinSpin->setValue(0);
    m_spiralRandomDirectionMaxSpin->setValue(0);

    m_circleGroup->hide();
    m_arcGroup->hide();
    m_lineGroup->hide();
    m_spiralGroup->hide();
}

void AttackInspector::updateAttackTypeFields()
{
    if (!m_currentAttack) {
        return;
    }

    QFormLayout *basicLayout = qobject_cast<QFormLayout *>(m_basicGroup->layout());

    if (m_currentAttack->has_bullet_id()) {
        basicLayout->setRowVisible(kBulletIdIndex, true);
        basicLayout->setRowVisible(kAttackIdIndex, false);
        m_bulletIdSpin->setValue(m_currentAttack->bullet_id());
    } else if (m_currentAttack->has_attack_id()) {
        basicLayout->setRowVisible(kBulletIdIndex, false);
        basicLayout->setRowVisible(kAttackIdIndex, true);
        m_attackIdSpin->setValue(m_currentAttack->attack_id());
    }
}

// Signal handlers implementation
void AttackInspector::onNameChanged(const QString &text)
{
    if (m_currentAttack) {
        m_currentAttack->set_name(text.toStdString());
    }
}

void AttackInspector::onAttackTypeChanged(int index)
{
    if (!m_currentAttack) {
        return;
    }

    if (index == static_cast<int>(AttackType::BulletId)) {
        m_currentAttack->set_bullet_id(m_bulletIdSpin->value());
        m_currentAttack->clear_attack_id();
    } else {
        m_currentAttack->set_attack_id(m_attackIdSpin->value());
        m_currentAttack->clear_bullet_id();
    }

    updateAttackTypeFields();
}

void AttackInspector::onBulletIdChanged(int value)
{
    if (m_currentAttack
        && m_attackTypeCombo->currentIndex() == static_cast<int>(AttackType::BulletId)) {
        m_currentAttack->set_bullet_id(value);
    }
}

void AttackInspector::onAttackIdChanged(int value)
{
    if (m_currentAttack
        && m_attackTypeCombo->currentIndex() == static_cast<int>(AttackType::AttackId)) {
        m_currentAttack->set_attack_id(value);
    }
}

void AttackInspector::onSpeedChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->set_speed(static_cast<float>(value));
    }
}

void AttackInspector::onSpawnOffsetXChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spawn_position_offset()->set_x(static_cast<float>(value));
    }
}

void AttackInspector::onSpawnOffsetYChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spawn_position_offset()->set_y(static_cast<float>(value));
    }
}

void AttackInspector::onAmountChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->set_amount(static_cast<float>(value));
    }
}

void AttackInspector::onAttackPatternChanged(int index)
{
    if (!m_currentAttack) {
        return;
    }

    // Clear all pattern types
    m_currentAttack->clear_circle();
    m_currentAttack->clear_arc();
    m_currentAttack->clear_line();
    m_currentAttack->clear_spiral();

    switch (static_cast<AttackPattern>(index)) {
    case AttackPattern::Circle:
        m_currentAttack->mutable_circle();
        break;
    case AttackPattern::Arc:
        m_currentAttack->mutable_arc();
        break;
    case AttackPattern::Line:
        m_currentAttack->mutable_line();
        break;
    case AttackPattern::Spiral:
        m_currentAttack->mutable_spiral();
        break;
    }

    updateAttackPatternFields();
}

void AttackInspector::onCircleRotationAngleChanged(double value)
{
    if (m_currentAttack && m_currentAttack->has_circle()) {
        m_currentAttack->mutable_circle()->set_rotation_angle(static_cast<float>(value));
    }
}

void AttackInspector::onArcLockOnPlayerChanged(int state)
{
    if (m_currentAttack && m_currentAttack->has_arc()) {
        m_currentAttack->mutable_arc()->set_lock_on_player(state == Qt::Checked);
    }
}

void AttackInspector::onArcDirectionAngleChanged(double value)
{
    if (m_currentAttack && m_currentAttack->has_arc()) {
        m_currentAttack->mutable_arc()->set_direction_angle(static_cast<float>(value));
    }
}

void AttackInspector::onArcSpreadAngleChanged(double value)
{
    if (m_currentAttack && m_currentAttack->has_arc()) {
        m_currentAttack->mutable_arc()->set_spread_angle(static_cast<float>(value));
    }
}

void AttackInspector::onLineLockOnPlayerChanged(int state)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->set_lock_on_player(state == Qt::Checked);
    }
}

void AttackInspector::onLineDirectionAngleChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->set_direction_angle(static_cast<float>(value));
    }
}

void AttackInspector::onLineDeltaTimeChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->set_delta_time(static_cast<float>(value));
    }
}

void AttackInspector::onLineDeltaSpeedChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->set_delta_speed(static_cast<float>(value));
    }
}

void AttackInspector::onLineRandomSpeedMinChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->mutable_random_speed()->set_min_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onLineRandomSpeedMaxChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->mutable_random_speed()->set_max_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onLineDeltaAmountChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->set_delta_amount(static_cast<float>(value));
    }
}

void AttackInspector::onLineRandomAmountMinChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->mutable_random_amount()->set_min_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onLineRandomAmountMaxChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->mutable_random_amount()->set_max_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onLineDeltaDirectionChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->set_delta_direction(static_cast<float>(value));
    }
}

void AttackInspector::onLineRandomDirectionMinChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->mutable_random_direction()->set_min_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onLineRandomDirectionMaxChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_line()->mutable_random_direction()->set_max_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onSpiralLockOnPlayerChanged(int state)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->set_lock_on_player(state == Qt::Checked);
    }
}

void AttackInspector::onSpiralBeginAngleChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->set_begin_angle(static_cast<float>(value));
    }
}

void AttackInspector::onSpiralRotationAngleChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->set_rotation_angle(static_cast<float>(value));
    }
}

void AttackInspector::onSpiralDeltaTimeChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->set_delta_time(static_cast<float>(value));
    }
}

void AttackInspector::onSpiralDeltaSpeedChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->set_delta_speed(static_cast<float>(value));
    }
}

void AttackInspector::onSpiralRandomSpeedMinChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->mutable_random_speed()->set_min_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onSpiralRandomSpeedMaxChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->mutable_random_speed()->set_max_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onSpiralDeltaAmountChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->set_delta_amount(static_cast<float>(value));
    }
}

void AttackInspector::onSpiralRandomAmountMinChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->mutable_random_amount()->set_min_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onSpiralRandomAmountMaxChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->mutable_random_amount()->set_max_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onSpiralDeltaDirectionChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->set_delta_direction(static_cast<float>(value));
    }
}

void AttackInspector::onSpiralRandomDirectionMinChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->mutable_random_direction()->set_min_value(
                static_cast<float>(value));
    }
}

void AttackInspector::onSpiralRandomDirectionMaxChanged(double value)
{
    if (m_currentAttack) {
        m_currentAttack->mutable_spiral()->mutable_random_direction()->set_max_value(
                static_cast<float>(value));
    }
}
