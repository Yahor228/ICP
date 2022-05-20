#include "Scene.h"
#include "class/Class.h"
#include "class/connection.h"
#include <QGraphicsSceneMouseEvent>
#include <commands/commandstack.h>
#include <commands/add_class.h>
#include <commands/delete_class.h>
#include <commands/delete_connection.h>
#include <util/util.h>


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
			alias_mapper.emplace(c->Alias().toStdU16String(), c);
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

			addItem(new Connection(c1, c2, to_type(n3)));
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
