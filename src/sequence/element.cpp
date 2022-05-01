#include <sequence/element.h>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <sequence/texter.h>
#include <sequence/timeline.h>


Element::Element(const QString& header)
	:lay(new QGraphicsLinearLayout)
{
	setLayout(lay);
	lay->addItem(new EditableText(header, lay));
}

void Element::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(Qt::darkCyan);
	painter->drawRoundedRect(rect(), 5, 5);
	QGraphicsWidget::paint(painter, option, widget);
}
