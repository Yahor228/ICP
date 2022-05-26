#include <sequence/SeqScene.h>
#include <sequence/Element.h>
#include <Scene.h>
#include <QGraphicsSceneMouseEvent>
#include <model/node.h>

std::span<Node* const> SeqScene::GetNodes() const noexcept
{
	return class_scene->Nodes();
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

	for (auto& i : GetNodes())
	{
		auto& name = i->Name();
		menu->addAction(name, [this, name]() {
			addItem(new Element(name));
			});
	}
}
