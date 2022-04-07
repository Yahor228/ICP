#include <Class.h>
#include <QJsonArray>


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
	auto* l = new QLabel(xname);
	name.setWidget(l);
	name_layout.addItem(&name);

	if (c.contains("Data"))
	{
		auto x = c["Data"].toArray();

		for (auto&& i : x)
		{
			auto arr = i.toArray();
			auto n = arr[1].toString();
			auto a = arr[0].toString();

			auto* l1 = new QGraphicsProxyWidget();
			l1->setWidget(new QLabel(a + ' ' + n));
			data_layout.addItem(l1);

			data.emplace(n, Get(a));
		}
	}
	name_layout.addItem(&data_layout);

	if (c.contains("Methods"))
	{
		auto x = c["Methods"].toArray();

		for (auto&& i : x)
		{
			auto arr = i.toArray();
			auto n = arr[1].toString();
			auto a = arr[0].toString();

			auto* l1 = new QGraphicsProxyWidget();
			l1->setWidget(new QLabel(a + ' ' + n));
			data_layout.addItem(l1);

			methods.emplace(n, Get(a));
		}
	}
	name_layout.addItem(&methods_layout);

	if (c.contains("Alias"))
	{
		alias = c["Alias"].toString();
	}
}