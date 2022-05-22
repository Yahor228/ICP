#pragma once
#include <QUndoCommand>


class Connection;


class ReverseConnectionCommand : public QUndoCommand
{
public:
    ReverseConnectionCommand(Connection* u);
    ~ReverseConnectionCommand();
private:
    void undo() override;
    void redo() override;
private:
    Connection* u;
};