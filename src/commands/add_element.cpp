#include <commands/add_element.h>
#include <QGraphicsScene>
#include <sequence/Element.h>

AddElementCommand::AddElementCommand(QGraphicsScene* scene, Element* u, QPointF pos)
	:u(u), scene(scene)
{
	u->setPos(pos);
	elem.reset(u);
}

AddElementCommand::~AddElementCommand()
{
}

void AddElementCommand::undo()
{
	scene->removeItem(u);
	elem.reset(u);
}

void AddElementCommand::redo()
{
	scene->addItem(elem.release());
}


RemoveElementCommand::RemoveElementCommand(QGraphicsScene* scene, Element* u)
	:u(u), scene(scene)
{
}

RemoveElementCommand::~RemoveElementCommand()
{
}

void RemoveElementCommand::undo()
{
	scene->addItem(elem.release());
	u->Reconnect();
}

void RemoveElementCommand::redo()
{
	scene->removeItem(u);
	elem.reset(u);
	elem->Relinquish();
}
