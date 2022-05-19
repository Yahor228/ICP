#pragma once
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>

class Data : public QGraphicsItem, public QGraphicsLayoutItem
{
private:
	QString access;
	QString data;
};