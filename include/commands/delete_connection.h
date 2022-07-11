/**
 * @file delete_connection.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for remove a connection in CD
 */



#pragma once
#include <QUndoCommand>
#include <memory>


class Connection;
class QGraphicsItem;
class QGraphicsScene;


class DeleteConnectionCommand : public QUndoCommand
{
public:
    DeleteConnectionCommand(QGraphicsScene* scene, Connection* u);
    ~DeleteConnectionCommand();
private:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Connection> elem;
    QGraphicsScene* scene;
    Connection* u;
};