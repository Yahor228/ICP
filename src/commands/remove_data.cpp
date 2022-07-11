#include <commands/remove_data.h>

RemoveMethodCommand::RemoveMethodCommand(Node* node, size_t index, Node::DataBinder binder)
	:node(node), index(index), binder(std::move(binder))
{
}

RemoveMethodCommand::~RemoveMethodCommand()
{
}

void RemoveMethodCommand::undo()
{
	node->InsertMethod(binder, index);
	node->Reselect();
}

void RemoveMethodCommand::redo()
{
	node->RemoveMethod(index);
	node->Reselect();
}

RemoveDataCommand::RemoveDataCommand(Node* node, size_t index, Node::DataBinder binder)
	:node(node), index(index), binder(std::move(binder))
{
}

RemoveDataCommand::~RemoveDataCommand()
{
}

void RemoveDataCommand::undo()
{
	node->InsertData(binder, index);
	node->Reselect();
}

void RemoveDataCommand::redo()
{
	node->RemoveData(index);
	node->Reselect();
}
