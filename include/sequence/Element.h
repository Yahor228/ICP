#pragma once
#include <QGraphicsWidget>
#include <ui/EditableText.h>
#include <QPointer>
#include <ISave.h>
#include <ISelectable.h>

class Node;
class Call;

class Element : public QGraphicsWidget, public ISelectable, public ISave
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
	const QString& Name()const noexcept;
	void ChangeName(const QString& c)noexcept;
	QString UID()const;

	void AddFrom(Call* c);
	void AddTo(Call* c);
	
	void RemoveFrom(Call* c);
	void RemoveTo(Call* c);

	void Relinquish();
	void Reconnect();

	void UpdateConnections();
	virtual void Save(QJsonObject& o)const override;
	virtual ty XType()const noexcept override
	{
		return ty::element;
	}
	bool Valid()const noexcept;
private:
	QLineF line;
	EditableText* name;
	QPointer<Node> node;
	std::vector<Call*> from;	//0------>
	std::vector<Call*> to;		// ------>0
	bool owns_c = false;
	Call* has_d = nullptr;
};