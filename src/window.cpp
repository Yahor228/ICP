#include "window.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QMenuBar>
#include <QApplication>
#include <sequence/Sequence.h>
#include <filesystem>
#include <fstream>
#include <commands/commandstack.h>


namespace fs = std::filesystem;
#define qsl QStringLiteral

Window::Window(uint16_t xwidth, uint16_t xheight)
	:view(&scene)
{
	resize(xwidth, xheight);
	auto* mb = menuBar();
	auto* file = mb->addMenu(qsl("File"));
	auto* diag = mb->addMenu(qsl("Diagram"));
	auto* edit = mb->addMenu(qsl("Edit"));
	auto* cs = diag->addAction(qsl("Create Sequence"), [this]() {
		CommandStack::append();
		tab.addTab(new SequenceTab(scene.Nodes()), qsl("Sequence %1").arg(seq++));
		tab.setCurrentIndex(tab.currentIndex() + 1);
		RebindCommands();
		}, QKeySequence::StandardKey::New);

	file->addAction(qsl("Load"), [this, cs]() {
		LoadJson();
		cs->setDisabled(false);
		}, QKeySequence::StandardKey::Open);
	file->addSeparator();
	file->addAction(qsl("Exit"), []() {
		qApp->closeAllWindows();
		}, QKeySequence::StandardKey::Quit);

	undo = edit->addAction(qsl("Undo"), []() {
		CommandStack::current().undo();
		}, QKeySequence::StandardKey::Undo);
	redo = edit->addAction(qsl("Redo"), []() {
		CommandStack::current().redo();
		}, QKeySequence::StandardKey::Redo);

	undo->setEnabled(false);
	redo->setEnabled(false);
	//cs->setDisabled(true);

	connect(&tab, &QTabWidget::currentChanged, [this](int i) {
		if (!i)return;
		CommandStack::set_current(i - 1); RebindCommands(); 
		});
	LoadJson();
	setCentralWidget(&tab);
}

void Window::LoadJson()
{
	//fs::path p{ QFileDialog::getOpenFileName(nullptr, "Find Class Diagram", "", "All files (*.*);;JSON (*.json))").toStdString() };
	fs::path p{ R"(C:\Users\Agrae\Source\Repos\ICP\examples\cd.json)" };
	if (p.empty()) return;

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
	tab.addTab(&view, QString::fromStdU16String(p.filename().u16string()));
	is_init = true;
}

void Window::RebindCommands()
{
	auto& cs = CommandStack::current();
	undo->setEnabled(cs.canUndo());
	redo->setEnabled(cs.canRedo());

	connect(&cs, &QUndoStack::canUndoChanged, [this](bool b) {undo->setEnabled(b); });
	connect(&cs, &QUndoStack::canRedoChanged, [this](bool b) {redo->setEnabled(b); });
}

void Window::closeEvent(QCloseEvent* event)
{
	tab.disconnect();
}
