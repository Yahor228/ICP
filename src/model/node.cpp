#include <model/node.h>
#include <ui/UIVisitor.h>
#include <QJsonArray>
#include <util/util.h>
#include <ranges>

Node::Node(QString name, QJsonObject obj)
	:name(std::move(name)), alias(obj.contains("Alias") ? obj["Alias"].toString() : this->name)
{
	for (auto&& i : obj["Data"].toArray())
	{
		auto arr = i.toArray();
		data.emplace_back(arr[0].toString(), arr[1].toString());
	}
	for (auto&& i : obj["Methods"].toArray())
	{
		auto arr = i.toArray();
		methods.emplace_back(arr[0].toString(), arr[1].toString());
	}
}

void Node::InsertData(QString acc, QString Name)
{
	data.emplace_back(std::move(acc), std::move(Name));
	Update(ChangeMode::data);
}

void Node::InsertData(DataBinder m, size_t at)
{
	if (at >= data.size()) at = data.size();
	data.emplace(data.begin() + at, std::move(m));
	Update(ChangeMode::data);
}

void Node::InsertMethod(QString acc, QString Name)
{
	methods.emplace_back(std::move(acc), std::move(Name));
	Update(ChangeMode::methods);
}

void Node::InsertMethod(DataBinder m, size_t at)
{
	if (at >= methods.size()) at = methods.size();
	methods.emplace(methods.begin() + at, std::move(m));
	Update(ChangeMode::methods);
}

void Node::RemoveData(size_t at)
{
	data.erase(data.begin() + at);
	Update(ChangeMode::data);
}

void Node::RemoveMethod(size_t at)
{
	methods.erase(methods.begin() + at);
	Update(ChangeMode::methods);
}

ver::generator<const Node::DataBinder*> Node::Methods() const
{
	for (auto& i : methods)
		co_yield &i;
	for (auto* i : inherits)
		for (auto&& j : i->Methods())
			co_yield j;
}

void Node::Propagate(ChangeMode change) {
	Update(ChangeMode(change));
}

bool Node::DerivedFrom(Node& base)
{
	return std::ranges::find(inherits, &base) != inherits.end();
}

void Node::Save(QJsonObject& o) const
{
	QJsonArray outer_data;
	for (auto& i : data)
	{
		QJsonArray inner;
		inner.append(i.acc);
		inner.append(i.Name);
		outer_data.append(inner);
	}
	if (!outer_data.isEmpty())
		o.insert(qsl("Data"), outer_data);
	QJsonArray outer_data1;
	for (auto& i : methods)
	{
		QJsonArray inner;
		inner.append(i.acc);
		inner.append(i.Name);
		outer_data1.append(inner);
	}
	if (!outer_data1.isEmpty())
		o.insert(qsl("Methods"), outer_data1);
	if (alias != name)
		o.insert(qsl("Alias"), alias);
}

Node::ty Node::XType() const noexcept
{
	return node;
}

void Node::InheritFrom(Node& node)
{
	inherits.push_back(&node);
	connect(&node, &Node::Update, this, &Node::Propagate);
	Update(ChangeMode::values);
}

void Node::RemoveConnection(Node& node)
{
	disconnect(&node, &Node::Update, this, &Node::Propagate);
	inherits.erase(std::find(inherits.begin(), inherits.end(), &node));
	Update(ChangeMode::values);
}

void Node::accept(UIVisitor& visitor)
{
	if (visitor.Change() & ChangeMode::data)
		for (auto& i : data)
			visitor.InsertData(i.acc, i.Name);
	if (visitor.Change() & ChangeMode::methods)
		for (auto& i : methods)
			visitor.InsertMethod(i.acc, i.Name);
	visitor.SetMode(UIVisitor::Mode::inherited);
	for (auto& i : inherits)
		i->accept(visitor);
	visitor.SetMode(UIVisitor::Mode::normal);
}
