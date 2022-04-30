#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <SceneView.h>
#include <Scene.h>


class Window : public QMainWindow
{
public:
	Window(uint16_t xwidth, uint16_t xheight);
public:
	void LoadJson();
	void RebindCommands();
	void closeEvent(QCloseEvent* event)override;
private:
	QTabWidget tab;
	Scene scene;
	SceneView view;

	QAction* undo;
	QAction* redo;
	size_t seq = 0;
	bool is_init = false;
};