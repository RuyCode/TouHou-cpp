#ifndef COMMAND_H
#define COMMAND_H

#include "game.pb.h"

#include <QUndoCommand>

class InspectorAwareCommand : public QUndoCommand
{
    using Updater = std::function<void()>;

public:
    InspectorAwareCommand(Updater updateInspector, QString filedIndex, QUndoCommand *parent = nullptr);

    void UpdateInspector();

protected:
    Updater m_updateInspector;
    QString m_fieldIndex;
};

class IntUndoCommand : public InspectorAwareCommand
{
    using Getter = std::function<int()>;
    using Setter = std::function<void(int)>;

public:
    IntUndoCommand(int newValue, Getter getter, Setter setter, QString filedIndex,
                   std::function<void()> updateInspector, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    bool mergeWith(const QUndoCommand *other) override;

private:
    int m_oldValue;
    int m_newValue;
    Getter m_getter;
    Setter m_setter;
};

// class DoubleUndoCommand : public InspectorAwareCommand
// {
// public:
//     DoubleUndoCommand(float *mobValue, double newValue, std::function<void()> updateInspector,
//                       QUndoCommand *parent = nullptr);

//     void undo() override;
//     void redo() override;

//     bool mergeWith(const QUndoCommand *other) override;

// private:
//     double m_oldValue;
//     double m_newValue;
//     float *m_mobValue;
// };

// class StringUndoCommand : public InspectorAwareCommand
// {
// public:
//     StringUndoCommand(game::Mob *mobValue, QString newValue, std::function<void()>
//     updateInspector,
//                       QUndoCommand *parent = nullptr);

//     void undo() override;
//     void redo() override;

//     bool mergeWith(const QUndoCommand *other) override;

// private:
//     QString m_oldValue;
//     QString m_newValue;
//     std::string *m_mobValue;
// };

#endif // COMMAND_H
