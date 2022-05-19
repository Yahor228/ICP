#pragma once
#include <vector>
#include <QGraphicsWidget>

namespace UI
{
	class Node : public QGraphicsWidget
	{
	public:
		Node();
	public:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	private:
		QSize bounder() const;
	private:
		std::vector<QString> data;
		std::vector<QString> methods;
		std::vector<Node*> inherited;
	private:
		static QFontMetrics space;
	};
}