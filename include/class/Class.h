#pragma once
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QJsonObject>
#include <model/node.h>
#include <span>


class EditableText;
class Connection;

class Class :public QGraphicsWidget
{
	using STy = const QString&;
	using base = QGraphicsWidget;
public:
	Class();
	Class(QString xname, QJsonObject c);
	~Class();
public:
	STy Name()const { return node.Name(); }
	STy Alias()const { return node.Alias(); }
	Node& Model(){return node;}
	const Node& Model()const {return node;}
public:
	void AppendData(STy acc, STy name, bool inherited);
	void AppendMethod(STy acc, STy name, bool inherited);
	void EraseData();
	void EraseMethod();
public:
	void ConnectFrom(Connection* c);
	void ConnectTo(Connection* c);
	void DisconnectFrom(Connection* c);
	void DisconnectTo(Connection* c);

	void Reconnect();
	void Reliquish();
	void Update(ChangeMode change);
protected:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;
private:
	void Init();
	QGraphicsLinearLayout* MakeItem(STy acc, STy name, bool inherited = false);
private:
	EditableText* l_name;

	QGraphicsLinearLayout* main_layout;
	QGraphicsLinearLayout name_layout;
	QGraphicsLinearLayout data_layout;
	QGraphicsLinearLayout methods_layout;

	std::vector<Connection*> from;
	std::vector<Connection*> to;
	Node node;
	bool owns_conn = false;
};