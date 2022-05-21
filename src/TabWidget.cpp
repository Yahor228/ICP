#include <TabWidget.h>
#include <commands/commandstack.h>
#include <sequence/Sequence.h>
#include <util/util.h>
#include <QJsonDocument>
#include <QFileDialog>
#include <QVBoxLayout>
#include <class/ClassDiagram.h>


namespace fs = std::filesystem;

TabWidget::TabWidget()
{
	setTabsClosable(true);
	connect(this, &QTabWidget::currentChanged, this, &TabWidget::CurrentChanged);
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
	//CommandStack::append();
	//tab.addTab(new SequenceTab(scene.Nodes()), qsl("Sequence %1").arg(seq++));
	//tab.setCurrentIndex(tab.currentIndex() + 1);
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
void TabWidget::LoadJson()
{
	//fs::path p{ QFileDialog::getOpenFileName(nullptr, "Find Class Diagram", "", "All files (*.*);;JSON (*.json))").toStdString() };
	fs::path p{ R"(C:\Users\Agrae\Source\Repos\ICP\examples\cd1.json)" };
	if (p.empty()) return;

	auto fn = p.filename().u16string();
	if (tabs.contains(fn))return;

	auto* xtab = new ClassDiagram(std::move(p));
	connect(xtab, &ClassDiagram::SelectionChanged, this, &TabWidget::SelectionChanged);

	addTab(xtab, QString::fromStdU16String(fn));
	tabs.emplace(std::move(fn));
}
