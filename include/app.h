/**
 * @file app.h
 * @author Yahor Senichak (xsenic00)
 * @brief Application class
 */


#pragma once 
#include <QApplication>
#include "window.h"

/// @brief Application class 
class App 
{
public:
	App(int& xargc, char** xargv);
public:
	/// @brief The start function of the programm
	/// @return finish code
	int start();
private: 
	QApplication app; //< Application parameters
	Window wnd;
};