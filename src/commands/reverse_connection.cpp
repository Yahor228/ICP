/**
 * @file reverse_connection.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief implementation of class for remove a connection in CD
 */

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
