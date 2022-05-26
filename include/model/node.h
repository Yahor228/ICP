/**
 * @file node.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for node which contains all information about single class
 */


#pragma once
#include <model/ChangeMode.h>
#include <ISave.h>
#include <ISelectable.h>
#include <span>

class UIVisitor;

class Node : public QObject, public ISave, public ISelectable
{
	Q_OBJECT
	using QSRTy = const QString&;
public:
	struct DataBinder { QString acc; QString Name; };
public:
	Node(QString name, QJsonObject obj);
public:
	void InsertData(QString acc, QString Name);
	void InsertData(DataBinder m, size_t at);
	void InsertMethod(QString acc, QString Name);
	void InsertMethod(DataBinder m, size_t at);
	void RemoveData(size_t at);
	void RemoveMethod(size_t at);
	std::span<DataBinder> Data() { return data; }
	std::span<DataBinder> Methods() { return methods; }

	QSRTy Alias()const { return alias; }
	void SetAlias(QSRTy in) { alias = in; }
	QSRTy Name()const { return name; }
	void SetName(QSRTy in) { name = in; Update(ChangeMode::name); }

	void InheritFrom(Node& node);
	void RemoveConnection(Node& node);
	
	void accept(UIVisitor& visitor);
	void Propagate(ChangeMode change);

	virtual void Save(QJsonObject& o)const override;
	virtual ty XType()const noexcept override;
signals:
	void Update(ChangeMode change);
	void Reselect();
private:
	QString name;
	QString alias;

	std::vector<DataBinder> data;
	std::vector<DataBinder> methods;

	std::vector<Node*> inherits;
};
