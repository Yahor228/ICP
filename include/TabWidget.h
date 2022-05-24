/**
 * @file TabWidget.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief creation a main scene
 */


#pragma once
#include <QTabWidget>
#include <SceneView.h>
#include <QDockWidget>
#include <Scene.h>
#include <unordered_set>
#include <Tab.h>

class TabWidget : public QTabWidget
{
	Q_OBJECT
public:
	TabWidget();
public:
	void CreateSequence();
	void LoadJson();
	void NewDiagram();
	void OnClose();
	void RemoveSelected();
	void SendRequest(Tab::request rq);
signals:
	void CurrentChanged(int i);
	void SelectionChanged(void* node);
	void CloseRequested(int i);
private:
	std::unordered_map<std::u16string, Tab*> tabs;
	size_t seq = 0;
};