#include "ExportWidget.h"
#include <ui/gui/guimodel/GuiModel.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>

class ExportWidget::ExportWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ExportWidgetImpl(const GuiModel &g, ExportWidget *parent);
    QHBoxLayout *getLayout() { return hLayout_; }
    void onModelChanged();

private:
    const GuiModel &guiModel_;
    ExportWidget *parent_;

    // Widgets to be put on a layout
    QComboBox *comboBox_;
    QPushButton *saveButton_;

    // Layout
    QHBoxLayout *hLayout_;
};

ExportWidget::ExportWidgetImpl::ExportWidgetImpl(const GuiModel &g, ExportWidget *parent)
 : QWidget{parent}, parent_{parent}, guiModel_{g}, hLayout_{new QHBoxLayout{this}} {
    // Construct a combo-box
    comboBox_ = new QComboBox{this};
    comboBox_->setFixedSize(200, 30);

    // Construct a button
    saveButton_ = new QPushButton{"Save!", this};
    saveButton_->setFixedSize(100, 30);

    hLayout_->addWidget(comboBox_, 0, Qt::AlignJustify);
    hLayout_->addWidget(saveButton_, 0, Qt::AlignJustify);
}

void ExportWidget::ExportWidgetImpl::onModelChanged() {
    const auto &installedTerm = guiModel_.getTerminals().installed_;

    for(const auto &term : installedTerm)
        comboBox_->addItem(QString::fromStdString(term));
}

// ExportWidget

ExportWidget::ExportWidget(const GuiModel &g, QWidget *parent) {
    pimpl_ = std::make_unique<ExportWidgetImpl>(g, this);
    this->setLayout(pimpl_->getLayout());
}

void ExportWidget::onModelChanged() {
    pimpl_->onModelChanged();
}

ExportWidget::~ExportWidget() = default;

#include "ExportWidget.moc"