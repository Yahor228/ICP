/**
 * @file EditableText.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration of class for editation a class in CD
 */


#pragma once
#include <QGraphicsTextItem>
#include <QGraphicsLayoutItem>
#include <QPen>
#include <QFont>

class QGraphicsLayout;

class EditableText : public QGraphicsItem, public QGraphicsLayoutItem
{
public:
	EditableText(const QString& text, QGraphicsLayout* layout);
public:
	void SetText(QString xtext);
	const QString& Text()const { return text; }
	auto* Layout() { return mLayout; }
	void SetFont(const QFont& xfont) { font = xfont; };
	void SetColor(const QColor& xcol) { color = xcol; };
public:
	void setGeometry(const QRectF& geom);
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint) const;
private:
	QRectF boundingRect() const override;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)override;
private:
	QString text;
	QGraphicsLayout* mLayout;
	QFont font;
	QPen color = QColor{ Qt::white };
};