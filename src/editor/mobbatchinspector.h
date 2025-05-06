// mobbatchinspector.h
#ifndef MOB_BATCH_INSPECTOR_H
#define MOB_BATCH_INSPECTOR_H

#include <QWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QList>
#include <QUndoStack>
#include <QPushButton>
#include <QLabel>

#include "game.pb.h"

class MobBatchInspector : public QWidget
{
    Q_OBJECT

public:
    explicit MobBatchInspector(QUndoStack *undoStack, QWidget *parent = nullptr);
    void setMobBatch(game::MobBatch *mobBatch);
    void clear();

private:
    void setupUI();
    void updateFields();
    void updateMobsList();
    void clearFields();
    void addMobRow(int mobIndex);

private slots:
    void onNameChanged(const QString &text);
    void onDelayChanged(double value);
    void onLifespanChanged(double value);
    void onAddMobClicked();
    void onDeleteMobClicked(int mobIndex);

private:
    // UI Elements
    QGroupBox *m_basicGroup;
    QLineEdit *m_nameEdit;
    QDoubleSpinBox *m_delaySpin;
    QDoubleSpinBox *m_lifespanSpin;

    QGroupBox *m_mobsGroup;
    QPushButton *m_addMobButton;
    QList<QPushButton*> m_deleteMobButtons;

    game::MobBatch *m_currentMobBatch;
    QUndoStack *m_undoStack;
};

#endif // MOB_BATCH_INSPECTOR_H
