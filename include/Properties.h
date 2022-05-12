#pragma once
#include <QDockWidget>
#include <memory>
#include <QComboBox>
#include <QLineEdit>
#include <QBoxLayout>
#include <QToolButton>


class Class;
class QListWidget;
class QListWidgetItem;
class EditableText;
enum class Access;

class W : public QWidget
{
	Q_OBJECT
public:
	W();
public:
	void SetData(std::u16string* d);
	void SetAccess(Access* xacc);
signals:
	void DataChanged(const QString& d);
	void AccessChanged(Access acc);
	void DeleteRequested();
public:
	std::u16string* data = nullptr;
	Access* acc = nullptr;
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