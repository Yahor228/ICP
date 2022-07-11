/**
 * @file add_connection.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for creation a new connection
 */


#pragma once
#include <QUndoCommand>
#include <memory>


class Connection;
class QGraphicsItem;
class QGraphicsScene;


/// @brief class for creation a new connection
class AddConnectionCommand : public QUndoCommand
{
public:
    AddConnectionCommand(QGraphicsScene* scene, Connection* u);
    ~AddConnectionCommand();
private:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Connection> elem;
    QGraphicsScene* scene;
    Connection* u;
};