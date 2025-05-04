#include "command.h"

#include <QDebug>

InspectorAwareCommand::InspectorAwareCommand(Updater updateInspector, QString fieldIndex,
                                             QUndoCommand *parent)
    : QUndoCommand(parent),
      m_updateInspector(std::move(updateInspector)),
      m_fieldIndex(std::move(fieldIndex))
{
}

void InspectorAwareCommand::UpdateInspector()
{
    m_updateInspector();
}

IntUndoCommand::IntUndoCommand(int newValue, Getter getter, Setter setter, QString fieldIndex,
                               std::function<void()> updateInspector, QUndoCommand *parent)
    : InspectorAwareCommand(updateInspector, fieldIndex, parent),
      m_oldValue(getter()),
      m_newValue(newValue),
      m_getter(std::move(getter)),
      m_setter(std::move(setter))
{
    m_setter(m_newValue);
}

void IntUndoCommand::undo()
{
    qDebug() << "undo" << m_oldValue;
    m_setter(m_oldValue);
    UpdateInspector();
}

void IntUndoCommand::redo()
{
    qDebug() << "redo" << m_newValue;
    m_setter(m_newValue);
    UpdateInspector();
}

bool IntUndoCommand::mergeWith(const QUndoCommand *other)
{
    const IntUndoCommand *otherCommand = dynamic_cast<const IntUndoCommand *>(other);

    if (m_fieldIndex != otherCommand->m_fieldIndex) {
        return false;
    }

    m_newValue = otherCommand->m_newValue;
    return true;
}
