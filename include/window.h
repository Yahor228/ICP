#pragma once
#include <QMainWindow>
#include <TabWidget.h>
#include <Properties.h>



class Window : public QMainWindow
{
public:
	Window(uint16_t xwidth, uint16_t xheight);
public:
	void RebindCommands();
	void closeEvent(QCloseEvent* event)override;
private:
	TabWidget t;
	Properties prop;

	QAction* undo;
	QAction* redo;
	bool is_init = false;
};