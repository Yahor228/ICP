#pragma once
#include <QGraphicsWidget>

class QGraphicsLinearLayout;

class Element : public QGraphicsWidget
{
public:
	Element(const QString& header);
public:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
private:
	QGraphicsLinearLayout* lay;
};