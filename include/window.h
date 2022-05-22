/**
 * @file window.h
 * @author Yahor Senichak (xsenic00)
 * @brief Main Window Class
 */


#pragma once
#include <QMainWindow>
#include <TabWidget.h>
#include <Properties.h>
#include <ui/Log.h>



class Window : public QMainWindow
{
public:
	/// @brief Create window with size peremeters
	/// @param xwidth 
	/// @param xheight 
	Window(uint16_t xwidth, uint16_t xheight);
public:
	/// @brief Rebind undo and redo for selected tab
	void RebindCommands();
	/// @brief Function to clean resources(stack, scene)
	/// @param event [unused]
	void closeEvent(QCloseEvent* event)override; 
private:
	TabWidget t; //< Relay of the tabs 
	Properties prop; //< Node parameters: class name, methods , etc.
	Logger l;  

	QAction* undo;
	QAction* redo;
	bool is_init = false; 
};