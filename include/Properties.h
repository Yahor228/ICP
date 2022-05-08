#pragma once
#include <QDockWidget>
#include <memory>
#include <QComboBox>
#include <QLineEdit>
#include <QBoxLayout>
#include <QToolButton>



class Class;
class QListWidget;

class W : public QWidget
{
	Q_OBJECT
public:
	W();
signals:
	void DataChanged();
public:
	QHBoxLayout lay;
	QLineEdit le;
	QComboBox cbox;
	QToolButton delet;
};

class Internal : public QWidget
{
public:
	Internal(Class* xnode);
	W* MakeEmpty(QListWidget* lvd);
private:
	Class* node;
};

class Properties : public QDockWidget
{
public:
	Properties() = default;
public:
	void EditSelected(Class* node);
};