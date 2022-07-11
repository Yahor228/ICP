/**
 * @file add_class.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief implementation class of "Class Diagram"
 */



#include <commands/add_class.h>
#include <class/Class.h>
#include <QGraphicsScene>

AddClassCommand::AddClassCommand(QGraphicsScene* scene, QPointF pos)
	:elem(std::make_unique<Class>()), scene(scene)
{
	elem->setPos(pos);
	u = elem.get();
}

AddClassCommand::~AddClassCommand()
{
}

void AddClassCommand::undo()
{
	scene->removeItem(u);
	elem.reset((Class*)u);
}

void AddClassCommand::redo()
{
	scene->addItem(elem.release());
}
