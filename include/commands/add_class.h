#pragma once
#include <QUndoCommand>
#include <sequence/diagram.h>

class QGraphicsScene;
class Class;

class AddClassCommand : public QUndoCommand
{
public:
    AddClassCommand(QGraphicsScene* scene, QPointF pos);
    ~AddClassCommand();
public:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Class> elem;
    QGraphicsScene* scene;
    QGraphicsItem* u;
};