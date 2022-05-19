#include <ui/UIVisitor.h>
#include <class/Class.h>

void UIVisitor::InsertMethod(STy acc, STy name)
{
	node.AppendMethod(acc, name, mode == Mode::inherited);
}

void UIVisitor::InsertData(STy acc, STy name)
{
	node.AppendData(acc, name, mode == Mode::inherited);
}
