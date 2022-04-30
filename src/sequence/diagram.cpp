#include <sequence/diagram.h>
#include <sequence/element.h>
#include <QGraphicsScene>

Diagram::Diagram()
	:hl(new QGraphicsLinearLayout(Qt::Orientation::Horizontal))
{
	setLayout(hl);
}

size_t Diagram::count() const
{
	return hl->count();
}

void Diagram::append(std::unique_ptr<Element> element)
{
	hl->addItem(element.release());
}

void Diagram::insert(std::unique_ptr<Element> element, size_t index)
{
	if (index > hl->count())
		return append(std::move(element));
	hl->insertItem(int(index), element.release());
}

std::unique_ptr<Element> Diagram::remove(size_t index)
{
	auto* x = static_cast<Element*>(hl->itemAt(int(index)));
	if (!x)return nullptr;
	hl->removeItem(x);
	scene()->removeItem(x);
	return std::unique_ptr<Element>{ x };
}
