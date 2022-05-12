#pragma once
#include <QDockWidget>
#include <memory>
#include <QComboBox>
#include <QLineEdit>
#include <QBoxLayout>
#include <QToolButton>

#include <model/Access.h>


class Class;
class QListWidget;
class QListWidgetItem;
class EditableText;

class W : public QWidget
{
	Q_OBJECT
public:
	W();
signals:
	void DataChanged(const QString& d);
	void DataChangeFinished();
	void AccessChanged(Access acc);
	void DeleteRequested();
public:
	QHBoxLayout lay;
	QLineEdit le;
	QComboBox cbox;
	QToolButton delet;
};

class Internal : public QWidget
{
	Q_OBJECT
public:
	using data_ty = std::pair<EditableText*, EditableText*>;
public:
	Internal(Class* xnode);
	std::pair<W*, QListWidgetItem*> MakeEmpty(QListWidget* lvd);
private:
	W* MakeData(data_ty& d);
	W* MakeMethod(data_ty& d);
signals:
	void CoAddData();
private:
	Class* node;
	QListWidget* data;
	QListWidget* methods;
};

class Properties : public QDockWidget
{
public:
	Properties() = default;
public:
	void EditSelected(Class* node);
};