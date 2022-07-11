#include <SceneView.h>
#include <QApplication>

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
