#include <sequence/Element.h>
#include <sequence/Call.h>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QGraphicsView>
#include <model/node.h>
#include <ui/Log.h>
#include <util/util.h>
#include <QVariant>

Element::Element(QPointer<Node> xnode)
	:line({ 0, 0 }, { 0,1000 }), node(std::move(xnode))
{
	auto* lay = new QGraphicsLinearLayout;
	EditableText* xname = new EditableText(node->Name(), lay);
	EditableText* name1 = new EditableText(qsl(":"), lay);
	name = new EditableText(QStringLiteral("Instance"), lay);
	lay->addItem(xname);
	lay->addItem(name1);
	lay->addItem(name);
	setLayout(lay);

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setZValue(1.0);
	lay->activate();
	auto r = base::boundingRect();
	line.translate(QLineF{ r.bottomLeft(), r.bottomRight() }.center());
}
Element::~Element()
{
	if (owns_c)
	{
		for (auto* i : from)
			delete i;
		for (auto* i : to)
			delete i;
	}
}
QRectF Element::boundingRect() const
{
	auto r = base::boundingRect();
	auto y = scene()->views()[0]->sceneRect().bottom();
	r.setBottom(y);
	return r;
}
void Element::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(Qt::darkCyan);

	QColor cc{ isSelected() ? Qt::yellow : Qt::white };
	painter->setPen({ cc, 2.0 });

	auto r = layout()->geometry();
	painter->drawRoundedRect(r, 5, 5);

	painter->setPen(QPen{ cc, 2.0,Qt::DashLine,Qt::RoundCap });
	painter->drawLine(line);


	//painter->drawRect(boundingRect());

	QGraphicsWidget::paint(painter, option, widget);
}

QVariant Element::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionChange)
	{
		UpdateConnections();
		if (has_d)
		{
			prepareGeometryChange();
			auto p = line.p2();
			p.setY(has_d->BottomPoint() - pos().y());
			line.setP2(p);
			update();
		}
	}
	return QGraphicsWidget::itemChange(change, value);
}

Node* Element::GetNode() const
{
	return node.data();
}

const QString& Element::Name() const noexcept
{
	return name->Text();
}

void Element::ChangeName(const QString& c) noexcept
{
	prepareGeometryChange();
	name->SetText(c);
	update();
}

QString Element::UID() const
{
	return node->Name() + Name();
}

void Element::AddFrom(Call* c)
{
	if (c->Type() == Type::t_delete)
	{
		if (has_d)
		{
			return Logger::Warn(qsl("Destructor already set, remove previous"));
			from.push_back(c);
			return;
		}

		prepareGeometryChange();
		has_d = c;
		auto p = line.p2();
		p.setY(has_d->BottomPoint() - pos().y());
		line.setP2(p);

		update();
	}

	from.push_back(c);
}

void Element::AddTo(Call* c)
{
	to.push_back(c);
}

void Element::RemoveFrom(Call* c)
{
	from.erase(std::find(from.begin(), from.end(), (c)));

	if (c->Type() == Type::t_delete)
	{
		prepareGeometryChange();
		has_d = nullptr;
		line.setLength(1000);
		update();
	}
}

void Element::RemoveTo(Call* c)
{
	to.erase(std::find(to.begin(), to.end(), (c)));
}

void Element::Relinquish()
{
	owns_c = true;
	for (auto* i : from)
		i->UnbindFrom();
	for (auto* i : to)
		i->UnbindTo();
}

void Element::Reconnect()
{
	owns_c = false;
	for (auto* i : from)
		i->BindFrom(scene());
	for (auto* i : to)
		i->BindTo(scene());
}

void Element::UpdateConnections()
{
	for (auto& i : from)
		i->Update();
	for (auto& i : to)
		i->Update();
}

void Element::Save(QJsonObject& o) const
{
	auto p = scenePos();
	QJsonArray pos;
	pos.append(p.x());
	pos.append(p.y());
	o.insert(QStringLiteral("Pos"), pos);
	o.insert(QStringLiteral("Name"), Name());
	o.insert(QStringLiteral("Class"), GetNode()->Name());
}

bool Element::Valid() const noexcept { return node; }
