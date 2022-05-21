#include <commands/delete_connection.h>
#include <QGraphicsScene>
#include <class/connection.h>

DeleteConnectionCommand::DeleteConnectionCommand(QGraphicsScene* scene, Connection* u)
	:u(u), scene(scene)
{
}

DeleteConnectionCommand::~DeleteConnectionCommand()
{
}

void DeleteConnectionCommand::undo()
{
	scene->addItem(elem.release());
	u->Reconnect();
	u->update();
}

void DeleteConnectionCommand::redo()
{
	elem.reset(u);
	elem->Disconnect();
	scene->removeItem(u);
}
