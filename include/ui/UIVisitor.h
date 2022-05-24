/**
 * @file UIVisitor.h
 * @author Yahor Senichak (xsenic00)
 * @brief 
 */



#pragma once
#include <model/ChangeMode.h>

class Class;

class UIVisitor
{
	using STy = const QString&;
public:
	enum class Mode
	{
		normal,
		inherited
	};
public:
	UIVisitor(ChangeMode c, Class& node):node(node), change(c) {}
public:
	void InsertMethod(STy acc, STy name);
	void InsertData(STy acc, STy name);
	void SetMode(Mode md) { mode = md; }
	ChangeMode Change()const { return  change; }
private:
	Class& node;
	Mode mode = Mode::normal;
	ChangeMode change;
};