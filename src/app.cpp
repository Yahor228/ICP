#pragma once 
#include <app.h>
#include <qstylefactory.h>
#include "window.h"

App::App(int& xargc, char** xargv)
	:app(xargc,xargv), wnd(1280,720)

{
	QCoreApplication::setApplicationName("Window 1");
	app.setStyle(QStyleFactory::create("Fusion"));
}

int App::start()
{
	wnd.show();
	return app.exec();
}
