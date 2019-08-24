#include "ExportWidget.h"
#include <ui/gui/model/terminalsmodel/TerminalsModel.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>

class ExportWidget::ExportWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ExportWidgetImpl(const TerminalsModel &t, ExportWidget *parent);
    QHBoxLayout *getLayout() { return hLayout_; }
    void onModelChanged();

    // Returns current item that is selected in comboBox_
    const QString getComboboxItem() const;

private:
    void doLayout();
    void doConnections();

private:
    const TerminalsModel &terminalsModel_;
    ExportWidget *parent_;

    // Widgets to be put on a layout
    QComboBox *comboBox_;
    QPushButton *saveButton_;

    // Layout
    QHBoxLayout *hLayout_;
};

ExportWidget::ExportWidgetImpl::ExportWidgetImpl(const TerminalsModel &t, ExportWidget *parent)
 : QWidget{parent}, parent_{parent}, terminalsModel_{t}, hLayout_{new QHBoxLayout{this}} {
    // Layouts everything together and connectes button via signal/slot
    doLayout();
    doConnections();
}

void ExportWidget::ExportWidgetImpl::doLayout() {
    // Construct a combo-box
    comboBox_ = new QComboBox{this};
    comboBox_->setFixedSize(150, 30);

    // Construct a button
    saveButton_ = new QPushButton{"Save!", this};
    saveButton_->setFixedSize(100, 30);

    // A spacer
    auto rightSpacer = new QSpacerItem{5, 0, QSizePolicy::Fixed, QSizePolicy::Fixed};

    // Adding everything together
    hLayout_->addWidget(comboBox_, 0, Qt::AlignRight);
    hLayout_->addWidget(saveButton_, 0, Qt::AlignRight);
    hLayout_->addSpacerItem(rightSpacer);
}

void ExportWidget::ExportWidgetImpl::doConnections() {
    connect(saveButton_, &QPushButton::clicked, [=]() {
        parent_->onSaveBtnClicked();
    });
}

void ExportWidget::ExportWidgetImpl::onModelChanged() {
    const auto &installedTerm = terminalsModel_.getTerminals().installed_;

    for(const auto &term : installedTerm)
        comboBox_->addItem(QString::fromStdString(term));
}

// By default, for an empty combo box or a combo box
// in which no current item is set, this property contains an invalid QVariant
const QString ExportWidget::ExportWidgetImpl::getComboboxItem() const {
    const auto comboItem = comboBox_->currentData(Qt::DisplayRole);
    return comboItem.toString();
}

// ExportWidget

ExportWidget::ExportWidget(const TerminalsModel &t, QWidget *parent) {
    pimpl_ = std::make_unique<ExportWidgetImpl>(t, this);
    this->setLayout(pimpl_->getLayout());
}

void ExportWidget::onModelChanged() {
    pimpl_->onModelChanged();
}

void ExportWidget::onSaveBtnClicked() {
    // Obtain currently selected element in combo-box
    const auto comboItem = pimpl_->getComboboxItem();

    // Send string via signal to MainWindow and do logic there
    emit saveBtnClicked(comboItem.toStdString());
}

ExportWidget::~ExportWidget() = default;

#include "ExportWidget.moc"