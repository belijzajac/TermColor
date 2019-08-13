#include "GuiModel.h"
#include <backend/dominantcolor/DominantColor.h>
#include <QDebug>

class GuiModel::GuiModelImpl {
public:
    explicit GuiModelImpl(GuiModel &p);
    const GuiModel::Colors &getColors() const { return colors_; }
    void setColors(const std::vector<color> &colors);

    void setImagePath(const QString &path);
    const std::string getImagePath() const;

private:
    GuiModel &parent_;
    GuiModel::Colors colors_;
    std::string imagePath_;
};

GuiModel::GuiModelImpl::GuiModelImpl(GuiModel &p) : parent_{p}, colors_{} {

}

void GuiModel::GuiModelImpl::setImagePath(const QString &path) {
    const auto pathStr = path.toStdString(); // "file:///home/username/Pictures/xxxxxxx.png"
    imagePath_ = pathStr.substr(7);          // removes "file://"
}

const std::string GuiModel::GuiModelImpl::getImagePath() const {
    return imagePath_;
}

void GuiModel::GuiModelImpl::setColors(const std::vector<color> &colors) {
    const auto size = colors.size();

    // Clear existing colors
    colors_.regular_.clear();
    colors_.intense_.clear();

    // "Cut" colors in half and assign each halve to both Colors::regular_ and Colors::intense_
    colors_.regular_.insert(colors_.regular_.begin(), colors.begin(), colors.begin() + size/2);
    colors_.intense_.insert(colors_.intense_.begin(), colors.begin() + size/2, colors.end());
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

    emit doProcessColors(pimpl_->getImagePath());
    emit hideImageDropWidget();
}

void GuiModel::setColors(const std::vector<color> &colors) {
    pimpl_->setColors(colors);
}
