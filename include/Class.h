#pragma once
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QJsonObject>
#include <model/node.h>




class Class :public QGraphicsWidget
{
public:
	Class();
	Class(QString xname, QJsonObject c);
public:
	const QString& GetName()const noexcept
	{
		return node.name;
	}
	void SetName(QString xname);
	const QString& alias()const noexcept
	{
		return node.alias.isEmpty() ? node.name : node.alias;
	}
	Node& Model()
	{
		return node;
	}
protected:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
private:
	void Init();
private:
	QLabel* l_name;
	QGraphicsLinearLayout* name_layout;
	QGraphicsLinearLayout* data_layout;
	QGraphicsLinearLayout* methods_layout;
	Node node;
};