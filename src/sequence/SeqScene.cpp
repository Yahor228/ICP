#include <sequence/SeqScene.h>
#include <sequence/Element.h>
#include <sequence/Call.h>
#include <Scene.h>
#include <QGraphicsSceneMouseEvent>
#include <model/node.h>
#include <util/util.h>
#include <commands/commandstack.h>
#include <commands/add_call.h>
#include <commands/add_element.h>
#include <ui/Log.h>


SeqScene::SeqScene()
{
	connect(this, &QGraphicsScene::selectionChanged, [this]() {
		for (auto* i : selectedItems())
		{
			if (auto* x = dynamic_cast<ISelectable*>(i))
				return SelectionChanged(x);
		}
		SelectionChanged(nullptr);
		});
}

void SeqScene::RemoveSelected()
{
	for (auto* i : selectedItems())
	{
		if (auto* x = dynamic_cast<Call*>(i))
			CommandStack::current().push(new RemoveCallCommand(this, x));
		else if (auto* x = dynamic_cast<Element*>(i))
			CommandStack::current().push(new RemoveElementCommand(this, x));
	}
}

std::span<Node* const> SeqScene::GetNodes() const noexcept
{
	return class_scene->Nodes();
}

void SeqScene::CreateConnection(Element* element)
{
	auto cc = new CallCreator(element, pos);
	addItem(cc);
	cc->grabMouse();
}

void SeqScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if (mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		pos = mouseEvent->scenePos();
		ReviseMenu();
		menu->popup(mouseEvent->screenPos());
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void SeqScene::ReviseMenu()
{
	if (class_scene->IsImmutable())return;
	menu.emplace();
	menu->addAction(qsl("Connect"), [this]() {
		for (auto* x : items(pos, Qt::IntersectsItemBoundingRect))
		{
			if (auto* c = dynamic_cast<Element*>(x))
				return CreateConnection(c);
		}
		});
	for (auto& i : GetNodes())
	{
		auto& name = i->Name();
		menu->addAction(name, [this, i]() {
			CommandStack::current().push(new AddElementCommand(this, new Element(i), pos));
			});
	}
}

void SeqScene::Save(QJsonObject& doc) const
{
	std::unordered_map<QString, Element*> names;

	QJsonObject classes;
	QJsonArray connections;

	for (auto* i : items())
	{
		if (auto* c = dynamic_cast<Element*>(i))
		{
			if (!c->Valid())continue;
			QJsonObject o;
			c->Save(o);
			auto v2 = c->UID();
			if (names.contains(v2))
			{
				auto c1 = names.at(v2);
				c1->setSelected(true);
				c->setSelected(true);
				Logger::Warn(qsl("Class \"%1\" has the same name as \"%2\". The results are undefined.")
					.arg(c->Name()).arg(c1->Name()));
			}

			classes.insert(v2, o);
			names.emplace(v2, c);
		}
		else if (auto* c = dynamic_cast<Call*>(i))
		{
			QJsonObject o;
			c->Save(o);
			connections.append(o["A"]);
		}
	}
	doc.insert(qsl("Classes"), classes);
	doc.insert(qsl("Connections"), connections);
}
void SeqScene::LoadFrom(QJsonObject doc)
{
	if (!doc.contains(qsl("Classes")))return;

	auto&& o = doc["Classes"].toObject();
	auto ks = o.keys();

	std::unordered_map<QString, Node*> mapper;
	std::unordered_map<QString, Element*> inst;
	for (auto&& i : GetNodes())
		mapper.emplace(i->Name(), i);


	for (auto&& i : ks)
	{
		auto r = o[i].toObject();
		auto cla = r["Class"].toString();
		if (!mapper.contains(cla))continue;

		auto* c = new Element(mapper[cla]);
		c->ChangeName(r["Name"].toString());
		if (r.contains("Pos"))
		{
			auto a = r["Pos"].toArray();
			c->setPos(a[0].toDouble(), a[1].toDouble());
		}
		inst.emplace(i, c);
		addItem(c);
	}

	if (!doc.contains(qsl("Connections")))return;

	for (auto&& i : doc[qsl("Connections")].toArray())
	{
		if (!i.isArray())continue;
		auto arr = i.toArray();
		desc d{};
		
		auto from = arr[0].toString();
		if(!inst.contains(from))
			continue;
		d.from = inst[from];

		auto to = arr[1].toString();
		if (!inst.contains(to))
			continue;
		d.to = inst[to];

		auto func = arr[2].toString();
		if (func != "<<create>>" && func != "<<delete>>")
		{
			bool found = false;
			for (auto& j : d.to->GetNode()->Methods())
			{
				if (found = j->Name == func)
					break;
			}
			if (!found)continue;
		}

		d.func = func;

		d.ty = Type(arr[3].toInt());

		if (d.ty == Type::t_none)continue;
		d.start = arr[4].toDouble();
		d.length = arr[5].toDouble();

		auto call = new Call(d);
		call->Connect();
		addItem(call);
	}
}
