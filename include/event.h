#pragma once
#include <QContextMenuEvent>

struct Event : public QContextMenuEvent
{
	QPointF scene_pos;
	Event(const QContextMenuEvent& e, QPointF scene_pos)
		:QContextMenuEvent(e), scene_pos(scene_pos) {}
};