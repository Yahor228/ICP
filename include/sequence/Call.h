#pragma once
#include <QGraphicsPathItem>
#include <ISave.h>

class Element;

enum class Type
{
	t_none,
	t_sync,
	t_async,
	t_create,
	t_delete
};

struct desc {
	Element* from;
	Element* to;
	QString func;
	Type ty;
	qreal start;
	qreal length;
};

class Call : public QGraphicsItem, public ISave
{
public:
	Call(desc d);
public:
	QRectF boundingRect() const override;
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	void Update();

	void Connect();
	void Disconnect();
	auto Type()const { return d.ty; }
	qreal BottomPoint()const noexcept;

	void UnbindFrom();
	void UnbindTo();
	void BindFrom(QGraphicsScene* scene);
	void BindTo(QGraphicsScene* scene);

	virtual void Save(QJsonObject& o)const override;
private:
	desc d;
};

class CallCreator :public QGraphicsItem
{
	enum state {
		s_line,
		s_rect,
	}st = s_line;
public:
	CallCreator(Element* from, QPointF start);
private:
	QRectF boundingRect() const;
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
	void SetConnection();
	void ProduceConnection();

	void LineEvent(QGraphicsSceneMouseEvent* event);
	void RectEvent(QGraphicsSceneMouseEvent* event);
private:
	Element* from; 
	Element* to = nullptr; 
	QPointF start;
	QString func;
	::Type ty = Type::t_none;
	qreal posy = 0.0;
	qreal pos;
};