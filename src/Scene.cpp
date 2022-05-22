#include "Scene.h"
#include "class/Class.h"
#include "class/connection.h"
#include <QGraphicsSceneMouseEvent>
#include <commands/commandstack.h>
#include <commands/add_class.h>
#include <commands/delete_class.h>
#include <commands/delete_connection.h>
#include <commands/reverse_connection.h>
#include <util/util.h>

#include <ui/Log.h>


/*
* descriptor
{
"Classes":{
"human":{ ->QJsonObject
	"Data":[["+","age"],
	["+","name"],
	["+","heght"]],
	"Methods":[["+","GetAge():number"]],
	"Alias": "H"
},
"human2":{ ->QJsonObject
	"Data":[["+","age"],
	["+","name"],
	["+","heght"]],
	"Methods":[["+","GetAge():number"]],
	"Alias": "H2"
}
}
"Connections":[[H,H2,con_t]]
}
*/


Scene::Scene()
{
	connect(this, &QGraphicsScene::selectionChanged, [this]() {
		for (auto* i : selectedItems())
		{
			if (auto* x = dynamic_cast<Class*>(i))
			{
				emit SelectionChanged(&x->Model());
				return;
			}
		}
		emit SelectionChanged(nullptr);
		});

	context.addAction(qsl("Add Class"), [this]() {
		CommandStack::current().push(new AddClassCommand(this, pos));
		});
	context.addAction(qsl("Connect"), [this]() {
		for (auto* x : items(pos))
		{
			if (auto* c = dynamic_cast<Class*>(x))
				return CreateConnection(c);
		}
		});
}

void Scene::RemoveSelected()
{
	for (auto* i : selectedItems())
	{
		if (auto* x = dynamic_cast<Class*>(i))
			CommandStack::current().push(new DeleteClassCommand(this, x));
		else if (auto* x = dynamic_cast<Connection*>(i))
			CommandStack::current().push(new DeleteConnectionCommand(this, x));
	}
}

void Scene::ReverseSelected()
{
	for (auto* i : selectedItems())
	{
		if (auto* x = dynamic_cast<Connection*>(i))
		{
			CommandStack::current().push(new ReverseConnectionCommand(x));
			return;
		}
	}
}

void Scene::LoadFrom(QJsonObject doc)
{
	std::unordered_map<std::u16string, Class*> alias_mapper;
	if (doc.contains("Classes"))
	{
		auto&& o = doc["Classes"].toObject();
		auto ks = o.keys();

		for (auto&& i : ks)
		{
			auto* c = new Class(std::move(i), o[i].toObject());

			auto alias = c->Alias().toStdU16String();
			if (alias_mapper.contains(alias))
				Logger::Warn(qsl("The alias %1 has already been defined, results may be corrupted.").arg(c->Alias()));

			alias_mapper.emplace(alias, c);
			nodes.emplace(c->Name().toStdU16String(), &c->Model());
			addItem(c);
		}
	}
	if (doc.contains("Connections"))
	{
		auto&& a = doc["Connections"].toArray();

		for (auto&& i : a)
		{
			auto j = i.toArray();
			auto n1 = j[0].toString().toStdU16String();
			auto n2 = j[1].toString().toStdU16String();
			auto n3 = j[2].toString();

			auto* c1 = alias_mapper[n1];
			auto* c2 = alias_mapper[n2];

			auto* conn = new Connection(c1, c2, to_type(n3));
			if (!conn->Valid())
			{
				Logger::Warn(qsl("The connection from %1 to %2 of type %3 is invalid and is not created.")
					.arg(j[0].toString()).arg(j[1].toString()).arg(n3));
				delete conn;
				continue;
			}
			conn->Reconnect();
			addItem(conn);
		}
	}
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if (mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		pos = mouseEvent->scenePos();
		context.popup(mouseEvent->screenPos());
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::CreateConnection(Class* c)
{
	auto cc = new ConnectionCreator(c);
	addItem(cc);
	cc->grabMouse();
}

void Scene::Save(QJsonObject& doc)const
{
	std::unordered_map<std::u16string_view, Class*> aliases;
	std::unordered_map<std::u16string_view, Class*> names;
	QJsonObject classes;
	QJsonArray connections;
	for (auto* i : items())
	{
		if (auto* c = dynamic_cast<Class*>(i))
		{
			QJsonObject o;
			c->Save(o);
			std::u16string_view v{ (const char16_t*)c->Alias().utf16() };
			if (aliases.contains(v))
			{
				auto c1 = aliases.at(v);
				c1->setSelected(true);
				c->setSelected(true);
				Logger::Warn(qsl("Class \"%1\" has the same alias as \"%2\". The results are undefined.")
				.arg(c->Name()).arg(c1->Name()));
			}
			std::u16string_view v2{ (const char16_t*)c->Name().utf16() };
			if (names.contains(v2))
			{
				auto c1 = names.at(v2);
				c1->setSelected(true);
				c->setSelected(true);
				Logger::Warn(qsl("Class \"%1\" has the same name as \"%2\". The results are undefined.")
					.arg(c->Name()).arg(c1->Name()));
			}

			classes.insert(c->Name(), o);
			aliases.emplace(v, c);
			names.emplace(v2, c);
		}
		else if (auto* c = dynamic_cast<Connection*>(i))
		{
			QJsonObject o;
			c->Save(o);
			connections.append(o["A"]);
		}
	}
	doc.insert(qsl("Classes"), classes);
	doc.insert(qsl("Connections"), connections);
}
