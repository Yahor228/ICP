#pragma once
#include <QUndoCommand>
#include <memory>


class Connection;
class QGraphicsItem;
class QGraphicsScene;


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