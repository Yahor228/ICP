#pragma once
#include <QDockWidget>
#include <QPlainTextEdit>



class Logger : public QDockWidget
{
public:
	Logger()
	{
		setWidget(&text);
		log = this;
	}
public:
	static Logger* current()
	{
		return log;
	}
	static void Warn(const QString& message)
	{
		auto* l = current();
		if (!l)return;
		l->text.appendPlainText(message);
	}
	static void Clear()
	{
		auto* l = current();
		if (!l)return;
		l->text.clear();
	}
private:
	QPlainTextEdit text;
	static Logger* log;
};

