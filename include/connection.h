#pragma once
#include <QGraphicsLineItem>

class Class;



class Connection : public QGraphicsLineItem
{
public:
	enum class Type
	{
		none,
		aggr,
		asoc,
		comp,
		gener
	};
public:
	Connection(Class* from, Class* to, Type ty);
public:
	void updatePosition();
	QRectF boundingRect() const override;
	void DrawPolygon(QPainter* painter);
protected:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
private:
	void ApplyConnection();
private:
	Class* from;
	Class* to;
	Type ty;
};

inline Connection::Type to_type(const QString& s)
{
	using enum Connection::Type;
	if (s == u"G")return gener;
	if (s == u"C")return comp;
	if (s == u"AG")return aggr;
	if (s == u"AS")return asoc;
	return none;
}
