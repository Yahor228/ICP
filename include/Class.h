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
	Class(QString xname, QJsonObject c);
public:
	std::u16string_view name()const noexcept
	{
		return node.name;
	}
	std::u16string_view alias()const noexcept
	{
		return node.alias.empty() ? node.name : node.alias;
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
	QGraphicsLinearLayout* name_layout;
	QGraphicsLinearLayout* data_layout;
	QGraphicsLinearLayout* methods_layout;
	Node node;
};