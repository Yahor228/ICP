#include <commands/add.h>
#include <sequence/element.h>



AddCommand::AddCommand(Diagram& d, const QString& header)
	:diag(d), elem(std::make_unique<Element>(header))
{
}

AddCommand::~AddCommand()
{
}

void AddCommand::undo()
{
	elem = diag.remove(pos);
}

void AddCommand::redo()
{
	diag.append(std::move(elem));
	pos = diag.count() - 1;
}
