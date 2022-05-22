#include <class/ClassDiagram.h>
#include <fstream>
#include <QJsonDocument>
#include <QVBoxLayout>
#include <QFileDialog>
#include <util/util.h>
#include <ui/Log.h>

ClassDiagram::ClassDiagram(std::filesystem::path p)
	:view(&scene), Tab(std::move(p))
{
	connect(&scene, &Scene::SelectionChanged, this, &ClassDiagram::SelectionChanged);

	auto* lay = new QVBoxLayout;
	lay->addWidget(&view);
	lay->setContentsMargins(0, 0, 0, 0);
	setLayout(lay);
	scene.setSceneRect(-32000, -32000, 64000, 64000);
	Load();
}

void ClassDiagram::Request(request rq)
{
	switch (rq)
	{
	case Tab::None:
		break;
	case Tab::Save:
		if (!Path().empty())Save();
		[[fallthrough]];
	case Tab::SaveAs:
		SaveAs();
		break;
	case Tab::Close:
		scene.disconnect();
		break;
	case Tab::Delete:
		scene.RemoveSelected();
		break;
	default:
		break;
	}
}

void ClassDiagram::Load()
{
	auto& p = Path();
	if (p.empty())return;
	std::fstream t;
	t.open(p, std::ios::in);

	std::string str;
	t.seekg(0, std::ios::end);
	int x = t.tellg();
	str.reserve(x);
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	if (str.empty())return;

	QJsonParseError e;

	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e).object();
	if (e.error != QJsonParseError::NoError)
	{
		Logger::Warn(e.errorString());
		return;
	}
	scene.LoadFrom(json);
}

void ClassDiagram::Save()
{
	std::fstream f;
	f.open(Path(), std::ios::out);
	if (!f.is_open())
	{
		Logger::Warn(qsl("Failed to open file ") + QString::fromStdU16String(Path().u16string()));
		return;
	}

	QJsonObject o;
	scene.Save(o);
	QJsonDocument doc{ o };
	f << doc.toJson().constData();
}
void ClassDiagram::SaveAs()
{
	std::filesystem::path path{ QFileDialog::getSaveFileName(
		nullptr,
		qsl("Save Diagram As"),
		"",
		qsl("JSON (*.json);;All files (*.*)")
	).toStdU16String() };

	if (path.empty()) return;
	if (!path.has_extension())
		path.replace_extension("json");

	std::fstream f;
	f.open(path, std::ios::out);
	if (!f.is_open())
	{
		Logger::Warn(qsl("Failed to open file ") + QString::fromStdU16String(Path().u16string()));
		return;
	}

	QJsonObject o;
	scene.Save(o);
	QJsonDocument doc{ o };
	f << doc.toJson().constData();
	if (path.empty()) { SetPath(std::move(path)); EmptySaved(); }
}

