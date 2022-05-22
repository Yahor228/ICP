#include <class/connection.h>
#include <class/Class.h>
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <array>
#include <numbers>
#include <commands/commandstack.h>
#include <commands/add_connection.h>
#include <util/util.h>

#include <ui/Log.h>


Connection::Connection(Class* from, Class* to, Type ty)
	:from(from), to(to), ty(ty), self(uint8_t(from == to ? from->SelfConnected() : 0))
{
	setFlag(ItemIsSelectable);
}

void Connection::CalculatePointsSelf(std::array<QPointF, 5>& poly)
{
	auto g1 = from->geometry();
	std::array<QLineF, 4> lines1{
		QLineF{g1.topLeft(), g1.topRight()},
		QLineF{g1.topLeft(), g1.bottomLeft()},
		QLineF{g1.topRight(), g1.bottomRight()},
		QLineF{g1.bottomRight(), g1.bottomLeft()} };

	switch (self)
	{
	default:
	case 0:
		poly[0] = lines1[2].center() + QPointF{ 0.0f,-5.0f };
		poly[4] = lines1[0].center() + QPointF{ 5.0f,0.0f };
		poly[1] = poly[0] + QPointF{ 30.0f,0.0f };
		poly[3] = poly[4] + QPointF{ 0.0f,-30.0f };
		poly[2] = { poly[1].x(), poly[3].y() };
		break;
	case 1:
		poly[0] = lines1[0].center() + QPointF{ -5.0f , 0.0f };
		poly[4] = lines1[1].center() + QPointF{ 0.0f,-5.0f };
		poly[1] = poly[0] + QPointF{ 0.0f,-30.0f };
		poly[3] = poly[4] + QPointF{ -30.0f, 0.0f };
		poly[2] = { poly[3].x(), poly[1].y() };
		break;
	case 2:
		poly[0] = lines1[1].center() + QPointF{ 0.0f,5.0f };
		poly[4] = lines1[3].center() + QPointF{ -5.0f , 0.0f };
		poly[1] = poly[0] + QPointF{ -30.0f,0.0f };
		poly[3] = poly[4] + QPointF{ 0.0f, 30.0f };
		poly[2] = { poly[1].x(), poly[3].y() };
		break;
	case 3:
		poly[0] = lines1[3].center() + QPointF{ 5.0f , 0.0f };
		poly[4] = lines1[2].center() + QPointF{ 0.0f, 5.0f };
		poly[1] = poly[0] + QPointF{ 0.0f,30.0f };
		poly[3] = poly[4] + QPointF{ 30.0f, 0.0f };
		poly[2] = { poly[3].x(), poly[1].y() };
		break;
	}
}

void Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	if (from == to)
		return PaintSelf(painter);
	if (from->collidesWithItem(to))
		return;
	PaintNormal(painter);
}

void Connection::PaintNormal(QPainter* painter)
{
	auto g1 = from->geometry();
	auto g2 = to->geometry();
	auto c1 = g1.center();
	auto c2 = g2.center();

	QLineF a(c1, c2);

	std::array<QLineF, 4> lines1{
		QLineF{g1.topLeft(), g1.topRight()},
		QLineF{g1.topLeft(), g1.bottomLeft()},
		QLineF{g1.topRight(), g1.bottomRight()},
		QLineF{g1.bottomRight(), g1.bottomLeft()} };
	std::array<QLineF, 4> lines2{
		QLineF{g2.topLeft(), g2.topRight()},
		QLineF{g2.topLeft(), g2.bottomLeft()},
		QLineF{g2.topRight(), g2.bottomRight()},
		QLineF{g2.bottomRight(), g2.bottomLeft()} };

	QPointF int1;
	QPointF int2;
	for (auto& l : lines1)
		if (a.intersects(l, &int1) == QLineF::IntersectType::BoundedIntersection)break;
	for (auto& l : lines2)
		if (a.intersects(l, &int2) == QLineF::IntersectType::BoundedIntersection)break;


	QColor c{ Qt::white };
	if (isSelected())
		c = Qt::yellow;

	painter->setPen({ c, 3.0f });

	QPainterPath p;
	p.moveTo(int1);
	p.lineTo(int2);
	setPath(p);
	painter->drawPath(p);
	DrawPolygon(painter, { int1 , int2 });
}

void Connection::PaintSelf(QPainter* painter)
{
	std::array<QPointF, 5> poly{};
	CalculatePointsSelf(poly);

	QColor c{ Qt::white };
	if (isSelected())
		c = Qt::yellow;

	painter->setPen({ c, 3.0f });

	QPainterPath p;
	p.moveTo(poly[0]);
	for (size_t i = 1; i < 5; i++)
	{
		p.lineTo(poly[i]);
	}
	setPath(p);
	painter->drawPath(p);
	DrawPolygon(painter, { poly[4],  poly[3] });
}

bool Connection::ValidateAgainst(Class* xfrom)const
{
	if (ty != Type::gener && ty!= Type::comp)return true;
	if (xfrom == to)return false;
	return to->ValidateConnection(xfrom);
}


void Connection::Save(QJsonObject& o) const
{
	QJsonArray arr;
	arr.append(from->Alias());
	arr.append(to->Alias());
	arr.append(to_string(ty));
	o.insert("A", arr);
}

QRectF Connection::boundingRect() const
{
	constexpr qreal extra = 10.0f;
	return QGraphicsPathItem::boundingRect().adjusted(-extra, -extra, extra, extra);
}

void Connection::DrawPolygon(QPainter* painter, const QLineF& line)
{
	QPolygonF head;
	auto arrowSize = 10.0;
	double angle = std::atan2(-line.dy(), line.dx());
	QPalette p;
	painter->setBrush(p.color(QPalette::Base));


	switch (ty)
	{
	case Connection::Type::none:
	case Connection::Type::asoc:
		return;
	case Connection::Type::comp:
		painter->setBrush(Qt::white);
		[[fallthrough]];
	case Connection::Type::aggr:
	{
		QPointF arrowP1 = line.p1() + QPointF(sin(angle + std::numbers::pi / 3) * arrowSize,
			cos(angle + std::numbers::pi / 3) * arrowSize);

		QPointF arrowP3 = line.p1() + QPointF(sin(angle + 2 * std::numbers::pi / 3) * arrowSize,
			cos(angle + 2 * std::numbers::pi / 3) * arrowSize);

		QPointF arrowP2 = line.p1() + QPointF(sin(angle + std::numbers::pi / 2) * 2 * arrowSize,
			cos(angle + std::numbers::pi / 2) * 2 * arrowSize);

		head.clear();
		head << line.p1() << arrowP1 << arrowP2 << arrowP3;
		break;
	}
	case Connection::Type::gener:
	{
		QPointF arrowP1 = line.p1() + QPointF(sin(angle + std::numbers::pi / 3) * arrowSize,
			cos(angle + std::numbers::pi / 3) * arrowSize);

		QPointF arrowP2 = line.p1() + QPointF(sin(angle + 2 * std::numbers::pi / 3) * arrowSize,
			cos(angle + 2 * std::numbers::pi / 3) * arrowSize);

		head.clear();
		head << line.p1() << arrowP1 << arrowP2;
		break;
	}
	default:
		break;
	}

	painter->drawPolygon(head);
}


void Connection::UnbindFrom()
{
	from->DisconnectTo(this);
	scene()->removeItem(this);
}
void Connection::UnbindTo()
{
	to->DisconnectFrom(this);
	scene()->removeItem(this);
	if (ty == Type::gener)
		to->Model().RemoveConnection(from->Model());
}

void Connection::BindFrom(QGraphicsScene* scene)
{
	from->ConnectTo(this);
	scene->addItem(this);
}
void Connection::BindTo(QGraphicsScene* scene)
{
	to->ConnectFrom(this);
	scene->addItem(this);
	if (ty == Type::gener)
		to->Model().InheritFrom(from->Model());
}

void Connection::Disconnect()
{
	if (from == to)
		return from->DisconnectSelf(this);
	from->DisconnectTo(this);
	to->DisconnectFrom(this);
	if (ty == Type::gener)
		to->Model().RemoveConnection(from->Model());
}
void Connection::Reconnect()
{
	if (from == to)
		return from->ConnectSelf(this);
	from->ConnectTo(this);
	to->ConnectFrom(this);
	if (ty == Type::gener)
		to->Model().InheritFrom(from->Model());
}

bool Connection::Valid() const
{
	return ValidateAgainst(from);
}

///////////////////////////////////////////////////////////////////////

ConnectionCreator::ConnectionCreator(Class* from)
	:from(from)
{
	setPen(QPen(Qt::white, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF ConnectionCreator::boundingRect() const
{
	constexpr qreal extra = 10.0f;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
		line().p2().y() - line().p1().y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}

void ConnectionCreator::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto g1 = from->geometry();
	auto c1 = g1.center();

	QLineF a(c1, pos);

	std::array<QLineF, 4> lines1{
		QLineF{g1.topLeft(), g1.topRight()},
		QLineF{g1.topLeft(), g1.bottomLeft()},
		QLineF{g1.topRight(), g1.bottomRight()},
		QLineF{g1.bottomRight(), g1.bottomLeft()} };

	QPointF int1;
	QPointF int2;
	for (auto& l : lines1)
		if (a.intersects(l, &int1) == QLineF::IntersectType::BoundedIntersection)break;


	QColor c{ Qt::white };
	if (isSelected())
		c = Qt::yellow;
	pen().setColor(c);

	setLine(a);
	painter->setPen(pen());
	painter->setBrush(c);
	painter->drawLine(line());
}

void ConnectionCreator::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	prepareGeometryChange();
	pos = event->scenePos();
	update();
	event->accept();
}



void ConnectionCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	ungrabMouse();
	event->accept();

	for (auto* x : scene()->items(pos))
	{
		if (auto* c = dynamic_cast<Class*>(x))
		{
			Menu::Exec(this, c, event->screenPos());
			Menu::Clear();
			return;
		}
	}
	delete this;
}

void ConnectionCreator::SetConnection(Class* to, Connection::Type t)
{
	std::unique_ptr<Connection> conn{ new Connection(to,from, t) };
	if (!conn->Valid()) {
		Logger::Warn(qsl("The connection from %1 to %2 of type %3 produces cycle and is not created.")
			.arg(to->Alias()).arg(from->Alias()).arg(to_string(t)));
		return;
	}
	CommandStack::current().push(new AddConnectionCommand(scene(), conn.release()));
}

ConnectionCreator::Menu::Menu()
{
	addAction(qsl("Generalization"), [this]() {x->SetConnection(to, Connection::Type::gener); });
	addAction(qsl("Agregation"), [this]() { x->SetConnection(to, Connection::Type::aggr); });
	addAction(qsl("Association"), [this]() {x->SetConnection(to, Connection::Type::asoc); });
	addAction(qsl("Composition"), [this]() {x->SetConnection(to, Connection::Type::comp); });
}
