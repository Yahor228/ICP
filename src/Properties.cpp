#include <Properties.h>
#include <QLabel>

#include <model/node.h>
#include <util/util.h>
#include <array>

constexpr std::array<const char*, 4> access_strings
{
	"Public",
	"Default",
	"Private",
	"Protected",
};
constexpr std::array<char, 5> access{ "+~-#" };

inline int GetAccess(const QString& s)
{
	switch (s[0].toLatin1())
	{
	case '+':return 0;
	case '~':return 1;
	case '-':return 2;
	case '#':return 3;
	}
	return 2;
}

void Properties::EditSelected(Node* node)
{
	if (!node)
		return setWidget(nullptr);
	inter.Refill(node);
	setWidget(&inter);
}

void Internal::Refill(Node* xnode)
{
	node = xnode;
	name.setText(node->Name());

	data.clear();
	methods.clear();
	for (auto& i : xnode->Data())
		MakeData(new W(i.acc, i.Name));
	for (auto& i : xnode->Methods())
		MakeMethod(new W(i.acc, i.Name));
}

QListWidgetItem* Internal::MakeEmpty(QListWidget& lvd, W* it)
{
	auto* item = new QListWidgetItem;
	item->setSizeHint(it->sizeHint());
	lvd.addItem(item);
	lvd.setItemWidget(item, it);
	return item;
}

void Internal::MakeData(W* w)
{
	auto item = MakeEmpty(data, w);
	auto x = data.row(item);
	connect(w, &W::DataChanged, [this, x](const QString& d) {
		node->Data()[x].Name = d;
		node->Update(ChangeMode::data);
		});
	connect(w, &W::AccessChanged, [this, x](const QString& d) {
		node->Data()[x].acc = d;
		node->Update(ChangeMode::data);
		});
	connect(w, &W::DeleteRequested, [this, item]() {
		node->RemoveData(data.row(item));
		delete item;
		});
}
void Internal::MakeMethod(W* w)
{
	auto item = MakeEmpty(methods, w);
	auto x = methods.row(item);
	connect(w, &W::DataChanged, [this, x](const QString& d) {
		node->Methods()[x].Name = d;
		node->Update(ChangeMode::methods);
		});
	connect(w, &W::AccessChanged, [this, x](const QString& d) {
		node->Methods()[x].acc = d;
		node->Update(ChangeMode::methods);
		});
	connect(w, &W::DeleteRequested, [this, item]() {
		node->RemoveMethod(methods.row(item));
		delete item;
		});
}
Internal::Internal()
{
	auto* vl = new QVBoxLayout;
	auto* hl1 = new QHBoxLayout;
	auto* name_l = new QLabel{ qsl("Class Name:") };
	hl1->addWidget(name_l);
	connect(&name, &QLineEdit::textEdited, [this](const QString& s) {
		node->SetName(s);
		});
	hl1->addWidget(&name);
	vl->addLayout(hl1);

	auto* hl2 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Data:") };

	add_data.setText(qsl("+"));
	connect(&add_data, &QToolButton::pressed, [this]() {
		node->InsertData(qsl("-"), "");
		MakeData(new W(qsl("-"), ""));
		});

	hl2->addWidget(name_l);
	hl2->addWidget(&add_data, Qt::AlignRight);


	//do work
	vl->addLayout(hl2);
	vl->addWidget(&data);

	hl2 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Methods:") };
	add_method.setText(qsl("+"));
	connect(&add_method, &QToolButton::pressed, [this]() {
		node->InsertMethod(qsl("-"), "");
		MakeMethod(new W(qsl("-"), ""));
		});

	hl2->addWidget(name_l);
	hl2->addWidget(&add_method, Qt::AlignRight);

	vl->addLayout(hl2);
	vl->addWidget(&methods);
	setLayout(vl);
}

W::W(const QString& acc, const QString& name)
{
	for (auto i : access_strings)
		cbox.addItem(i);

	delet.setText(qsl("X"));

	lay.addWidget(&le);
	lay.addWidget(&cbox);
	lay.addWidget(&delet);
	setLayout(&lay);

	cbox.setCurrentIndex(GetAccess(acc));
	le.setText(name);


	connect(&le, &QLineEdit::textChanged, this, &W::DataChanged);
	connect(&le, &QLineEdit::editingFinished, this, &W::DataChangeFinished);
	connect(&delet, &QToolButton::pressed, this, &W::DeleteRequested);
	connect(&cbox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int a) {
		W::AccessChanged({ access[a] }); });
}
