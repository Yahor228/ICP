#pragma once
#include <QDockWidget>
#include <memory>
#include <QComboBox>
#include <QLineEdit>
#include <QBoxLayout>
#include <QToolButton>
#include <QListWidget>


#include <model/Access.h>


class Node;
class QListWidget;
class QListWidgetItem;
class EditableText;

class W : public QWidget
{
	Q_OBJECT
public:
	W(const QString& acc, const QString& name);
signals:
	void DataChanged(const QString& d);
	void AccessChanged(const QString& d);
	void DataChangeFinished();
	void DeleteRequested();
public:
	QHBoxLayout lay;
	QLineEdit le;
	QComboBox cbox;
	QToolButton delet;
};

class Internal : public QWidget
{
public:
	using data_ty = std::pair<EditableText*, EditableText*>;
public:
	Internal();
public:
	void Refill(Node* xnode);
	QListWidgetItem* MakeEmpty(QListWidget& lvd, W* it);
private:
	void MakeData(W* w);
	void MakeMethod(W* w);
private:
	Node* node = nullptr;
	QLineEdit name;
	QToolButton add_data;
	QToolButton add_method;
	QListWidget data;
	QListWidget methods;
};

class Properties : public QDockWidget
{
public:
	Properties() = default;
public:
	void EditSelected(Node* node);
private:
	Internal inter;
};