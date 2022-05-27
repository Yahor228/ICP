#include <commands/add_call.h>
#include <sequence/Call.h>
#include <QGraphicsScene>

AddCallCommand::AddCallCommand(QGraphicsScene* scene, Call* u)
	:u(u), scene(scene), elem(std::unique_ptr<Call>(u))
{
}

AddCallCommand::~AddCallCommand()
{
}

void AddCallCommand::undo()
{
	elem.reset(u);
	elem->Disconnect();
	scene->removeItem(u);
}

void AddCallCommand::redo()
{
	scene->addItem(elem.release());
	u->Connect();
	u->Update();
}

RemoveCallCommand::RemoveCallCommand(QGraphicsScene* scene, Call* u)
	:u(u), scene(scene)
{
}

RemoveCallCommand::~RemoveCallCommand()
{
}

void RemoveCallCommand::undo()
{
	scene->addItem(elem.release());
	u->Connect();
	u->Update();
}

void RemoveCallCommand::redo()
{
	elem.reset(u);
	scene->removeItem(u);
	elem->Disconnect();
}
