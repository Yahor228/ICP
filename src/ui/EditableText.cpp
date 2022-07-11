#include <ui/EditableText.h>
#include <QGraphicsLayout>
#include <QPainter>
#include <QFontMetricsF>
#include <QApplication>

EditableText::EditableText(const QString& text, QGraphicsLayout* layout) :
    mLayout(layout), text(text)
{
    setGraphicsItem(this);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setOwnedByLayout(true);
}

void EditableText::setGeometry(const QRectF& geom)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geom);
    setPos(geom.topLeft());
}

void EditableText::SetText(QString xtext)
{
    text = std::move(xtext);
    updateGeometry();
    mLayout->updateGeometry();
    update();
}

QSizeF EditableText::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    return boundingRect().size();
}

QRectF EditableText::boundingRect() const
{
    auto r = QFontMetricsF{ font }.boundingRect(text);
    if(r.y() < 0) r.moveTop(0);
    return r;
}

void EditableText::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*)
{
    p->setFont(font);
    p->setPen(color);
    p->drawText(boundingRect().bottomLeft(), text);
}
