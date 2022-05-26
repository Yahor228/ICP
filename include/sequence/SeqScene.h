#pragma once
#include <QGraphicsScene>
#include <QMenu>
#include <optional>
#include <span>

class Scene;
class Node;

class SeqScene : public QGraphicsScene
{
public:
	SeqScene() = default;
public:
	auto GetClass()const { return class_scene; }
	void SetClass(std::shared_ptr<Scene> c) { class_scene = std::move(c); }
private:
	std::span<Node* const> GetNodes()const noexcept;
	void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)override;
	void ReviseMenu();
private:
	std::shared_ptr<Scene> class_scene;
	std::optional<QMenu> menu;
	QPointF pos;
};