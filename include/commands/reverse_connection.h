/**
 * @file reverse_connection.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for remove a connection in CD
 */


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