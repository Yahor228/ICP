/**
 * @file ClassDiagram.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration class of "Class Diagram"
 */


#pragma once
#include <Tab.h>
#include <Scene.h>
#include <SceneView.h>
#include <memory>


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
	std::shared_ptr<Scene> GetClass()const
	{
		return scene;
	}
signals:
	void SelectionChanged(ISelectable* c);
	void EmptySaved();
protected:	
	/// @brief Function to load JSON version of CD
	void Load();
	void Save();
	void SaveAs();
private:
	std::shared_ptr<Scene> scene;
	SceneView view;
};
