#include <Properties.h>
#include <QListWidget>
#include <QToolButton>
#include <QLabel>

#include <Class.h>
#include <util/util.h>
#include <sequence/texter.h>


void Properties::EditSelected(Class* node)
{
	if (!node)return setWidget(nullptr);
	auto* w = new Internal(node);
	setWidget(w);
}

std::pair<W*, QListWidgetItem*> Internal::MakeEmpty(QListWidget* lvd)
{
	auto* p = new W;
	auto* item = new QListWidgetItem;
	item->setSizeHint(p->sizeHint());
	lvd->addItem(item);
	lvd->setItemWidget(item, p);
	return { p, item };
}

W* Internal::MakeData(data_ty& x)
{
	auto [w, item] = MakeEmpty(data);
	connect(w, &W::DataChanged, [x](const QString& d) {
		x.second->SetText(d);
		});
	connect(w, &W::DeleteRequested, [this, item]() {
		node->EraseData(data->row(item));
		delete item;
		});
	return w;
}
W* Internal::MakeMethod(data_ty& x)
{
	auto [w, item] = MakeEmpty(methods);
	connect(w, &W::DataChanged, [x](const QString& d) {
		x.second->SetText(d);
		});
	connect(w, &W::DeleteRequested, [this, item]() {
		node->EraseMethod(methods->row(item));
		delete item;
		});
	return w;
}

Internal::Internal(Class* xnode)
	:node(xnode)
{
	auto* vl = new QVBoxLayout;
	auto* hl1 = new QHBoxLayout;
	auto* name_l = new QLabel{ qsl("Class Name:") };
	hl1->addWidget(name_l);
	auto* name = new QLineEdit;
	name->setText(node->GetName());
	connect(name, &QLineEdit::textEdited, [this](const QString& s) {
		node->SetName(s);
		});
	hl1->addWidget(name);
	vl->addLayout(hl1);


	auto* hl2 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Data:") };
	auto* adb = new QToolButton;
	data = new QListWidget;

	adb->setText(qsl("+"));
	connect(adb, &QToolButton::pressed, [this]() {
		auto &p = node->AppendData();
		p.first->SetText(qsl("~"));
		MakeData(p);
		});


	hl2->addWidget(name_l);
	hl2->addWidget(adb, Qt::AlignRight);


	
	for (auto &i : xnode->Data())
	{
		auto* w = MakeData(i);
		w->le.setText(i.second->toPlainText());
	}


	//do work
	vl->addLayout(hl2);
	vl->addWidget(data);

	hl2 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Methods:") };
	adb = new QToolButton;
	adb->setText(qsl("+"));
	connect(adb, &QToolButton::pressed, [this]() {
		auto& p = node->AppendMethod();
		p.first->SetText(qsl("~"));
		MakeMethod(p);
		});

	hl2->addWidget(name_l);
	hl2->addWidget(adb, Qt::AlignRight);
	methods = new QListWidget;

	for (auto& i : xnode->Methods())
	{
		auto* w = MakeMethod(i);
		w->le.setText(i.second->toPlainText());
	}
	vl->addLayout(hl2);
	vl->addWidget(methods);
	setLayout(vl);
}

W::W()
{
	lay.addWidget(&le);
	lay.addWidget(&cbox);
	lay.addWidget(&delet);
	setLayout(&lay);

	connect(&le, &QLineEdit::textChanged, this, &W::DataChanged);
	connect(&delet, &QToolButton::pressed, this, &W::DeleteRequested);
	connect(&cbox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &W::AccessChanged);

	for (auto i : access_strings)
		cbox.addItem(i.data());

	delet.setText(qsl("X"));
}

void W::SetData(std::u16string* d)
{
	data = d;
	le.setText(QString::fromUtf16(d->data(), d->size()));
}
