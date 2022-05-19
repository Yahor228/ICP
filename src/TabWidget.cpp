#include <TabWidget.h>
#include <commands/commandstack.h>
#include <sequence/Sequence.h>
#include <util/util.h>
#include <fstream>
#include <filesystem>
#include <QJsonDocument>
#include <QFileDialog>
#include <QVBoxLayout>


namespace fs = std::filesystem;

TabWidget::TabWidget()
	:view(&scene)
{
	auto* lay = new QVBoxLayout;
	lay->addWidget(&tab);
	lay->setContentsMargins(0, 0, 0, 0);
	connect(&tab, &QTabWidget::currentChanged, this, &TabWidget::CurrentChanged);
	connect(&scene, &Scene::SelectionChanged, this, &TabWidget::SelectionChanged);
	setLayout(lay);
}

void TabWidget::CreateSequence()
{
	//CommandStack::append();
	//tab.addTab(new SequenceTab(scene.Nodes()), qsl("Sequence %1").arg(seq++));
	//tab.setCurrentIndex(tab.currentIndex() + 1);
}
void TabWidget::OnClose()
{
	tab.disconnect();
	scene.disconnect();
}
void TabWidget::RemoveSelected()
{
	scene.RemoveSelected();
}
void TabWidget::LoadJson()
{
	//fs::path p{ QFileDialog::getOpenFileName(nullptr, "Find Class Diagram", "", "All files (*.*);;JSON (*.json))").toStdString() };
	fs::path p{ R"(C:\Users\Agrae\Source\Repos\ICP\examples\cd1.json)" };
	if (p.empty()) return;

	auto fn = p.filename().u16string();
	if (tabs.contains(fn))return;

	std::fstream t;
	t.open(p, std::ios::in);

	std::string str;
	t.seekg(0, std::ios::end);

	//preallocation
	int x = t.tellg();
	str.reserve(x);
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	if (str.empty())return;

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e).object();
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return; }
	scene.LoadFrom(json);
	scene.setSceneRect(-32000, -32000, 64000, 64000);


	tab.addTab(&view, QString::fromStdU16String(fn));
	tabs.emplace(std::move(fn));
}
