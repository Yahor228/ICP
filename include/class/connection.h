#pragma once
#include <QGraphicsLineItem>
#include <QMenu>

class Class;

class Connection : public QGraphicsLineItem
{
	friend class ConnectionCreator;
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
	QRectF boundingRect() const override;
	void DrawPolygon(QPainter* painter);
	void UnbindFrom();
	void UnbindTo();
	void BindFrom(QGraphicsScene* scene);
	void BindTo(QGraphicsScene* scene);
	void Disconnect();
	void Reconnect();
	Type GetType()const { return ty; }
	bool Valid()const;
	bool ValidateAgainst(Class* from)const;
	void ApplyConnection();
protected:
	Connection(Class* from, Class* to, Type ty, bool);
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
protected:
	Class* from;
	Class* to;
	Type ty;
};

class ConnectionCreator : public QGraphicsLineItem
{
	struct Menu : QMenu
	{
		static auto& instance()
		{
			static Menu m;
			return m;
		}
		static void Exec(ConnectionCreator* xfor, Class* xto, QPoint pos)
		{
			auto& i = instance();
			i.x = xfor;
			i.to = xto;
			i.exec(pos);
		}
		static void Clear()
		{
			auto& i = instance();
			delete i.x;
			i.x = nullptr;
		}
	private:
		Menu();
		ConnectionCreator* x = nullptr;
		Class* to = nullptr;
	};
public:
	ConnectionCreator(Class* from);
public:
	QRectF boundingRect() const override;
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
	void SetConnection(Class* to, Connection::Type t);
private:
	Class* from;
	QPointF pos;
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
