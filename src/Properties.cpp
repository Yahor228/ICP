#include <Properties.h>
#include <class/connection.h>

#include <sequence/Element.h>
#include <model/node.h>
#include <ISelectable.h>
#include <util/util.h>
#include <array>
#include <commands/remove_data.h>
#include <commands/commandstack.h>

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

void Properties::EditSelected(ISelectable* node)
{
	if (!node)
		return setWidget(nullptr);
	switch (node->XType())
	{
	case ISelectable::node:
		inter.Refill(static_cast<Node*>(node));
		return setWidget(&inter);
	case ISelectable::connection:
		connector.Refill(static_cast<Connection*>(node));
		return setWidget(&connector);
	case ISelectable::element:
		elem.Refill(static_cast<Element*>(node));
		return setWidget(&elem);
	default:
		break;
	}
}

void Internal::Refill(Node* xnode)
{
	node = xnode;
	name.setText(node->Name());
	alias.setText(node->Alias());

	data.clear();
	methods.clear();
	for (auto& i : xnode->Data())
		MakeData(new W(i.acc, i.Name));
	for (auto& i : xnode->LocalMethods())
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
		size_t i = data.row(item);
		delete item;
		CommandStack::current().push(new RemoveDataCommand(node, i, node->Data()[i]));
		});
}
void Internal::MakeMethod(W* w)
{
	auto item = MakeEmpty(methods, w);
	auto x = methods.row(item);
	connect(w, &W::DataChanged, [this, x](const QString& d) {
		node->LocalMethods()[x].Name = d;
		node->Update(ChangeMode::methods);
		});
	connect(w, &W::AccessChanged, [this, x](const QString& d) {
		node->LocalMethods()[x].acc = d;
		node->Update(ChangeMode::methods);
		});
	connect(w, &W::DeleteRequested, [this, item]() {
		size_t i = methods.row(item);
		delete item;
		CommandStack::current().push(new RemoveMethodCommand(node, i, node->LocalMethods()[i]));
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

	hl1 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Alias:") };
	hl1->addWidget(name_l);
	connect(&alias, &QLineEdit::textEdited, [this](const QString& s) {
		node->SetAlias(s);
		});
	hl1->addWidget(&alias);
	vl->addLayout(hl1);

	hl1 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Data:") };

	add_data.setText(qsl("+"));
	connect(&add_data, &QToolButton::pressed, [this]() {
		node->InsertData(qsl("-"), "");
		MakeData(new W(qsl("-"), ""));
		});

	hl1->addWidget(name_l);
	hl1->addWidget(&add_data, Qt::AlignRight);


	//do work
	vl->addLayout(hl1);
	vl->addWidget(&data);

	hl1 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Methods:") };
	add_method.setText(qsl("+"));
	connect(&add_method, &QToolButton::pressed, [this]() {
		node->InsertMethod(qsl("-"), "");
		MakeMethod(new W(qsl("-"), ""));
		});

	hl1->addWidget(name_l);
	hl1->addWidget(&add_method, Qt::AlignRight);

	vl->addLayout(hl1);
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


constexpr std::array<const char*, 4> relations{
	"1",
	"0..1",
	"0..n",
	"1..n",
};

ConnectionEditor::ConnectionEditor()
	:msg(qsl("Label: ")), valid(QRegularExpression{qsl(R"(^\d+(?:\.\.(?:\d+|n))?$)")})
{
	auto* hl1 = new QHBoxLayout;

	hl1->addWidget(&msg);
	hl1->addWidget(&message);
	main_lay.setAlignment(Qt::AlignTop);

	connect(&message, &QLineEdit::textEdited, [this](const QString& str) {conn->ChangeText(str); });
	main_lay.addLayout(hl1);

	rel_from.setEditable(true);
	rel_to.setEditable(true);



	grp.setTitle(qsl("Coordinality:"));

	hl1 = new QHBoxLayout;
	auto* clab = new QLabel{ qsl("From:") };
	lay.addWidget(clab);
	lay.addWidget(&rel_from);
	clab = new QLabel{ qsl("To:") };
	lay.addWidget(clab);
	lay.addWidget(&rel_to);
	lay.addLayout(hl1);
	std::for_each(relations.begin(), relations.end(), [this](const char* a) {rel_from.addItem(a); rel_to.addItem(a); });
	grp.setLayout(&lay);
	main_lay.addWidget(&grp);

	connect(&rel_from, &QComboBox::currentTextChanged, [this](const QString& str) {
		conn->ChangeRelFrom(str);
		});
	connect(&rel_to, &QComboBox::currentTextChanged, [this](const QString& str) {
		conn->ChangeRelTo(str);
		});
	rel_from.setValidator(&valid);
	rel_to.setValidator(&valid);
	rel_from.setInsertPolicy(QComboBox::NoInsert);
	rel_to.setInsertPolicy(QComboBox::NoInsert);

	setLayout(&main_lay);
}

void ConnectionEditor::Refill(Connection* xnode)
{
	conn = xnode;
	grp.setVisible(conn->GetType() == Connection::Type::asoc);
	message.setText(conn->Text());
	rel_from.setCurrentText(conn->RelFrom());
	rel_to.setCurrentText(conn->RelTo());
}


ElementEditor::ElementEditor()
	:x(qsl("Instance Name: "))
{
	lay.addWidget(&x);
	lay.addWidget(&le);
	main_lay.setAlignment(Qt::AlignTop);
	main_lay.addLayout(&lay);
	setLayout(&main_lay);
	connect(&le, &QLineEdit::textEdited, [this](const QString& str) {node->ChangeName(str); });
}

void ElementEditor::Refill(Element* xnode)
{
	node = xnode;
	le.setText(node->Name());
}
