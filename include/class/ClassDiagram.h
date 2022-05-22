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
	virtual const std::filesystem::path& ClassDiagPath()const override
	{
		return Path();
	}

signals:
	void SelectionChanged(void* c);
	void EmptySaved();
protected:
	void Load();
	void Save();
	void SaveAs();
private:
	Scene scene;
	SceneView view;
};
