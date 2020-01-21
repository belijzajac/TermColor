#ifndef IMAGEDROPWIDGET_H
#define IMAGEDROPWIDGET_H

#include <QWidget>
#include <memory>

namespace TermColor {

class ImageDropWidget : public QWidget {
    class ImageDropWidgetImpl;
    Q_OBJECT
public:
    explicit ImageDropWidget(QWidget *parent = nullptr);
    ~ImageDropWidget();

signals:
    void imageDropped(const QString& path);
    void onHideWidget();

public slots:
    void hideWidget();

private:
    std::unique_ptr<ImageDropWidgetImpl> pimpl_;
};

}

#endif // IMAGEDROPWIDGET_H
