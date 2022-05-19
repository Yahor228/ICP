#include <class/Class.h>
#include <QJsonArray>
#include <QPainter>
#include <ui/EditableText.h>
#include <connection.h>
#include <ui/UIVisitor.h>


Class::Class()
	:node(QStringLiteral("Class Name"), {}){}
Class::Class(QString xname, QJsonObject c)
	:node(std::move(xname), c)
{
	Init();
}

Class::~Class()
{
	for (int i = 0; i < main_layout->count(); i++)
		main_layout->removeAt(i);
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
	if(inherited)
	{
		xacc->SetColor(Qt::gray);
		l2->SetColor(Qt::gray);
	}

	return &u_layout;
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
	main_layout->insertItem(1, &data_layout);
}
void Class::EraseMethod()
{
	methods_layout.~QGraphicsLinearLayout();
	new (&methods_layout)QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	main_layout->insertItem(2, &methods_layout);
}

void Class::ConnectFrom(Connection* c)
{
	from.push_back(c);
}
void Class::ConnectTo(Connection* c)
{
	to.push_back(c);
}
void Class::DisconnectFrom(Connection* c)
{
	from.erase(std::find(from.begin(), from.end(), (c)));
}
void Class::DisconnectTo(Connection* c)
{
	to.erase(std::find(to.begin(), to.end(), (c)));
}

void Class::Reliquish()
{
	for (auto* i : from)
		i->UnbindTo();
	for (auto* i : to)
		i->UnbindFrom();
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

	if (isSelected())
		painter->setPen(QPen{ Qt::white, 2, Qt::PenStyle::DashLine });

	// path for the content of this node
	painter->setBrush(Qt::black);
	painter->drawRoundedRect(g, 5, 5);

	base::paint(painter, option, widget);
}

QVariant Class::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionChange)
	{
		for (auto& i : from)
		{
			i->update();
		}
		for (auto& i : to)
		{
			i->update();
		}
	}
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


