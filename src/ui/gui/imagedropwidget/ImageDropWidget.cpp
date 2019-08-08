#include "ImageDropWidget.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QLabel>

class ImageDropWidget::ImageDropWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ImageDropWidgetImpl(ImageDropWidget *parent);
    QVBoxLayout *getLayout(){ return layout_; }

private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void doLayout();

    QVBoxLayout *layout_;
    ImageDropWidget *parent_;
};

ImageDropWidget::ImageDropWidgetImpl::ImageDropWidgetImpl(ImageDropWidget *parent) : QWidget{parent}, parent_{parent} {
    setAcceptDrops(true); // to enable drop events
    doLayout();
}

void ImageDropWidget::ImageDropWidgetImpl::doLayout() {
    auto label = new QLabel{this};
    label->setText("<< Drop Image >>");

    layout_ = new QVBoxLayout{this};
    layout_->addWidget(label);
}

// To inform about the types of data that the widget accepts
void ImageDropWidget::ImageDropWidgetImpl::dragEnterEvent(QDragEnterEvent *event) {
    if(const bool hasImage = event->mimeData()->hasImage()){
        event->setAccepted(hasImage);
        emit parent_->imageDropped(event->mimeData()->text());
    }else{
        event->ignore();
    }
}

// To unpack dropped data and handle it
void ImageDropWidget::ImageDropWidgetImpl::dropEvent(QDropEvent *event) {
    //...
}

// ImageDropWidget

ImageDropWidget::ImageDropWidget(QWidget *parent) : QWidget{parent} {
    pimpl_ = std::make_unique<ImageDropWidgetImpl>(this);
    this->setLayout(pimpl_->getLayout());
}

ImageDropWidget::~ImageDropWidget() = default;

#include "ImageDropWidget.moc"
