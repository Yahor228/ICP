#include <sequence/texter.h>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLayout>
#include <QTextCursor>

EditableText::EditableText(const QString& text, QGraphicsLayout* layout, QGraphicsItem* parent) :
    QGraphicsLayoutItem(),
    QGraphicsTextItem(parent),
    mLayout(layout)
{
    setGraphicsItem(this);

    setHtml(text);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void EditableText::SetText(const QString& s)
{
    setPlainText(s);
    updateGeometry();
    mLayout->updateGeometry();
}

void EditableText::setGeometry(const QRectF& geom)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geom);
    setPos(geom.topLeft());
}

QSizeF EditableText::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    return boundingRect().size();
}

void EditableText::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!editable)return;
    if (event->button() == Qt::LeftButton && textInteractionFlags() == Qt::NoTextInteraction) {
        setTextInteractionFlags(Qt::TextEditorInteraction);
    }

    QGraphicsTextItem::mousePressEvent(event);
}

void EditableText::focusOutEvent(QFocusEvent* event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    auto cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    QGraphicsTextItem::focusOutEvent(event);
}

void EditableText::keyPressEvent(QKeyEvent* event)
{
    QGraphicsTextItem::keyPressEvent(event);
    updateGeometry();
    mLayout->updateGeometry();
}