#include "ColorsModel.h"

namespace TermColor {

using namespace TermColor::Utils;

class ColorsModel::ColorsModelImpl {
public:
    explicit ColorsModelImpl(ColorsModel &p);

    // ColorsModel::Colors getters/setters
    const ColorsModel::Colors &getColors() const { return colors_; }
    void setImgColors(const std::vector<color> &colors);
    void setBGFGColors(const std::vector<color> &bgfg);

    // imagePath_ getters/setters
    void setImagePath(const QString &path);
    const std::string getImagePath() const;

private:
    ColorsModel &parent_;
    ColorsModel::Colors colors_;
    std::string imagePath_;
};

ColorsModel::ColorsModelImpl::ColorsModelImpl(ColorsModel &p) : parent_{p}, colors_{} {}

void ColorsModel::ColorsModelImpl::setImagePath(const QString &path) {
    const auto pathStr = path.toStdString(); // "file:///home/username/Pictures/xxxxxxx.png"
    imagePath_ = pathStr.substr(7);          // removes "file://"
}

const std::string ColorsModel::ColorsModelImpl::getImagePath() const {
    return imagePath_;
}

void ColorsModel::ColorsModelImpl::setImgColors(const std::vector<color> &colors) {
    const auto size = colors.size();

    // Clear existing colors
    colors_.regular_.clear();
    colors_.intense_.clear();

    // "Cut" colors in half and assign each halve to both Colors::regular_ and Colors::intense_
    colors_.regular_.insert(colors_.regular_.begin(), colors.begin(), colors.begin() + size/2);
    colors_.intense_.insert(colors_.intense_.begin(), colors.begin() + size/2, colors.end());

    // Changed state
    colors_.changedState_ = ChangedState::NewColors;
    emit parent_.modelChanged();
}

// Does same thing as setImgColors, but with BG/FG colors
void ColorsModel::ColorsModelImpl::setBGFGColors(const std::vector<color> &bgfg) {
    const auto size = bgfg.size();

    // Clear existing colors
    colors_.BGFG_.clear();
    colors_.BGFGintense_.clear();

    // "Cut" colors in half and assign each halve to both Colors::regular_ and Colors::intense_
    colors_.BGFG_.insert(colors_.BGFG_.begin(), bgfg.begin(), bgfg.begin() + size/2);
    colors_.BGFGintense_.insert(colors_.BGFGintense_.begin(), bgfg.begin() + size/2, bgfg.end());

    // Changed state
    colors_.changedState_ = ChangedState::Background;
    emit parent_.modelChanged();
}

// ColorsModel

ColorsModel::ColorsModel(QObject *parent) : QObject{parent} {
    pimpl_ = std::make_unique<ColorsModelImpl>(*this);
}

ColorsModel::~ColorsModel() = default;

ColorsModel::Colors::Colors() : changedState_{ChangedState::None}, BGFG_{}, BGFGintense_{}, regular_{}, intense_{} {}

const ColorsModel::Colors &ColorsModel::getColors() const {
    return pimpl_->getColors();
}

void ColorsModel::onImageDropped(const QString &path) {
    pimpl_->setImagePath(path);

    emit doProcessColors(pimpl_->getImagePath());
    emit hideImageDropWidget();
}

void ColorsModel::setImgColors(const std::vector<color> &colors) {
    pimpl_->setImgColors(colors);
}

void ColorsModel::setBGFGColors(const std::vector<color> &bgfg) {
    pimpl_->setBGFGColors(bgfg);
}

}
