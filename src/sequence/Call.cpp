#include <sequence/Call.h>
#include <sequence/Element.h>
#include <util/util.h>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

CallCreator::CallCreator(Element* from, QPointF start)
	:from(from), start(start)
{}

QRectF CallCreator::boundingRect() const
{
	constexpr qreal adj = 10.0f;
	return QRectF{ start, QPointF{pos,start.y()} }.adjusted(-adj, -adj, adj, adj);
}

void CallCreator::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(QPen{ Qt::white, 2.0, Qt::DashDotLine });
	painter->drawLine({ start, QPointF{pos,start.y()} });
}

void CallCreator::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	prepareGeometryChange();
	pos = event->scenePos().x();
	update();
	event->accept();
}

void CallCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	ungrabMouse();
	event->accept();

	for (auto* x : scene()->items(QPointF(pos, start.y()), Qt::IntersectsItemBoundingRect))
	{
		if (auto* c = dynamic_cast<Element*>(x))
		{
			to = c;
			menu.addAction(qsl("Sync"));
			menu.addAction(qsl("Async"));
			menu.addAction(qsl("Create"));
			menu.addAction(qsl("Delete"));
			menu.exec(event->screenPos());
			break;
		}
	}
	delete this;
}

void CallCreator::SetConnection(Element* to, Call::Type t)
{
}
