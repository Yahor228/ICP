#pragma once
#include <QUndoCommand>
#include <sequence/diagram.h>

class QGraphicsScene;
class Element;

class AddCommand : public QUndoCommand
{
public:
    AddCommand(Diagram& d, const QString& header);
    ~AddCommand();
public:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Element> elem;
    Diagram& diag;
    size_t pos = 0;
};