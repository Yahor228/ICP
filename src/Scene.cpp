#include "Scene.h"
#include "Class.h"
#include "connection.h"
#include <QGraphicsSceneMouseEvent>
#include <commands/commandstack.h>
#include <commands/add_class.h>


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
				emit SelectionChanged(x);
				return;
			}
		}
		emit SelectionChanged(nullptr);
		});
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
			alias_mapper.emplace(c->alias().toStdU16String(), c);
			nodes.emplace(c->GetName().toStdU16String(), &c->Model());
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
		CommandStack::current().push(new AddClassCommand(this, mouseEvent->scenePos()));
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}
