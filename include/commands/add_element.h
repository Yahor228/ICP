/**
 * @file add_class.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration class of "Class Diagram"
 */

#pragma once
#include <QUndoCommand>
#include <memory>

class QGraphicsScene;
class Element;

class AddElementCommand : public QUndoCommand
{
public:
    AddElementCommand(QGraphicsScene* scene, Element* u, QPointF pos);
    ~AddElementCommand();
public:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Element> elem;
    QGraphicsScene* scene;
    Element* u;
};
class RemoveElementCommand : public QUndoCommand
{
public:
    RemoveElementCommand(QGraphicsScene* scene, Element* u);
    ~RemoveElementCommand();
public:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Element> elem;
    QGraphicsScene* scene;
    Element* u;
};