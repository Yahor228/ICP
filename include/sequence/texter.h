#pragma once
#include <QGraphicsTextItem>
#include <QGraphicsLayoutItem>

class QGraphicsLayout;

class EditableText : public QGraphicsTextItem, public QGraphicsLayoutItem
{
public:
	EditableText(const QString& text, QGraphicsLayout* layout, QGraphicsItem* parent = nullptr);
public:
	void setGeometry(const QRectF& geom);
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint) const;
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void focusOutEvent(QFocusEvent* event);
	void keyPressEvent(QKeyEvent* event);
private:
	QGraphicsLayout* mLayout;
};