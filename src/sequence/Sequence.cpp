#include <sequence/Sequence.h>
#include <sequence/element.h>
#include <sequence/diagram.h>
#include <commands/commandstack.h>
#include <commands/add.h>
#include <util/util.h>
#include <event.h>

SequenceTab::SequenceTab(const std::unordered_map<std::u16string_view, Node*>& info)
	:info(info), view(&scene)
{
	lay.setContentsMargins(0, 0, 0, 0);
	lay.addWidget(&view);
	setLayout(&lay);
	view.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	scene.addItem(&diag);
	
	view.SetWidget(this);
	FillMenu();
}

void SequenceTab::FillMenu()
{
	for (auto& i : info)
	{
		auto r = i.first;
		menu.addAction(QString::fromUtf16(i.first.data(), int(i.first.size())), [this, r]() {
			CommandStack::current().push(new AddCommand(diag, QString::fromUtf16(r.data(), int(r.size()))));
			});
	}
}

bool SequenceTab::event(QEvent* e)
{
	if (e->type() == QEvent::ContextMenu)
	{
		auto& x = static_cast<Event&>(*e);
		menu.popup(x.globalPos());
		return true;
	}
	return QWidget::event(e);
}
