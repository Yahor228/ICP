#include <Properties.h>
#include <QListWidget>
#include <QToolButton>
#include <QLabel>

#include <Class.h>
#include <util/util.h>



void Properties::EditSelected(Class* node)
{
	if (!node)return setWidget(nullptr);
	auto* w = new Internal(node);
	setWidget(w);
}

W* Internal::MakeEmpty(QListWidget* lvd)
{
	auto* p = new W;
	auto* item = new QListWidgetItem;
	item->setSizeHint(p->sizeHint());
	lvd->addItem(item);
	lvd->setItemWidget(item, p);
	return p;
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

	auto& model = xnode->Model();


	auto* hl2 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Data:") };
	auto* adb = new QToolButton;
	hl2->addWidget(name_l);
	hl2->addWidget(adb, Qt::AlignRight);
	auto* lvd = new QListWidget;

	for (auto& i : model.data)
	{
		auto* w = MakeEmpty(lvd);
		w->le.setText(QString::fromStdU16String(i.first));
	}


	//do work
	vl->addLayout(hl2);
	vl->addWidget(lvd);

	hl2 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Methods:") };
	adb = new QToolButton;
	hl2->addWidget(name_l);
	hl2->addWidget(adb, Qt::AlignRight);
	lvd = new QListWidget;

	for (auto& i : model.methods)
	{
		auto* w = MakeEmpty(lvd);
		w->le.setText(QString::fromStdU16String(i.first));
	}
	//do work
	vl->addLayout(hl2);
	vl->addWidget(lvd);


	setLayout(vl);
}

W::W()
{
	lay.addWidget(&le);
	lay.addWidget(&cbox);
	lay.addWidget(&delet);
	setLayout(&lay);
}
