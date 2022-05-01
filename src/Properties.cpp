#include <Properties.h>
#include <QBoxLayout>
#include <QListWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>

#include <Class.h>
#include <util/util.h>



void Properties::EditSelected(Class* node)
{
	if (!node)return setWidget(nullptr);
	auto* w = new Internal(node);
	setWidget(w);
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
	hl2->addWidget(name_l);
	hl2->addWidget(adb, Qt::AlignRight);
	auto* lvd = new QListWidget;
	//do work
	vl->addLayout(hl2);
	vl->addWidget(lvd);

	hl2 = new QHBoxLayout;
	name_l = new QLabel{ qsl("Class Methods:") };
	adb = new QToolButton;
	hl2->addWidget(name_l);
	hl2->addWidget(adb, Qt::AlignRight);
	lvd = new QListWidget;
	//do work
	vl->addLayout(hl2);
	vl->addWidget(lvd);


	setLayout(vl);
}
