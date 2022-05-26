/**
 * @file window.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief Implementation of main window
 */


#include "window.h"
#include <QMenuBar>
#include <QApplication>
#include <commands/commandstack.h>
#include <util/util.h>



Window::Window(uint16_t xwidth, uint16_t xheight)

{
	resize(xwidth, xheight);
	auto* mb = menuBar();
	auto* file = mb->addMenu(qsl("File"));
	auto* diag = mb->addMenu(qsl("Diagram"));
	auto* edit = mb->addMenu(qsl("Edit"));
	auto* log = mb->addMenu(qsl("Log"));
	auto* cs = diag->addAction(qsl("Create Sequence"), [this]() {
		CommandStack::append();
		t.CreateSequence();
		RebindCommands();
		});
	// Create a visual programm bar 
	diag->addAction(qsl("Remove Selected"), [this]() {
		t.RemoveSelected();
		}, QKeySequence::StandardKey::Delete);
	diag->addAction(qsl("Reverse Connection"), [this]() {
		t.SendRequest(Tab::Rev);
		}, qsl("R"));

	file->addAction(qsl("New"), [this, cs]() {
		CommandStack::append();
		t.NewDiagram();
		cs->setEnabled(true);
		}, QKeySequence::StandardKey::New);
	file->addAction(qsl("Load"), [this, cs]() {
		CommandStack::append();
		t.LoadJson();
		cs->setEnabled(true);
		}, QKeySequence::StandardKey::Open);

	file->addAction(qsl("Save"), [this, cs]() {
		t.SendRequest(Tab::Save);
		}, QKeySequence::StandardKey::Save);
	file->addAction(qsl("Save As"), [this, cs]() {
		t.SendRequest(Tab::SaveAs);
		}, tr("Ctrl+Shift+S"));


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

	log->addAction(qsl("Clear"), []() {Logger::Clear(); });

	undo->setEnabled(false);
	redo->setEnabled(false);
	cs->setEnabled(false);

	connect(&t, &TabWidget::CurrentChanged, [this](int i) {
		if (i == -1)
		{
			undo->setEnabled(false);
			redo->setEnabled(false);
			return;
		}
		CommandStack::set_current(i); 
		prop.EditSelected(nullptr);
		RebindCommands(); 
		});
	connect(&t, &TabWidget::SelectionChanged, [this](ISelectable* node) {
		prop.EditSelected(node);
		});
	connect(&t, &TabWidget::CloseRequested, [this](int i) {
		CommandStack::remove_at(i);
		});


	setCentralWidget(&t);
	addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, &prop);
	addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, &l);
	resizeDocks({ &prop }, { 256 }, Qt::Orientation::Horizontal);
	resizeDocks({ &l }, { 128 }, Qt::Orientation::Vertical);
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
	CommandStack::clear();
	t.OnClose();
}
