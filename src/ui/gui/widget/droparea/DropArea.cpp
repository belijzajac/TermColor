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

    if (mimeData->hasUrls()) {
        // There is only ONE available text/url that we can read,
        // therefore there's no need to loop through mimeData->urls()
        auto picLocation = mimeData->urls().at(0).toString();
        const auto picLocationStr = picLocation.toStdString();

        // Remove new line symbol ("\r\n") in case it gets appended to the dropped mimeData
        const std::string newLn {"\r\n"};
        const auto loc = picLocationStr.find(newLn);

        if (loc != std::string::npos) {
            const auto goodStr = picLocationStr.substr(0, loc);

            // Replace picLocation with correct input
            picLocation.clear();
            picLocation = QString::fromStdString(goodStr);
        }
        emit imageDropped(picLocation);
    }

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
