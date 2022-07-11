#include <sequence/Call.h>
#include <sequence/Element.h>
#include <util/util.h>
#include <model/node.h>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <commands/add_call.h>
#include <commands/commandstack.h>






Call::Call(desc d)
	:d(d)
{
	setFlag(ItemIsSelectable);
	setZValue(2.0);
}

QRectF Call::boundingRect() const
{
	constexpr qreal adj = 10.0f;
	auto x1 = d.from->geometry().center().x();
	auto x2 = d.to->geometry().center().x();
	return QRectF{ QPointF{x1, d.start}, QPointF{x2, d.start + d.length} }.normalized().adjusted(-adj, -adj, adj, adj);
}

void Call::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(QPen{ isSelected()? Qt::yellow :Qt::white, 2.0 });
	painter->setBrush(Qt::darkCyan);

	auto x1 = d.from->geometry().center().x();
	auto x2 = d.to->geometry().center().x();

	auto rc = QRectF{ QPointF{x2, d.start}, QPointF{x2,  d.start + d.length} };
	painter->drawRect(rc.adjusted(-5.0, 0.0, 5.0, 0.0));

	auto line = QLineF{ {x1, d.start}, QPointF{x2,d.start} };
	painter->drawText(line.center() - QPointF{ 0.0,4.0 }, d.func);
	painter->drawLine(line);

	if (d.ty == Type::t_async)
		return;

	painter->setPen(QPen{ isSelected() ? Qt::yellow : Qt::white, 2.0 , Qt::DashLine});
	line.translate(0.0, d.length);
	painter->drawLine(line);
}

void Call::Update()
{
	prepareGeometryChange();
	update();
}

void Call::Connect()
{
	d.from->AddTo(this);
	d.to->AddFrom(this);
}

void Call::Disconnect()
{
	d.from->RemoveTo(this);
	d.to->RemoveFrom(this);
}

qreal Call::BottomPoint() const noexcept
{
	return { d.start + d.length };
}

void Call::UnbindFrom()
{
	d.from->RemoveTo(this);
	scene()->removeItem(this);
}

void Call::UnbindTo()
{
	d.to->RemoveFrom(this);
	scene()->removeItem(this);
}

void Call::BindFrom(QGraphicsScene* scene)
{
	d.from->AddTo(this);
	scene->addItem(this);
}

void Call::BindTo(QGraphicsScene* scene)
{
	d.to->AddFrom(this);
	scene->addItem(this);
}

void Call::Save(QJsonObject& o) const
{
	QJsonArray a;
	a.append(d.from->UID());
	a.append(d.to->UID());
	a.append(d.func);
	a.append(int(d.ty));
	a.append(int(d.start));
	a.append(int(d.length));
	o.insert(qsl("A"), a);
}



//////////////////////////////////////////////////////////////////////////
CallCreator::CallCreator(Element* from, QPointF start)
	:from(from), start(from->geometry().center().x(), start.y()), pos(start.x())
{

}

QRectF CallCreator::boundingRect() const
{
	constexpr qreal adj = 10.0f;
	switch (st)
	{
	case CallCreator::s_line:
		return QRectF{ start, QPointF{pos,start.y()} }.normalized().adjusted(-adj, -adj, adj, adj);
	case CallCreator::s_rect:
		return QRectF{ start, QPointF{pos,posy} }.normalized().adjusted(-adj, -adj, adj, adj);
	default:
		break;
	}
}

void CallCreator::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(QPen{ Qt::white, 2.0, Qt::DashDotLine });

	switch (st)
	{
	case CallCreator::s_rect:
	{
		painter->setPen(QPen{ Qt::white, 2.0 });
		painter->setBrush(Qt::darkCyan);

		auto rc = QRectF{ QPointF{pos, start.y()}, QPointF{pos, posy} };
		painter->drawRect(rc.adjusted(-5.0, 0.0, 5.0, 0.0));
		painter->drawText(QLineF{ start, QPointF{pos, start.y()} }.center() - QPointF{ 0.0,4.0 }, func);
	}
	[[fallthrough]];
	case CallCreator::s_line:
		painter->drawLine({ start, QPointF{pos,start.y()} });
		break;
	default:
		break;
	}

}

void CallCreator::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	prepareGeometryChange();
	switch (st)
	{
	case CallCreator::s_line:
		pos = event->scenePos().x();
		break;
	case CallCreator::s_rect:
		posy = std::max(event->scenePos().y(), start.y());
		break;
	default:
		break;
	}
	update();
	event->accept();
}

void CallCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	ungrabMouse();
	event->accept();

	switch (st)
	{
	case CallCreator::s_line:
		return LineEvent(event);
	case CallCreator::s_rect:
		return RectEvent(event);
	default:
		break;
	}
}

void CallCreator::SetConnection()
{
	st = state::s_rect;
	pos = to->geometry().center().x();
	posy = start.y();
	grabMouse();
}

void CallCreator::ProduceConnection()
{
	auto* c = new Call{ {from, to, std::move(func), ty, start.y(), posy - start.y()} };
	CommandStack::current().push(new AddCallCommand(scene(), c));
	delete this;
}

void CallCreator::LineEvent(QGraphicsSceneMouseEvent* event)
{
	for (auto* x : scene()->items(QPointF(pos, start.y()), Qt::IntersectsItemBoundingRect))
	{
		if (auto* c = dynamic_cast<Element*>(x))
		{
			to = c;

			QMenu menu;
			menu.addAction(qsl("Create"), [this]() {func = qsl("<<create>>"); ty = ::Type::t_create; SetConnection(); });
			menu.addAction(qsl("Delete"), [this]() {func = qsl("<<delete>>"); ty = ::Type::t_delete; SetConnection(); });
			auto methods = to->GetNode()->Methods();

			bool b = from->GetNode()->DerivedFrom(*to->GetNode());
			for (auto& i : methods)
				if (b || i->acc[0] == '+')
					menu.addAction(i->Name, [this, i]() {func = i->Name; SetConnection(); });

			menu.exec(event->screenPos());
			return;
		}
	}
	delete this;
}

void CallCreator::RectEvent(QGraphicsSceneMouseEvent* event)
{
	if (ty != ::Type::t_none)
		return ProduceConnection();
	QMenu menu;
	menu.addAction(qsl("Sync"), [this]() {ty = ::Type::t_sync; });
	menu.addAction(qsl("Async"), [this]() {ty = ::Type::t_async; });
	menu.exec(event->screenPos());

	if (ty != ::Type::t_none)
		return ProduceConnection();
	delete this;
}

