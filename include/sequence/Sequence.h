#pragma once
#include <Tab.h>
#include <SceneView.h>
#include <sequence/SeqScene.h>



class Scene;

class SequenceDiagram :public Tab
{
	Q_OBJECT
public:
	SequenceDiagram(std::filesystem::path self);
public:
	void SetClassPath(std::filesystem::path xclass_diag);
	void Request(request rq)override;
	void SetClass(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> GetClass()const;
	virtual const std::filesystem::path& ClassDiagPath()const override;
	void OnEnter()override;
signals:
	void FindDiagram(const std::filesystem::path& p);
	void EmptySaved();
private:
	std::filesystem::path class_diag;
	SeqScene scene;
	SceneView view;
};