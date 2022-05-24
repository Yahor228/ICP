/**
 * @file window.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration class for the connections of CD
 */


#pragma once
#include <QGraphicsLineItem>
#include <QMenu>
#include <ISave.h>
#include <util/util.h>

class Class;

class Connection : public QGraphicsPathItem, public ISave
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
	/// <summary>
	/// function to create an 'arrow'
	/// </summary>
	/// <param name="from">start point </param>
	/// <param name="to"> end point</param>
	/// <param name="ty"> type of conection </param>
	Connection(Class* from, Class* to, Type ty);
public:
	QRectF boundingRect() const override;
	void DrawPolygon(QPainter* painter, const QLineF& line);
	void UnbindFrom();
	void UnbindTo();
	void BindFrom(QGraphicsScene* scene);
	void BindTo(QGraphicsScene* scene);
	void Disconnect();
	void Reconnect();
	void Reverse();
	Type GetType()const { return ty; }
	bool Valid()const;
	bool ValidateAgainst(Class* from)const;

	virtual void Save(QJsonObject& o)const override;
protected:
	void CalculatePointsSelf(std::array<QPointF, 5>& poly);
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	void PaintNormal(QPainter* painter);
	void PaintSelf(QPainter* painter);
protected:
	Class* from;
	Class* to;
	Type ty;
	uint8_t self;
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
	if (s == qsl("G"))return Connection::Type::gener;
	if (s == qsl("C"))return Connection::Type::comp;
	if (s == qsl("AG"))return Connection::Type::aggr;
	if (s == qsl("AS"))return Connection::Type::asoc;
	return Connection::Type::none;
}
inline QString to_string(Connection::Type ty)
{
	switch (ty)
	{
	default:
	case Connection::Type::none:return "";
	case Connection::Type::aggr:return qsl("AG");
	case Connection::Type::asoc:return qsl("AS");
	case Connection::Type::comp:return qsl("C");
	case Connection::Type::gener:return qsl("G");
	}
}
