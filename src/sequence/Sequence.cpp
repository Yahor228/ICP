#include <sequence/Sequence.h>
#include <QVBoxLayout>
#include <ui/Log.h>
#include <util/util.h>
#include <fstream>
#include <QJsonDocument>
#include <QFileDialog>

SequenceDiagram::SequenceDiagram(std::filesystem::path self)
	:Tab(self), view(&scene)
{
	auto* vl = new QVBoxLayout;
	vl->setContentsMargins(0, 0, 0, 0);
	vl->addWidget(&view);
	setLayout(vl);
	scene.setSceneRect(-32000, -32000, 64000, 64000);

	connect(&scene, &SeqScene::SelectionChanged, this, &SequenceDiagram::SelectionChanged);;
}

void SequenceDiagram::SetClassPath(std::filesystem::path xclass_diag)
{
	class_diag = std::move(xclass_diag);
}

void SequenceDiagram::Request(request rq)
{
	switch (rq)
	{
	case Tab::None:
		break;
	case Tab::Save:
		if (!Path().empty())return Save();
		[[fallthrough]];
	case Tab::SaveAs:
		return SaveAs();
	case Tab::Close:
		break;
	case Tab::Rev:
		break;
	case Tab::Delete:
		scene.RemoveSelected();
		break;
	default:
		break;
	}
}

void SequenceDiagram::SetClass(std::shared_ptr<Scene> xscene)
{
	scene.SetClass(std::move(xscene));
}

std::shared_ptr<Scene> SequenceDiagram::GetClass() const
{
	return scene.GetClass();
}

const std::filesystem::path& SequenceDiagram::ClassDiagPath() const
{
	return class_diag;
}

void SequenceDiagram::OnEnter()
{
}


void SequenceDiagram::Save()
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

void SequenceDiagram::SaveAs()
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
	o.insert("Class Diagram", QString::fromStdU16String(class_diag.u16string()));
	scene.Save(o);
	QJsonDocument doc{ o };
	f << doc.toJson().constData();
	if (path.empty()) { SetPath(std::move(path)); EmptySaved(); }
}

void SequenceDiagram::Load(QJsonObject o)
{
	scene.LoadFrom(o);
}
