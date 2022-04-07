#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <Scene.h>
#include <optional> 


class Window : public QMainWindow
{
public:
	Window(uint16_t xwidth, uint16_t xheight)
	{
		resize(xwidth, xheight);
		LoadJson();
	}
	void LoadJson();
private:
	std::optional<Scene> scene;
};