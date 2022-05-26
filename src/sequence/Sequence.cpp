#include <sequence/Sequence.h>
#include <QVBoxLayout>

SequenceDiagram::SequenceDiagram(std::filesystem::path self)
	:Tab(self), view(&scene)
{
	auto* vl = new QVBoxLayout;
	vl->setContentsMargins(0, 0, 0, 0);
	vl->addWidget(&view);
	setLayout(vl);
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
		if (!Path().empty());
		[[fallthrough]];
	case Tab::SaveAs:
		break;
	case Tab::Close:
		break;
	case Tab::Rev:
		break;
	case Tab::Delete:
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
