/**
 * @file app.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief Implementation of application class
 */


#pragma once 
#include <app.h>
#include <qstylefactory.h>
#include "window.h"


App::App(int& xargc, char** xargv)
	:app(xargc,xargv), wnd(1280,720)

{
	QCoreApplication::setApplicationName("Window 1");
	app.setStyle(QStyleFactory::create("Fusion"));

    // Style definition (darkmode)
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
}

int App::start()
{
	wnd.show();
	return app.exec();
}
