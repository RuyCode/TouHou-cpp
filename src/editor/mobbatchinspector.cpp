// mobbatchinspector.cpp
#include "mobbatchinspector.h"

#include "datamanager.h"

namespace {
const QString kBasicGroupTitle = "Basic";
const QString kMobsGroupTitle = "Mobs";
const double kMaxValue = 999;
const double kSingleStep = 0.1;
const int kMinWidth = 300;
const int kDeleteButtonWidthPadding = 10;
const int kMobsGroupOffset = 1; // Add button
} // namespace

MobBatchInspector::MobBatchInspector(QUndoStack *undoStack, QWidget *parent)
    : QWidget(parent),
      m_basicGroup(new QGroupBox(kBasicGroupTitle)),
      m_nameEdit(new QLineEdit()),
      m_delaySpin(new QDoubleSpinBox()),
      m_lifespanSpin(new QDoubleSpinBox()),
      m_mobsGroup(new QGroupBox(kMobsGroupTitle)),
      m_addMobButton(new QPushButton()),
      m_currentMobBatch(nullptr),
      m_undoStack(undoStack)
{
    setMinimumWidth(kMinWidth);
    setupUI();
}

void MobBatchInspector::setMobBatch(game::MobBatch *mobBatch)
{
    m_currentMobBatch = mobBatch;
    updateFields();
}

void MobBatchInspector::clear()
{
    m_currentMobBatch = nullptr;
    clearFields();
}

void MobBatchInspector::setupUI()
{
    QFormLayout *layout = new QFormLayout(this);

    // Basic fields
    QFormLayout *basicLayout = new QFormLayout(m_basicGroup);
    basicLayout->addRow("Name:", m_nameEdit);

    m_delaySpin->setRange(0, kMaxValue);
    m_delaySpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Delay after previous batch:", m_delaySpin);

    m_lifespanSpin->setRange(0, kMaxValue);
    m_lifespanSpin->setSingleStep(kSingleStep);
    basicLayout->addRow("Lifespan:", m_lifespanSpin);

    m_basicGroup->setLayout(basicLayout);
    layout->addRow(m_basicGroup);

    // Mobs list section
    QFormLayout *mobsLayout = new QFormLayout(m_mobsGroup);

    m_addMobButton->setText("Add Mob");
    mobsLayout->addRow(m_addMobButton);

    m_mobsGroup->setLayout(mobsLayout);
    layout->addRow(m_mobsGroup);

    // Connect signals
    connect(m_nameEdit, &QLineEdit::textChanged, this, &MobBatchInspector::onNameChanged);
    connect(m_delaySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobBatchInspector::onDelayChanged);
    connect(m_lifespanSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MobBatchInspector::onLifespanChanged);
    connect(m_addMobButton, &QPushButton::clicked, this, &MobBatchInspector::onAddMobClicked);
}

void MobBatchInspector::updateFields()
{
    if (!m_currentMobBatch) {
        return;
    }

    m_nameEdit->setText(QString::fromStdString(m_currentMobBatch->name()));
    m_delaySpin->setValue(m_currentMobBatch->delay_after_previous_batch());
    m_lifespanSpin->setValue(m_currentMobBatch->lifespan());

    updateMobsList();
}

void MobBatchInspector::updateMobsList()
{
    qDeleteAll(m_deleteMobButtons);
    m_deleteMobButtons.clear();

    if (!m_currentMobBatch) {
        return;
    }

    QFormLayout *mobsLayout = qobject_cast<QFormLayout *>(m_mobsGroup->layout());
    while (mobsLayout->rowCount() > kMobsGroupOffset) {
        mobsLayout->removeRow(kMobsGroupOffset);
    }

    for (int i = 0; i < m_currentMobBatch->mobs_size(); ++i) {
        addMobRow(i);
    }
}

void MobBatchInspector::clearFields()
{
    m_nameEdit->clear();
    m_delaySpin->setValue(0);
    m_lifespanSpin->setValue(0);

    qDeleteAll(m_deleteMobButtons);
    m_deleteMobButtons.clear();
}

void MobBatchInspector::addMobRow(int mobIndex)
{
    if (!m_currentMobBatch || mobIndex >= m_currentMobBatch->mobs_size()) {
        return;
    }

    QFormLayout *mobsLayout = qobject_cast<QFormLayout *>(m_mobsGroup->layout());

    const auto &mob = m_currentMobBatch->mobs(mobIndex);
    QString mobName = QString::fromStdString(mob.name());
    if (mobName.isEmpty()) {
        mobName = QString("Mob %1").arg(mobIndex + 1);
    }

    QLabel *mobLabel = new QLabel(mobName);

    QPushButton *deleteButton = new QPushButton();
    deleteButton->setText("X");
    deleteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QFontMetrics fm(deleteButton->font());
    int width = fm.horizontalAdvance(deleteButton->text()) + kDeleteButtonWidthPadding;
    deleteButton->setFixedSize(width, deleteButton->sizeHint().height());

    QHBoxLayout *mobLayout = new QHBoxLayout();
    mobLayout->addWidget(mobLabel);
    mobLayout->addWidget(deleteButton);

    mobsLayout->addRow(mobLayout);

    m_deleteMobButtons.append(deleteButton);

    connect(deleteButton, &QPushButton::clicked, this,
            [this, mobIndex]() { onDeleteMobClicked(mobIndex); });
}

void MobBatchInspector::onNameChanged(const QString &text)
{
    if (m_currentMobBatch) {
        m_currentMobBatch->set_name(text.toStdString());
    }
}

void MobBatchInspector::onDelayChanged(double value)
{
    if (m_currentMobBatch) {
        m_currentMobBatch->set_delay_after_previous_batch(static_cast<float>(value));
    }
}

void MobBatchInspector::onLifespanChanged(double value)
{
    if (m_currentMobBatch) {
        m_currentMobBatch->set_lifespan(static_cast<float>(value));
    }
}

void MobBatchInspector::onAddMobClicked()
{
    if (m_currentMobBatch) {
        auto *mob = m_currentMobBatch->add_mobs();
        mob->set_name("New Mob");
        mob->set_mob_prefab_id(0);
        mob->set_spawn_delay(0);
        updateMobsList();
    }
}

void MobBatchInspector::onDeleteMobClicked(int mobIndex)
{
    if (!m_currentMobBatch || mobIndex >= m_currentMobBatch->mobs_size()) {
        return;
    }

    auto *mobs = m_currentMobBatch->mutable_mobs();
    mobs->DeleteSubrange(mobIndex, 1);
    updateMobsList();
}
