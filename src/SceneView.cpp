#include <SceneView.h>
#include <QApplication>
#include <event.h>

SceneView::SceneView(QGraphicsScene* scene)
	:QGraphicsView(scene)
{
	setRenderHints(QPainter::Antialiasing |
		QPainter::TextAntialiasing |
		QPainter::SmoothPixmapTransform);

	setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setCacheMode(QGraphicsView::CacheBackground);
}

void SceneView::contextMenuEvent(QContextMenuEvent* event)
{
	if(w)
	QApplication::postEvent(w, new Event(*event, mapToScene(event->pos())));
}
