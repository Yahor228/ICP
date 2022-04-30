#pragma once
#include <QGraphicsView>

class SceneView:public QGraphicsView
{
public:
	SceneView(QGraphicsScene * scene);
	void SetWidget(QWidget* xw)
	{
		w = xw;
	}
protected:
	void contextMenuEvent(QContextMenuEvent* event)override;
private:
	QWidget* w = nullptr;
};