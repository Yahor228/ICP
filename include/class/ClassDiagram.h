#pragma once
#include <Tab.h>
#include <Scene.h>
#include <SceneView.h>

class ClassDiagram : public Tab
{
	Q_OBJECT
public:
	ClassDiagram(std::filesystem::path p);
public:
	virtual void Request(request rq)override;
signals:
	void SelectionChanged(void* c);
protected:
	void Load();
private:
	Scene scene;
	SceneView view;
};
