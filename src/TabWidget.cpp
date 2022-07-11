#include <TabWidget.h>
#include <commands/commandstack.h>
#include <sequence/Sequence.h>
#include <util/util.h>
#include <QJsonDocument>
#include <QFileDialog>
#include <QVBoxLayout>
#include <fstream>
#include <ui/Log.h>
#include <class/ClassDiagram.h>


namespace fs = std::filesystem;

TabWidget::TabWidget()
{
	setTabsClosable(true);
	connect(this, &QTabWidget::currentChanged, [this](int index) { 
		CurrentChanged(index);  
		if (index < 0)return;
		static_cast<Tab*>(widget(index))->OnEnter();
		});
	connect(this, &QTabWidget::tabCloseRequested, [this](int index) {
		CloseRequested(index);
		auto str = tabText(index).toStdU16String();
		auto* w = static_cast<Tab*>(widget(index));
		w->Request(Tab::request::Close);
		tabs.erase(str);
		delete w;
		});
}

void TabWidget::CreateSequence()
{
	auto* tab = static_cast<Tab*>(currentWidget());
	auto&p = tab->ClassDiagPath();
	auto* xtab = new SequenceDiagram({});
	xtab->SetClassPath(p);
	xtab->SetClass(tab->GetClass());
	connect(xtab, &SequenceDiagram::FindDiagram, [this, xtab](const std::filesystem::path& p) {
		auto it = tabs.find(p.filename().u16string());
		if(it!=tabs.end())
			xtab->SetClass(it->second->GetClass());
		});
	connect(xtab, &SequenceDiagram::SelectionChanged, this, &TabWidget::SelectionChanged);
	connect(xtab, &SequenceDiagram::EmptySaved, [this, xtab]() {
		auto fn = xtab->Path().filename().u16string();
		if (auto x = tabs.find(fn); x != tabs.end())
			tabCloseRequested(indexOf(x->second));
		setTabText(indexOf(xtab), QString::fromStdU16String(fn));
		tabs.emplace(std::move(fn), xtab);
		});

	addTab(xtab, qsl("Sequence Diagram %1").arg(seq++));
	setCurrentIndex(indexOf(xtab));
}
void TabWidget::OnClose()
{
	disconnect();
	for (int i = 0; i < count(); i++)
		static_cast<Tab*>(widget(i))->Request(Tab::request::Close);
}
void TabWidget::RemoveSelected()
{
	static_cast<Tab*>(currentWidget())->Request(Tab::request::Delete);
}

void TabWidget::LoadSequence()
{
	fs::path p{ QFileDialog::getOpenFileName(nullptr, "Find Class Diagram", "", "JSON (*.json);;All files (*.*)").toStdU16String() };
	if (p.empty()) return;

	auto fn = p.filename().u16string();
	if (tabs.contains(fn))return;

	std::fstream t;
	t.open(p, std::ios::in);

	std::string str;
	t.seekg(0, std::ios::end);
	int x = t.tellg();
	str.reserve(x);
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	if (str.empty())return;

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e).object();
	if (e.error != QJsonParseError::NoError)
	{
		Logger::Warn(e.errorString());
		return;
	}
	if (!json.contains(qsl("Class Diagram")))return;
	auto xp = json["Class Diagram"].toString().toStdU16String();

	LoadJson(xp);

	auto* tab = static_cast<Tab*>(currentWidget());
	auto& cp = tab->ClassDiagPath();
	auto* xtab = new SequenceDiagram(std::move(p));
	xtab->SetClassPath(cp);
	xtab->SetClass(tab->GetClass());
	xtab->Load(json);
	connect(xtab, &SequenceDiagram::SelectionChanged, this, &TabWidget::SelectionChanged);

	addTab(xtab, QString::fromStdU16String(fn));
	tabs.emplace(std::move(fn), xtab);
	setCurrentIndex(indexOf(xtab));
}


void TabWidget::SendRequest(Tab::request rq)
{
	auto* cw = currentWidget();
	if (!cw)return;
	static_cast<Tab*>(cw)->Request(rq);
}
void TabWidget::LoadJson()
{
	fs::path p{ QFileDialog::getOpenFileName(nullptr, "Find Class Diagram", "", "JSON (*.json);;All files (*.*)").toStdU16String() };
	if (p.empty()) return;

	LoadJson(p);
}

void TabWidget::LoadJson(std::filesystem::path p)
{
	auto fn = p.filename().u16string();
	if (tabs.contains(fn))return setCurrentWidget(tabs[fn]);

	CommandStack::append();
	auto* xtab = new ClassDiagram(std::move(p));
	connect(xtab, &ClassDiagram::SelectionChanged, this, &TabWidget::SelectionChanged);

	addTab(xtab, QString::fromStdU16String(fn));
	tabs.emplace(std::move(fn), xtab);
	setCurrentIndex(indexOf(xtab));
}

void TabWidget::NewDiagram()
{
	auto* xtab = new ClassDiagram({});
	connect(xtab, &ClassDiagram::SelectionChanged, this, &TabWidget::SelectionChanged);
	connect(xtab, &ClassDiagram::EmptySaved, [this, xtab]() {
		auto fn = xtab->Path().filename().u16string();
		if (auto x = tabs.find(fn); x!=tabs.end())
			tabCloseRequested(indexOf(x->second));
		setTabText(indexOf(xtab), QString::fromStdU16String(fn));
		tabs.emplace(std::move(fn), xtab);
		});

	addTab(xtab, qsl("Class Diagram %1").arg(seq++));
	setCurrentIndex(indexOf(xtab));
}
