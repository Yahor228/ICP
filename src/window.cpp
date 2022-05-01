#include "window.h"
#include <QMenuBar>
#include <QApplication>
#include <commands/commandstack.h>
#include <util/util.h>
#include <Class.h>



Window::Window(uint16_t xwidth, uint16_t xheight)
{
	resize(xwidth, xheight);
	auto* mb = menuBar();
	auto* file = mb->addMenu(qsl("File"));
	auto* diag = mb->addMenu(qsl("Diagram"));
	auto* edit = mb->addMenu(qsl("Edit"));
	auto* cs = diag->addAction(qsl("Create Sequence"), [this]() {
		t.CreateSequence();
		RebindCommands();
		}, QKeySequence::StandardKey::New);

	file->addAction(qsl("Load"), [this, cs]() {
		CommandStack::append();
		t.LoadJson();
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

	connect(&t, &TabWidget::CurrentChanged, [this](int i) {
		CommandStack::set_current(i); 
		RebindCommands(); 
		});
	connect(&t, &TabWidget::SelectionChanged, [this](void* node) {
		prop.EditSelected(static_cast<Class*>(node));
		});



	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, &t);
	setCentralWidget(&t);
	addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, &prop);
	resizeDocks({ &prop }, { 256 }, Qt::Orientation::Horizontal);
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
	t.OnClose();
}
