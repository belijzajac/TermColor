#include "DisplayWidget.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>

class DisplayWidget::DisplayWidgetImpl : public QWidget {
public:
    DisplayWidgetImpl(DisplayWidget *parent, QVBoxLayout* layout);

private:
    QTextEdit *textDspl_;
};

DisplayWidget::DisplayWidgetImpl::DisplayWidgetImpl(DisplayWidget *parent, QVBoxLayout* layout) : QWidget{parent} {
    auto label = new QLabel{this};
    label->setText("Terminal preview");

    textDspl_ = new QTextEdit{this};
    textDspl_->setFixedSize(600, 300);
    textDspl_->setText("#ifndef EXCEPTION_H \n"
                       "#define EXCEPTION_H \n"
                       "\n"
                       "#include <bits/exception.h> \n"
                       "#include <string> \n"
                       "\n"
                       "class Exception : public std::exception { \n"
                       "public: \n"
                       "   explicit Exception(const std::string &msg) : msg_{msg} {} \n"
                       "   const char *what() const noexcept override { return msg_.c_str(); } \n"
                       "\n"
                       "private: \n"
                       "   std::string msg_; \n"
                       "}; \n"
                       "\n"
                       "#endif // EXCEPTION_H");
    textDspl_->setReadOnly(true);

    layout->addWidget(label);
    layout->addWidget(textDspl_);
}

// DisplayWidget

DisplayWidget::DisplayWidget(QWidget *parent) : QWidget{parent} {
    auto layout = new QVBoxLayout{this};
    pimpl_ = new DisplayWidgetImpl{this, layout};
    layout->addWidget(pimpl_);
}
