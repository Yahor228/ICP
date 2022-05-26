/**
 * @file Scene.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration a main scene
 */


#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonObject> 
#include <QMenu> 
#include <ISave.h>
#include <ISelectable.h>

class Node;
class Class;

class Scene : public QGraphicsScene, public ISave
{
	Q_OBJECT
public:
	Scene();
public:
	void RemoveSelected();
	void ReverseSelected();
	void LoadFrom(QJsonObject doc);
	const auto& Nodes()const noexcept {
		return nodes;
	}
	void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)override;
	void CreateConnection(Class* c);
	void Save(QJsonObject& doc)const override;
	bool IsImmutable()const noexcept { return immutable; }
	void SetImmutable()noexcept { immutable = true; }
signals:
	void SelectionChanged(ISelectable* node);
private:
	std::vector<Node*> nodes;
	QMenu context;
	QPointF pos;
	bool immutable = false;
};
