#include <Class.h>
#include <QJsonArray>
#include <QPainter>


static Access Get(QString s)
{
	switch (s[0].toLatin1())
	{
	case '+':return Access::Public;
	case '~':
	case '-':return Access::Private;
	case '#':return Access::Protected;
	}
	return Access::Private;
}

Class::Class(QString xname, QJsonObject c)
{
	auto& u_layout = *new QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	name_layout = new QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	data_layout = new QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	methods_layout = new QGraphicsLinearLayout{ Qt::Orientation::Vertical };
	auto& name = *new QGraphicsProxyWidget;
	auto* l = new QLabel(xname);

	QFont f{};
	f.setBold(true);
	l->setWordWrap(true);
	l->setAlignment(Qt::AlignCenter);
	l->setFont(f);
	l->setAttribute(Qt::WA_TranslucentBackground);

	name.setWidget(l);
	name_layout->addItem(&name);
	u_layout.addItem(name_layout);

	if (c.contains("Data"))
	{
		auto x = c["Data"].toArray();

		for (auto&& i : x)
		{
			auto arr = i.toArray();
			auto n = arr[1].toString();
			auto a = arr[0].toString();

			auto* l1 = new QGraphicsProxyWidget();
			auto* l2 = new QLabel(a + ' ' + n);
			l2->setAttribute(Qt::WA_TranslucentBackground);
			l1->setWidget(l2);
			data_layout->addItem(l1);

			node.data.emplace(n.toStdU16String(), Get(a));
		}
	}
	u_layout.addItem(data_layout);

	node.name = xname.toStdU16String();

	if (c.contains("Alias"))
		node.alias = c["Alias"].toString().toStdU16String();

	if (c.contains("Methods"))
	{
		auto x = c["Methods"].toArray();

		for (auto&& i : x)
		{
			auto arr = i.toArray();
			auto n = arr[1].toString();
			auto a = arr[0].toString();

			auto* l1 = new QGraphicsProxyWidget();
			auto* l2 = new QLabel(a + ' ' + n);
			l2->setAttribute(Qt::WA_TranslucentBackground);
			l1->setWidget(l2);
			methods_layout->addItem(l1);

			node.methods.emplace(n.toStdU16String(), Get(a));
		}
	}
	u_layout.addItem(methods_layout);

	setLayout(&u_layout);

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setZValue(1.0);
}

void Class::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	auto rc = name_layout->geometry();
	auto rcu = data_layout->geometry();
	auto rcm = methods_layout->geometry();

	// path for the content of this node
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(rc, 5, 5);
	painter->setBrush(Qt::black);
	painter->drawPath(path_content.simplified());

	painter->setBrush(QColor(QStringLiteral("#0a091a")));
	painter->drawRect(rcu);

	painter->drawRect(rcm);

	QGraphicsWidget::paint(painter, option, widget);
}
