#pragma once
#include <QGraphicsScene>
#include <QMenu>
#include <optional>
#include <span>
#include <ISave.h>
#include <ISelectable.h>

class Scene;
class Node;
class Element;

class SeqScene : public QGraphicsScene, public ISave
{
	Q_OBJECT
public:
	SeqScene();
public:
	auto GetClass()const { return class_scene; }
	void SetClass(std::shared_ptr<Scene> c) { class_scene = std::move(c); }
	void RemoveSelected();
	virtual void Save(QJsonObject& o)const override;
	void LoadFrom(QJsonObject doc);
private:
	std::span<Node* const> GetNodes()const noexcept;
	void CreateConnection(Element* element);
	void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)override;
	void ReviseMenu();
signals:
	void SelectionChanged(ISelectable* sel);
private:
	std::shared_ptr<Scene> class_scene;
	std::optional<QMenu> menu;
	QPointF pos;
};