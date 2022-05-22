#include <commands/reverse_connection.h>
#include <class/connection.h>

ReverseConnectionCommand::ReverseConnectionCommand(Connection* u)
	:u(u)
{
}

ReverseConnectionCommand::~ReverseConnectionCommand()
{
}

void ReverseConnectionCommand::undo()
{
	u->Reverse();
}

void ReverseConnectionCommand::redo()
{
	undo();
}
