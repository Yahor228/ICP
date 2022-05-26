#include <sequence/SeqScene.h>
#include <sequence/Element.h>
#include <sequence/Call.h>
#include <Scene.h>
#include <QGraphicsSceneMouseEvent>
#include <model/node.h>
#include <util/util.h>

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
		menu->addAction(name, [this, name]() {
			auto* element = new Element(name);
			element->setPos(pos);
			addItem(element);
			});
	}
}
