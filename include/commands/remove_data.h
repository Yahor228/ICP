#pragma once
#include <model/node.h>
#include <QUndoCommand>


class Node;

class RemoveMethodCommand : public QUndoCommand
{
public:
	RemoveMethodCommand(Node* node, size_t index, Node::DataBinder binder);
	~RemoveMethodCommand();
private:
	void undo() override;
	void redo() override;
private:
	Node* node;
	size_t index;
	Node::DataBinder binder;
};

class RemoveDataCommand : public QUndoCommand
{
public:
	RemoveDataCommand(Node* node, size_t index, Node::DataBinder binder);
	~RemoveDataCommand();
private:
	void undo() override;
	void redo() override;
private:
	Node* node;
	size_t index;
	Node::DataBinder binder;
};