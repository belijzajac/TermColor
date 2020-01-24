#include "ImageDropWidget.h"

#include <ui/gui/widget/droparea/DropArea.h>

#include <QDragEnterEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QLabel>

namespace TermColor {

class ImageDropWidget::ImageDropWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ImageDropWidgetImpl(ImageDropWidget *parent);
    QVBoxLayout *getLayout(){ return layout_; }

private slots:
    void onImageDrop(const QString& path);

private:
    void doLayout();

    QVBoxLayout *layout_;
    DropArea *dropArea_;
    ImageDropWidget *parent_;
};

ImageDropWidget::ImageDropWidgetImpl::ImageDropWidgetImpl(ImageDropWidget *parent)
    : QWidget{parent}
    , parent_{parent}
{
    dropArea_ = new DropArea{this, {845, 500}};
    connect(dropArea_, SIGNAL(imageDropped(QString)), this, SLOT(onImageDrop(QString)));
    doLayout();
}

void ImageDropWidget::ImageDropWidgetImpl::doLayout() {
    layout_ = new QVBoxLayout{this};
    layout_->setMargin(0);
    layout_->addWidget(dropArea_);
}

void ImageDropWidget::ImageDropWidgetImpl::onImageDrop(const QString& path) {
    dropArea_->setLabelText("Working on image...");
    emit parent_->imageDropped(path);
}

// ImageDropWidget

ImageDropWidget::ImageDropWidget(QWidget *parent) : QWidget{parent} {
    pimpl_ = std::make_unique<ImageDropWidgetImpl>(this);
    this->setLayout(pimpl_->getLayout());
}

void ImageDropWidget::hideWidget() {
    this->hide();
    emit onHideWidget();
}

ImageDropWidget::~ImageDropWidget() = default;

}

#include "ImageDropWidget.moc"
