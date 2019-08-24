#include "DropArea.h"
#include "qevent.h"
#include <QMimeData>

DropArea::DropArea(QWidget *parent, const QSize &size) {
    setMinimumSize(size);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setAutoFillBackground(true);

    resetLabel();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event) {
    setBackgroundRole(QPalette::Highlight);
    event->acceptProposedAction();
}

// To unpack dropped data and handle it
void DropArea::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasText())
        emit imageDropped(mimeData->text());

    setBackgroundRole(QPalette::Midlight);
    event->acceptProposedAction();
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event) {
    resetLabel();
    event->accept();
}

void DropArea::resetLabel() {
    setLabelText("<< Drop Image >>");
    setBackgroundRole(QPalette::Midlight);
}

void DropArea::setLabelText(const QString &text) {
    setText(text);
    repaint();
}
