#pragma once
#include <QUndoCommand>


class Class;
class QGraphicsItem;
class QGraphicsScene;


class DeleteClassCommand : public QUndoCommand
{
public:
	DeleteClassCommand(QGraphicsScene* scene, QGraphicsItem* u);
	~DeleteClassCommand();
private:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Class> elem;
    QGraphicsScene* scene;
    QGraphicsItem* u;
};