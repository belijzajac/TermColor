#ifndef IMAGEDROPWIDGET_H
#define IMAGEDROPWIDGET_H

#include <QWidget>
#include <memory>

class ImageDropWidget : public QWidget {
    class ImageDropWidgetImpl;
    Q_OBJECT
public:
    explicit ImageDropWidget(QWidget *parent = nullptr);
    ~ImageDropWidget();

private slots:
    void onImageDrop(const QString& path);

private:
    std::unique_ptr<ImageDropWidgetImpl> pimpl_;
};

#endif // IMAGEDROPWIDGET_H
