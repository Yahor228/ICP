#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonObject> 

class Scene
{
public:
	Scene(QJsonObject doc);
public:
	QWidget* Widget() { return &view; }
private:
	QGraphicsScene scene;
	QGraphicsView view;
};