#include "ExportWidget.h"
#include <ui/gui/model/terminalsmodel/TerminalsModel.h>
#include <backend/writer/Writer.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>

class ExportWidget::ExportWidgetImpl : public QWidget {
    Q_OBJECT
public:
    explicit ExportWidgetImpl(const TerminalsModel &t, ExportWidget *parent);
    QHBoxLayout *getLayout() { return hLayout_; }
    void onModelChanged();
    void disableSaveBtn();

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
    QPushButton *jsonButton_;

    // Layout
    QHBoxLayout *hLayout_;
};

ExportWidget::ExportWidgetImpl::ExportWidgetImpl(const TerminalsModel &t, ExportWidget *parent)
 : QWidget{parent}, parent_{parent}, terminalsModel_{t}, hLayout_{new QHBoxLayout{this}} {
    // Layouts everything together and connects buttons via signal/slot
    doLayout();
    doConnections();
}

void ExportWidget::ExportWidgetImpl::doLayout() {
    // Construct a combo-box
    comboBox_ = new QComboBox{this};
    comboBox_->setFixedSize(150, 30);

    // Construct a save button
    saveButton_ = new QPushButton{"Save!", this};
    saveButton_->setFixedSize(100, 30);

    // Construct a save-to-json button
    jsonButton_ = new QPushButton{"Export to JSON", this};
    jsonButton_->setFixedSize(110, 30);

    // A spacer
    auto rightSpacer = new QSpacerItem{5, 0, QSizePolicy::Fixed, QSizePolicy::Fixed};

    // Adding everything together
    hLayout_->addWidget(comboBox_, 0, Qt::AlignRight);
    hLayout_->addWidget(saveButton_, 0, Qt::AlignRight);
    hLayout_->addWidget(jsonButton_, 0, Qt::AlignRight);
    hLayout_->addSpacerItem(rightSpacer);
}

void ExportWidget::ExportWidgetImpl::doConnections() {
    // Save button
    connect(saveButton_, &QPushButton::clicked, [=]() {
        parent_->onSaveBtnClicked();
    });

    // Save-to-json button
    connect(jsonButton_, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this,
                tr("Export to JSON"), "/home/" + QString::fromStdString(Writer::getUsername()), tr("JSON file (*.json)"));

        if (!fileName.isEmpty())
            emit parent_->saveToJsonBtnClicked(fileName.toStdString());
    });
}

void ExportWidget::ExportWidgetImpl::onModelChanged() {
    const auto &terminals = terminalsModel_.getTerminals();
    const auto state = terminals.changedState_;

    // On first run append combo-box with new terminals
    if (state == TerminalsModel::ChangedState::None) {
        // Nothing to do...
    } else if (state == TerminalsModel::ChangedState::NewTerminals) {
        for (const auto &term : terminals.installed_)
            comboBox_->addItem(QString::fromStdString(term));
    }
}

void ExportWidget::ExportWidgetImpl::disableSaveBtn() {
    comboBox_->setDisabled(true);
    saveButton_->setDisabled(true);
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

void ExportWidget::disableSaveBtn() {
    pimpl_->disableSaveBtn();
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