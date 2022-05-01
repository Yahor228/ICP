#pragma once
#include <QTabWidget>
#include <SceneView.h>
#include <QDockWidget>
#include <Scene.h>

class TabWidget : public QDockWidget
{
	Q_OBJECT
public:
	TabWidget();
public:
	void CreateSequence();
	void LoadJson();
	void OnClose();
signals:
	void CurrentChanged(int i);
	void SelectionChanged(void* node);
private:
	QTabWidget tab;
	Scene scene;
	SceneView view;
	size_t seq = 0;
};