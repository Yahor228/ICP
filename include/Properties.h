/**
 * @file Properties.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for properties in CD
 */



#pragma once
#include <QDockWidget>
#include <memory>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QBoxLayout>
#include <QToolButton>
#include <QListWidget>
#include <QLabel>
#include <QRegularExpressionValidator>


class Node;
class Element;
class QListWidget;
class QListWidgetItem;
class EditableText;
class Connection;
struct ISelectable;

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
	QLineEdit alias;
	QToolButton add_data;
	QToolButton add_method;
	QListWidget data;
	QListWidget methods;
};

class ConnectionEditor : public QWidget
{
public:
	ConnectionEditor();
public:
	void Refill(Connection* xnode);
private:
	QVBoxLayout main_lay;
	Connection* conn = nullptr;
	QLabel msg;
	QLineEdit message;

	QGroupBox grp;
	QHBoxLayout lay;
	QComboBox rel_from;
	QComboBox rel_to;
	QRegularExpressionValidator valid;
};

class ElementEditor : public QWidget
{
public:
	ElementEditor();
public:
	void Refill(Element* xnode);
private:
	Element* node = nullptr;
	QVBoxLayout main_lay;
	QHBoxLayout lay;
	QLabel x;
	QLineEdit le;
};

class Properties : public QDockWidget
{
public:
	Properties() = default;
public:
	void EditSelected(ISelectable* node);
private:
	Internal inter;
	ConnectionEditor connector;
	ElementEditor elem;
};