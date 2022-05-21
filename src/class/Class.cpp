#include <class/Class.h>
#include <QJsonArray>
#include <QPainter>
#include <QPainterPath>
#include <ui/EditableText.h>
#include <class/connection.h>
#include <ui/UIVisitor.h>


Class::Class()
	:Class(QStringLiteral("Class Name"), {})
{}
Class::Class(QString xname, QJsonObject c)
	:node(std::move(xname), c)
{
	Init();
}

Class::~Class()
{
	for (int i = 0; i < main_layout->count(); i++)
		main_layout->removeAt(i);
	if (owns_conn)
	{
		for (auto* i : from)
			delete i;
		for (auto* i : to)
			delete i;
	}
}

QGraphicsLinearLayout* Class::MakeItem(STy acc, STy name, bool inherited)
{
	prepareGeometryChange();
	auto& u_layout = *new QGraphicsLinearLayout{ Qt::Orientation::Horizontal };

	auto* xacc = new EditableText(acc, &u_layout);
	auto* l2 = new EditableText(name, &u_layout);
	u_layout.setContentsMargins(0, 0, 0, 0);
	u_layout.addItem(xacc);
	u_layout.addItem(l2);
	u_layout.setSpacing(0);
	if (inherited)
	{
		xacc->SetColor(Qt::gray);
		l2->SetColor(Qt::gray);
	}

	return &u_layout;
}
void Class::UpdateConnections()
{
	for (auto& i : from)
		i->Update();
	for (auto& i : to)
		i->Update();
	for (auto& i : self)
		i->Update();
}
void Class::AppendData(STy acc, STy name, bool inherited)
{
	data_layout.addItem(MakeItem(acc, name, inherited));
}
void Class::AppendMethod(STy acc, STy name, bool inherited)
{
	methods_layout.addItem(MakeItem(acc, name, inherited));
}

void Class::EraseData()
{
	data_layout.~QGraphicsLinearLayout();
	new (&data_layout) QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	data_layout.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	main_layout->insertItem(1, &data_layout);
}
void Class::EraseMethod()
{
	methods_layout.~QGraphicsLinearLayout();
	new (&methods_layout)QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	methods_layout.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	main_layout->insertItem(2, &methods_layout);
}

void Class::ConnectFrom(Connection* c)
{
	from.push_back(c);
}
void Class::ConnectTo(Connection* c)
{
	to.push_back(c);
	UpdateConnections();
}
void Class::ConnectSelf(Connection* c)
{
	self.push_back(c);
}
void Class::DisconnectFrom(Connection* c)
{
	from.erase(std::find(from.begin(), from.end(), (c)));
	UpdateConnections();
}
void Class::DisconnectTo(Connection* c)
{
	to.erase(std::find(to.begin(), to.end(), (c)));
	UpdateConnections();
}
void Class::DisconnectSelf(Connection* c)
{
	self.erase(std::find(self.begin(), self.end(), (c)));
	UpdateConnections();
}


bool Class::ValidateConnection(Class* c)
{
	for (auto* i : to)
		if (!i->ValidateAgainst(c))
			return false;
	return true;
}

void Class::Reconnect()
{
	owns_conn = false;
	for (auto* i : from)
		i->BindFrom(scene());
	for (auto* i : to)
		i->BindTo(scene());
}

void Class::Reliquish()
{
	owns_conn = true;
	for (auto* i : from)
		i->UnbindFrom();
	for (auto* i : to)
		i->UnbindTo();
}

void Class::Update(ChangeMode change)
{
	if (change & ChangeMode::name)
		return l_name->SetText(node.Name());
	if (change & ChangeMode::data)
		EraseData();
	if (change & ChangeMode::methods)
		EraseMethod();

	UIVisitor v{ change, *this };
	node.accept(v);
}

void Class::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto g = layout()->geometry();
	auto g1 = name_layout.geometry();
	auto g2 = data_layout.geometry();
	auto g3 = methods_layout.geometry();

	auto stop1 = QPointF(g.right(), g1.bottom() + 0.5 * g1.height());
	auto top_left1 = g.topLeft() + QPointF{ 0,g1.height() / 2 };
	// path for the caption of this node
	QPainterPath path_title;
	path_title.setFillRule(Qt::WindingFill);
	path_title.addRoundedRect(QRectF{ g.topLeft(),stop1 }, 5, 5);
	path_title.addRect(QRectF{ top_left1, stop1 });
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::black);
	auto p1x = path_title.simplified();
	painter->drawPath(p1x);

	stop1.setX(g.left());
	// path for the content of this node
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRectF{ stop1, g.bottomRight() }, 5, 5);
	path_content.addRect(QRectF{ stop1, stop1 + QPointF{ g.width(),5.0f} });
	painter->setBrush(QGradient{ QGradient::ViciousStance });
	auto p2x = path_content.simplified();
	painter->drawPath(p2x);

	if (isSelected()) painter->setPen(QPen{ Qt::white, 2, Qt::PenStyle::DashLine });
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath((p1x + p2x).simplified());

	auto y = (g3.top() - g2.bottom()) / 2 + g2.bottom();

	painter->setPen(QPen{ Qt::gray, 2});
	painter->drawLine(QLineF{g.left(), y, g.right(), y});

	base::paint(painter, option, widget);
}

QVariant Class::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionChange)
		UpdateConnections();
	return base::itemChange(change, value);
}

void Class::Init()
{
	connect(&node, &Node::Update, this, &Class::Update);

	main_layout = new QGraphicsLinearLayout{ Qt::Orientation::Vertical };

	name_layout.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	data_layout.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	methods_layout.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	l_name = new EditableText(node.Name(), &name_layout);
	QFont f{};
	f.setBold(true);
	l_name->SetFont(f);

	name_layout.addItem(l_name);
	main_layout->addItem(&name_layout);

	main_layout->addItem(&data_layout);
	main_layout->addItem(&methods_layout);
	setLayout(main_layout);

	Update(ChangeMode::values);

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setZValue(1.0);
}


