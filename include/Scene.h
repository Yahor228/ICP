#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonObject> 

class Node;

class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	Scene();
public:
	void LoadFrom(QJsonObject doc);
	const auto& Nodes()const noexcept {
		return nodes;
	}
signals:
	void SelectionChanged(void* node);
private:
	std::unordered_map<std::u16string_view, Node*> nodes;
};
