#include "ImageDropWidget.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QLabel>
#include <ui/gui/droparea/DropArea.h>
#include <QDebug>

class ImageDropWidget::ImageDropWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ImageDropWidgetImpl(ImageDropWidget *parent);
    QVBoxLayout *getLayout(){ return layout_; }

private:
    void doLayout();

    QVBoxLayout *layout_;
    DropArea *dropArea_;
    ImageDropWidget *parent_;
};

ImageDropWidget::ImageDropWidgetImpl::ImageDropWidgetImpl(ImageDropWidget *parent) : QWidget{parent}, parent_{parent} {
    dropArea_ = new DropArea{this, {500, 500}};
    connect(dropArea_, SIGNAL(imageDropped(QString)), parent_, SLOT(onImageDrop(QString)));
    doLayout();
}

void ImageDropWidget::ImageDropWidgetImpl::doLayout() {
    layout_ = new QVBoxLayout{this};
    layout_->setMargin(0);
    layout_->addWidget(dropArea_);

    setMinimumSize(500, 500);
}

void ImageDropWidget::onImageDrop(const QString& path) {
    qDebug() << "ImageDropWidget::onImageDrop():" << path;
}

// ImageDropWidget

ImageDropWidget::ImageDropWidget(QWidget *parent) : QWidget{parent} {
    pimpl_ = std::make_unique<ImageDropWidgetImpl>(this);
    this->setLayout(pimpl_->getLayout());
}

ImageDropWidget::~ImageDropWidget() = default;

#include "ImageDropWidget.moc"
