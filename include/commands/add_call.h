/**
 * @file add_connection.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for creation a new connection
 */


#pragma once
#include <QUndoCommand>
#include <memory>


class Call;
class QGraphicsItem;
class QGraphicsScene;


/// @brief class for creation a new connection
class AddCallCommand : public QUndoCommand
{
public:
    AddCallCommand(QGraphicsScene* scene, Call* u);
    ~AddCallCommand();
private:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Call> elem;
    QGraphicsScene* scene;
    Call* u;
};

class RemoveCallCommand : public QUndoCommand
{
public:
    RemoveCallCommand(QGraphicsScene* scene, Call* u);
    ~RemoveCallCommand();
private:
    void undo() override;
    void redo() override;
private:
    std::unique_ptr<Call> elem;
    QGraphicsScene* scene;
    Call* u;
};