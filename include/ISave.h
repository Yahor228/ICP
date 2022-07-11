/**
 * @file ISave.h
 * @author Yahor Senichak (xsenic00)
 * @brief 
 */




#pragma once
#include <QJsonObject>

struct ISave
{
	~ISave() = default;
	virtual void Save(QJsonObject& o)const = 0;
};