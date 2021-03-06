#include <connection.h>
#include <Class.h>
#include <QPen>
#include <QPainter>
#include <array>
#include <numbers>

void Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	if (from->collidesWithItem(to))
		return;

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


	QPen myPen = pen();
	myPen.setColor(Qt::white);
	painter->setPen(myPen);
	painter->setBrush(Qt::white);

	setLine(QLineF(int1, int2));
	painter->drawLine(line());
	DrawPolygon(painter);
}

void Connection::ApplyConnection()
{
	auto& sm = from->Model();
	auto& fm = to->Model();

	switch (ty)
	{
	case Connection::Type::aggr:
		fm.aggregates.push_back(&sm);
		break;
	case Connection::Type::asoc:
		break;
	case Connection::Type::comp:
		fm.composes.push_back(&sm);
		break;
	case Connection::Type::gener:
		fm.methods.insert(sm.methods.begin(), sm.methods.end());
		break;
	default:
		break;
	}
}

Connection::Connection(Class* from, Class* to, Type ty)
	:from(from), to(to), ty(ty)
{
	setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	ApplyConnection();
}

void Connection::updatePosition()
{
	QLineF line(mapFromItem(from, 0, 0), mapFromItem(to, 0, 0));
	setLine(line);
	setZValue(0.0);
}

QRectF Connection::boundingRect() const
{
	qreal extra = (pen().width() + 20) / 2.0;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
		line().p2().y() - line().p1().y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}

void Connection::DrawPolygon(QPainter* painter)
{
	QPolygonF head;
	auto arrowSize = 10.0;
	double angle = std::atan2(-line().dy(), line().dx());
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
		QPointF arrowP1 = line().p1() + QPointF(sin(angle + std::numbers::pi / 3) * arrowSize,
			cos(angle + std::numbers::pi / 3) * arrowSize);

		QPointF arrowP3 = line().p1() + QPointF(sin(angle + 2 * std::numbers::pi / 3) * arrowSize,
			cos(angle + 2 * std::numbers::pi / 3) * arrowSize);

		QPointF arrowP2 = line().p1() + QPointF(sin(angle + std::numbers::pi / 2) * 2 * arrowSize,
			cos(angle + std::numbers::pi / 2) * 2 * arrowSize);

		head.clear();
		head << line().p1() << arrowP1 << arrowP2 << arrowP3;
		break;
	}
	case Connection::Type::gener:
	{
		QPointF arrowP1 = line().p1() + QPointF(sin(angle + std::numbers::pi / 3) * arrowSize,
			cos(angle + std::numbers::pi / 3) * arrowSize);

		QPointF arrowP2 = line().p1() + QPointF(sin(angle + 2 * std::numbers::pi / 3) * arrowSize,
			cos(angle + 2 * std::numbers::pi / 3) * arrowSize);

		head.clear();
		head << line().p1() << arrowP1 << arrowP2;
		break;
	}
	default:
		break;
	}

	painter->drawPolygon(head);
}
