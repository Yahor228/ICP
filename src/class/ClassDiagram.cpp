#include <class/ClassDiagram.h>
#include <fstream>
#include <QJsonDocument>
#include <QVBoxLayout>

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
		break;
	case Tab::SaveAs:
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
}

