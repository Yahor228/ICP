#include <sequence/Element.h>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QGraphicsView>

Element::Element(const QString& xname)
	:line({ 0, 0 }, { 0,1000 })
{
	auto* lay = new QGraphicsLinearLayout;
	EditableText* name = new EditableText(xname, lay);
	lay->addItem(name);
	setLayout(lay);

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setZValue(1.0);
	lay->activate();
	auto r = base::boundingRect();
	line.translate(QLineF{ r.bottomLeft(), r.bottomRight() }.center());
}
QRectF Element::boundingRect() const
{
	auto r = base::boundingRect();
	auto y = scene()->views()[0]->sceneRect().bottom();
	r.setBottom(y);
	return r;
}
void Element::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(Qt::darkCyan);
	painter->setPen({ Qt::white, 2.0 });
	
	auto r = layout()->geometry();
	painter->drawRoundedRect(r, 5, 5);

	painter->setPen(QPen{ Qt::white, 2.0,Qt::DashLine,Qt::RoundCap });
	painter->drawLine(line);

	QGraphicsWidget::paint(painter, option, widget);
}

QVariant Element::itemChange(GraphicsItemChange change, const QVariant& value)
{
	return QGraphicsWidget::itemChange(change, value);
}
