#include <sequence/Element.h>
#include <QGraphicsLinearLayout>
#include <QPainter>

Element::Element(const QString& xname)
{
	auto* lay = new QGraphicsLinearLayout;
	EditableText* name = new EditableText(xname,lay);
	lay->addItem(name);
	setLayout(lay);



	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setZValue(1.0);
}

void Element::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(Qt::darkCyan);
	painter->setPen({ Qt::white, 2.0 });
	painter->drawRoundedRect(boundingRect(), 5, 5);
	QGraphicsWidget::paint(painter, option, widget);
}

QVariant Element::itemChange(GraphicsItemChange change, const QVariant& value)
{
	return QGraphicsWidget::itemChange(change, value);
}
