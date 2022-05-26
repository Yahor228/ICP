#pragma once
#include <QGraphicsWidget>
#include <ui/EditableText.h>

class Element : public QGraphicsWidget
{
	using base = QGraphicsWidget;
public:
	Element(const QString& name);
	QRectF boundingRect() const override;
public:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;
private:
	QLineF line;
};