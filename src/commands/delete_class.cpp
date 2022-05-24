/**
 * @file delete_class.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief implementation of class for remove a class in CD
 */



#include <commands/delete_class.h>
#include <QGraphicsScene>
#include <class/Class.h>

DeleteClassCommand::DeleteClassCommand(QGraphicsScene* scene, QGraphicsItem* u)
	:u(u), scene(scene)
{
}

DeleteClassCommand::~DeleteClassCommand()
{
}

void DeleteClassCommand::undo()
{
	scene->addItem(elem.release());
	((Class*)u)->Reconnect();
}

void DeleteClassCommand::redo()
{
	scene->removeItem(u);
	elem.reset((Class*)u);
	elem->Reliquish();
}
