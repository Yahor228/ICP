#pragma once
#include <QJsonObject>

struct ISave
{
	~ISave() = default;
	virtual void Save(QJsonObject& o)const = 0;
};