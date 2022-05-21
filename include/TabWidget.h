#pragma once
#include <QTabWidget>
#include <SceneView.h>
#include <QDockWidget>
#include <Scene.h>
#include <unordered_set>

class TabWidget : public QTabWidget
{
	Q_OBJECT
public:
	TabWidget();
public:
	void CreateSequence();
	void LoadJson();
	void OnClose();
	void RemoveSelected();
signals:
	void CurrentChanged(int i);
	void SelectionChanged(void* node);
	void CloseRequested(int i);
private:
	std::unordered_set<std::u16string> tabs;
	size_t seq = 0;
};