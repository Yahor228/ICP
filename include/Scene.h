#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonObject> 

class Scene : public QGraphicsScene
{
public:
	Scene() = default;
public:
	void LoadFrom(QJsonObject doc);
};