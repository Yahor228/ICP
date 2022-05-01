#pragma once
#include <QDockWidget>
#include <memory>

class Class;


class Internal : public QWidget
{
public:
	Internal(Class* xnode);
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