/**
 * @file add_connection.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief implementation of class for creation a new connection
 */



#include <commands/add_connection.h>
#include <QGraphicsScene>
#include <class/connection.h>

AddConnectionCommand::AddConnectionCommand(QGraphicsScene* scene, Connection* u)
	:u(u), scene(scene), elem(std::unique_ptr<Connection>(u))
{
}

AddConnectionCommand::~AddConnectionCommand()
{
}

void AddConnectionCommand::undo()
{
	elem.reset(u);
	elem->Disconnect();
	scene->removeItem(u);
}

void AddConnectionCommand::redo()
{
	scene->addItem(elem.release());
	u->Reconnect();
}
