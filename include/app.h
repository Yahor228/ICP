#pragma once 
#include <QApplication>
#include "window.h"


class App {


public:
	App(int& xargc, char** xargv);

	int start();
		


private: 
	QApplication app;
	Window wnd;
};