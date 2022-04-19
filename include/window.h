#pragma once
#include <QMainWindow>
#include <SceneView.h>
#include <Scene.h>


class Window : public QMainWindow
{
public:
	Window(uint16_t xwidth, uint16_t xheight)
		:view(&scene)
	{
		resize(xwidth, xheight);
		LoadJson();
	}
	void LoadJson();
private:
	Scene scene;
	SceneView view;
};