#pragma once
#include <QGraphicsWidget>
#include <ui/EditableText.h>
#include <QPointer>

class Node;
class Call;

class Element : public QGraphicsWidget
{
	using base = QGraphicsWidget;
public:
	Element(QPointer<Node> node);
	~Element();
public:
	QRectF boundingRect() const override;
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override;
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;
	Node* GetNode()const;

	void AddFrom(Call* c);
	void AddTo(Call* c);
	
	void RemoveFrom(Call* c);
	void RemoveTo(Call* c);

	void Relinquish();
	void Reconnect();

	void UpdateConnections();
private:
	QLineF line;
	QPointer<Node> node;
	std::vector<Call*> from;	//0------>
	std::vector<Call*> to;		// ------>0
	bool owns_c = false;
};