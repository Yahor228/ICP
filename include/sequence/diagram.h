#pragma once
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <memory>

class Element;

class Diagram : public QGraphicsWidget
{
public:
	Diagram();
public:
	size_t count()const;
	void append(std::unique_ptr<Element> element);
	void insert(std::unique_ptr<Element> element, size_t index);
	std::unique_ptr<Element> remove(size_t index);
private:
	QGraphicsLinearLayout* hl;
};