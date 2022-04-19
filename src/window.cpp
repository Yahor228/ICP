#include "window.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void Window::LoadJson()
{
	fs::path p{ QFileDialog::getOpenFileName(nullptr, "Find Class Diagram", "", "All files (*.*);;JSON (*.json))").toStdString() };
	if (p.empty()) return;

	std::fstream t;
	t.open(p, std::ios::in);

	std::string str;
	t.seekg(0, std::ios::end);

	//preallocation
	int x = t.tellg();
	str.reserve(x);
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	if (str.empty())return;

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e).object();
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return; }
	scene.LoadFrom(json);

	setCentralWidget(&view);
}