#pragma once
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QJsonObject>
#include <unordered_map>


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
protected:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
private:
	QGraphicsLinearLayout* name_layout;
	QGraphicsLinearLayout* data_layout;
	QGraphicsLinearLayout* methods_layout;

	QString alias;
	std::unordered_map<QString, Access> data;
	std::unordered_map<QString, Access> methods;
};