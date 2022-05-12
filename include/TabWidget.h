#pragma once
#include <QTabWidget>
#include <SceneView.h>
#include <QDockWidget>
#include <Scene.h>
#include <unordered_set>

class TabWidget : public QWidget
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
	std::unordered_set<std::u16string> tabs;
};