#include <Class.h>
#include <QJsonArray>
#include <QPainter>
#include <sequence/texter.h>


Class::Class()
	:l_name(new QLabel(QStringLiteral("Class Name")))
{
	Init();
}

Class::Class(QString xname, QJsonObject c)
{
	l_name = new QLabel(xname);
	Init();

	if (c.contains("Data"))
	{
		auto x = c["Data"].toArray();

		for (auto&& i : x)
		{
			auto arr = i.toArray();
			auto[x,y] = AppendData();
			x->SetText(arr[0].toString());
			y->SetText(arr[1].toString());
		}
	}

	node.name = std::move(xname);

	if (c.contains("Alias"))
		node.alias = c["Alias"].toString();

	if (c.contains("Methods"))
	{
		auto x = c["Methods"].toArray();

		for (auto&& i : x)
		{
			auto arr = i.toArray();
			auto n = arr[1].toString();
			auto a = arr[0].toString();

			auto [x, y] = AppendMethod();
			x->SetText(arr[0].toString());
			y->SetText(arr[1].toString());
		}
	}
}

void Class::SetName(QString xname)
{
	node.name = xname;
	l_name->setText(xname);
}

std::pair<EditableText*, EditableText*>& Class::AppendData()
{
	prepareGeometryChange();
	auto& u_layout = *new QGraphicsLinearLayout{ Qt::Orientation::Horizontal };

	auto* l2 = new EditableText("", &u_layout);
	auto* acc = new EditableText("", &u_layout);

	l2->SetEditable(false);
	acc->SetEditable(false);
	u_layout.setContentsMargins(0, 0, 0, 0);


	u_layout.addItem(acc);
	u_layout.addItem(l2);
	u_layout.setSpacing(0);
	
	data_layout->addItem(&u_layout);
	return data.emplace_back(acc, l2);
}

void Class::EraseData(size_t index)
{
	auto& x = data.at(index);
	auto* lay = x.first->Layout();
	data_layout->removeItem(lay);
	delete lay;
	delete x.first;
	delete x.second;
	data.erase(data.begin() + index);
	update();
}

std::pair<EditableText*, EditableText*>& Class::AppendMethod()
{
	prepareGeometryChange();
	auto& u_layout = *new QGraphicsLinearLayout{ Qt::Orientation::Horizontal };

	auto* l2 = new EditableText("", &u_layout);
	auto* acc = new EditableText("", &u_layout);

	l2->SetEditable(false);
	acc->SetEditable(false);
	u_layout.setContentsMargins(0, 0, 0, 0);


	u_layout.addItem(acc);
	u_layout.addItem(l2);
	u_layout.setSpacing(0);

	//model

	methods_layout->addItem(&u_layout);
	return methods.emplace_back(acc, l2);
}

void Class::EraseMethod(size_t index)
{
	auto& x = methods.at(index);
	auto* lay = x.first->Layout();
	methods_layout->removeItem(lay);
	delete lay;
	delete x.first;
	delete x.second;
	methods.erase(methods.begin() + index);
	update();
}

void Class::MethodModel(size_t index)
{
}

void Class::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto rc = name_layout->geometry();
	auto rcu = data_layout->geometry();
	auto rcm = methods_layout->geometry();
	auto g = layout()->geometry();

	if (isSelected())
		painter->setPen(QPen{ Qt::white, 2, Qt::PenStyle::DashLine });

	// path for the content of this node
	painter->setBrush(Qt::black);
	painter->drawRoundedRect(g, 5, 5);

	painter->setPen(QPen{ Qt::white, 2 });
	painter->setBrush(QColor(QStringLiteral("#0a091a")));
	painter->drawRect(rcu);

	painter->drawRect(rcm);

	QGraphicsWidget::paint(painter, option, widget);
}

void Class::Init()
{
	auto& u_layout = *new QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	name_layout = new QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	data_layout = new QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	methods_layout = new QGraphicsLinearLayout{ Qt::Orientation::Vertical };

	name_layout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	data_layout->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	methods_layout->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	auto& name = *new QGraphicsProxyWidget;

	QFont f{};
	f.setBold(true);
	l_name->setWordWrap(true);
	l_name->setAlignment(Qt::AlignCenter);
	l_name->setFont(f);
	l_name->setAttribute(Qt::WA_TranslucentBackground);

	name.setWidget(l_name);
	name_layout->addItem(&name);
	u_layout.addItem(name_layout);

	u_layout.addItem(data_layout);
	u_layout.addItem(methods_layout);
	setLayout(&u_layout);


	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setZValue(1.0);
}
