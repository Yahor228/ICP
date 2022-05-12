#pragma once
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QJsonObject>
#include <model/node.h>
#include <span>


class EditableText;

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
	std::span<std::pair<EditableText*, EditableText*>> Data()noexcept
	{
		return data;
	}
	std::pair<EditableText*, EditableText*>& AppendData();
	void EraseData(size_t index);

	std::span<std::pair<EditableText*, EditableText*>> Methods()noexcept
	{
		return methods;
	}
	std::pair<EditableText*, EditableText*>& AppendMethod();
	void EraseMethod(size_t index);
	std::pair<Access, QString>& MethodModel(size_t index);

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

	std::vector<std::pair<EditableText*, EditableText*>> data;
	std::vector<std::pair<EditableText*, EditableText*>> methods;
	Node node;
};