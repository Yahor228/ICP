#pragma once
#include <model/ChangeMode.h>
#include <QJsonObject>
#include <span>

class UIVisitor;

class Node : public QObject
{
	Q_OBJECT
	using QSRTy = const QString&;
public:
	struct DataBinder { QString acc; QString Name; };
public:
	Node(QString name, QJsonObject obj);
public:
	void InsertData(QString acc, QString Name);
	void InsertMethod(QString acc, QString Name);
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
signals:
	void Update(ChangeMode change);
private:
	QString name;
	QString alias;

	std::vector<DataBinder> data;
	std::vector<DataBinder> methods;

	std::vector<Node*> inherits;
};
