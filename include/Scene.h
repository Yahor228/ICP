#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonObject> 
#include <QMenu> 
#include <ISave.h>

class Node;
class Class;

class Scene : public QGraphicsScene, public ISave
{
	Q_OBJECT
public:
	Scene();
public:
	void RemoveSelected();
	void LoadFrom(QJsonObject doc);
	const auto& Nodes()const noexcept {
		return nodes;
	}
	void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)override;
	void CreateConnection(Class* c);
	void Save(QJsonObject& doc)const override;
signals:
	void SelectionChanged(void* node);
private:
	std::unordered_map<std::u16string_view, Node*> nodes;
	QMenu context;
	QPointF pos;
};
