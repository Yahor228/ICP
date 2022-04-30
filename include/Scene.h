#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonObject> 

class Node;

class Scene : public QGraphicsScene
{
public:
	Scene() = default;
public:
	void LoadFrom(QJsonObject doc);
	const auto& Nodes()const noexcept {
		return nodes;
	}
private:
	std::unordered_map<std::u16string_view, Node*> nodes;
};