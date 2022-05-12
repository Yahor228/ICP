#include <commands/delete_class.h>
#include <QGraphicsScene>
#include <Class.h>

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
}

void DeleteClassCommand::redo()
{
	scene->removeItem(u);
	elem.reset((Class*)u);
}
