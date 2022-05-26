#pragma once
#include <QGraphicsPathItem>
#include <QMenu>

class Element;

class Call : public QGraphicsPathItem
{
public:
	enum class Type
	{
		sync,
		async
	};
public:
	Call();
public:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
private:
};

class CallCreator :public QGraphicsItem
{
public:
	CallCreator(Element* from, QPointF start);
private:
	QRectF boundingRect() const;
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
	void SetConnection(Element* to, Call::Type t);
private:
	Element* from; 
	Element* to = nullptr; 
	QPointF start;
	qreal pos = 0.0;
	QMenu menu;
};