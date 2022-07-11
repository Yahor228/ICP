/**
 * @file add_class.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration class of "Class Diagram"
 */

#pragma once
#include <QUndoCommand>

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