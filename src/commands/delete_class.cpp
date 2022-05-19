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
	elem->Reliquish();
	scene->removeItem(u);
	elem.reset((Class*)u);
}
