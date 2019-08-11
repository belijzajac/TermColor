#include "GuiModel.h"

class GuiModel::GuiModelImpl {
public:
    explicit GuiModelImpl(GuiModel &p);
    const GuiModel::Colors &getColors() const { return colors_; }

    void setImagePath(const QString &path);

private:
    GuiModel &parent_;
    GuiModel::Colors colors_;
    std::string imagePath_;
};

GuiModel::GuiModelImpl::GuiModelImpl(GuiModel &p) : parent_{p}, colors_{} {

}

void GuiModel::GuiModelImpl::setImagePath(const QString &path) {
    imagePath_ = path.toStdString();
}

// GuiModel

GuiModel::GuiModel(QObject *parent) : QObject{parent} {
    pimpl_ = std::make_unique<GuiModelImpl>(*this);
}

GuiModel::~GuiModel() = default;

GuiModel::Colors::Colors() : BGFG_{{121, 129, 139}, {197, 200, 198}}, regular_{}, intense_{} {

}

const GuiModel::Colors &GuiModel::getColors() const {
    return pimpl_->getColors();
}

void GuiModel::onImageDropped(const QString &path) {
    pimpl_->setImagePath(path);

    // TODO:
    // 1) proceed to obtain colors
    // 2) hide widget

    emit hideImageDropWidget();
}
