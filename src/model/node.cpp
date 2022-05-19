#include <model/node.h>
#include <ui/UIVisitor.h>

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

void Node::InsertMethod(QString acc, QString Name)
{
	methods.emplace_back(std::move(acc), std::move(Name));
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

void Node::InheritFrom(Node& node)
{
	inherits.push_back(&node);
	connect(&node, &Node::Update,
		[this](ChangeMode change) {
			Update(ChangeMode(change));
		}
	);
	Update(ChangeMode::values);
}

void Node::RemoveConnection(Node& node)
{
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
