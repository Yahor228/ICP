#include <sequence/Element.h>
#include <sequence/Call.h>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QGraphicsView>
#include <model/node.h>

Element::Element(QPointer<Node> xnode)
	:line({ 0, 0 }, { 0,1000 }), node(std::move(xnode))
{
	auto* lay = new QGraphicsLinearLayout;
	EditableText* name = new EditableText(node->Name(), lay);
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
		UpdateConnections();
	return QGraphicsWidget::itemChange(change, value);
}

Node* Element::GetNode() const
{
	return node.data();
}

void Element::AddFrom(Call* c)
{
	from.push_back(c);
}

void Element::AddTo(Call* c)
{
	to.push_back(c);
}

void Element::RemoveFrom(Call* c)
{
	from.erase(std::find(from.begin(), from.end(), (c)));
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
