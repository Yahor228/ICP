#include <sequence/SeqScene.h>
#include <sequence/Element.h>
#include <sequence/Call.h>
#include <Scene.h>
#include <QGraphicsSceneMouseEvent>
#include <model/node.h>
#include <util/util.h>
#include <commands/commandstack.h>
#include <commands/add_call.h>
#include <commands/add_element.h>


void SeqScene::RemoveSelected()
{
	for (auto* i : selectedItems())
	{
		if (auto* x = dynamic_cast<Call*>(i))
			CommandStack::current().push(new RemoveCallCommand(this, x));
		else if (auto* x = dynamic_cast<Element*>(i))
			CommandStack::current().push(new RemoveElementCommand(this, x));
	}
}

std::span<Node* const> SeqScene::GetNodes() const noexcept
{
	return class_scene->Nodes();
}

void SeqScene::CreateConnection(Element* element)
{
	auto cc = new CallCreator(element, pos);
	addItem(cc);
	cc->grabMouse();
}

void SeqScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if (mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		pos = mouseEvent->scenePos();
		ReviseMenu();
		menu->popup(mouseEvent->screenPos());
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void SeqScene::ReviseMenu()
{
	if (class_scene->IsImmutable())return;
	menu.emplace();
	menu->addAction(qsl("Connect"), [this]() {
		for (auto* x : items(pos, Qt::IntersectsItemBoundingRect))
		{
			if (auto* c = dynamic_cast<Element*>(x))
				return CreateConnection(c);
		}
		});
	for (auto& i : GetNodes())
	{
		auto& name = i->Name();
		menu->addAction(name, [this, i]() {
			CommandStack::current().push(new AddElementCommand(this, new Element(i), pos));
			});
	}
}
