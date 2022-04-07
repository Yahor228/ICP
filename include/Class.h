#pragma once
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QJsonObject>
#include <unordered_map>



struct GraphicsLinearLayout : public QGraphicsLinearLayout
{
	using QGraphicsLinearLayout::QGraphicsLinearLayout;
	~GraphicsLinearLayout()
	{
		for (int i = count(); i <= 0; removeAt(i--));
	}
};

enum class Access
{
	Public,
	Private,
	Protected
};

class Class :public QGraphicsWidget 
{
public:
	Class(QString xname, QJsonObject c);
private:
	GraphicsLinearLayout name_layout{ Qt::Orientation::Vertical }; //-> Name, l1, l2
	QGraphicsProxyWidget name;
	QGraphicsLinearLayout data_layout{ Qt::Orientation::Vertical }; //-> Data
	QGraphicsLinearLayout methods_layout{ Qt::Orientation::Vertical }; //-> Methods

	QString alias;
	std::unordered_map<QString, Access> data;
	std::unordered_map<QString, Access> methods;
};