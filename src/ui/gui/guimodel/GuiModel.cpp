#include "GuiModel.h"
#include <backend/dominantcolor/DominantColor.h>

class GuiModel::GuiModelImpl {
public:
    explicit GuiModelImpl(GuiModel &p);

    // GuiModel::Colors getters/setters
    const GuiModel::Colors &getColors() const { return colors_; }
    void setImgColors(const std::vector<color> &colors);
    void setBGFGColors(const std::vector<color> &bgfg);

    // GuiModel::Terminals getters/setters
    const GuiModel::Terminals &getTerminals() const { return terminals_; };
    void insertTerminal(const std::string &term);

    // imagePath_ getters/setters
    void setImagePath(const QString &path);
    const std::string getImagePath() const;

private:
    GuiModel &parent_;
    GuiModel::Colors colors_;
    GuiModel::Terminals terminals_;
    std::string imagePath_;
};

GuiModel::GuiModelImpl::GuiModelImpl(GuiModel &p) : parent_{p}, colors_{}, terminals_{} {

}

void GuiModel::GuiModelImpl::setImagePath(const QString &path) {
    const auto pathStr = path.toStdString(); // "file:///home/username/Pictures/xxxxxxx.png"
    imagePath_ = pathStr.substr(7);          // removes "file://"
}

const std::string GuiModel::GuiModelImpl::getImagePath() const {
    return imagePath_;
}

void GuiModel::GuiModelImpl::setImgColors(const std::vector<color> &colors) {
    const auto size = colors.size();

    // Clear existing colors
    colors_.regular_.clear();
    colors_.intense_.clear();

    // "Cut" colors in half and assign each halve to both Colors::regular_ and Colors::intense_
    colors_.regular_.insert(colors_.regular_.begin(), colors.begin(), colors.begin() + size/2);
    colors_.intense_.insert(colors_.intense_.begin(), colors.begin() + size/2, colors.end());
}

// Does same thing as setImgColors, but with BG/FG colors
void GuiModel::GuiModelImpl::setBGFGColors(const std::vector<color> &bgfg) {
    const auto size = bgfg.size();

    // Clear existing colors
    colors_.BGFG_.clear();
    colors_.BGFGintense_.clear();

    // "Cut" colors in half and assign each halve to both Colors::regular_ and Colors::intense_
    colors_.BGFG_.insert(colors_.BGFG_.begin(), bgfg.begin(), bgfg.begin() + size/2);
    colors_.BGFGintense_.insert(colors_.BGFGintense_.begin(), bgfg.begin() + size/2, bgfg.end());
}

void GuiModel::GuiModelImpl::insertTerminal(const std::string &term) {
    auto &terminals = terminals_.installed_;
    if (terminals.empty() || std::find(terminals.begin(), terminals.end(), term) == terminals.end())
        terminals.push_back(term);
}

// GuiModel

GuiModel::GuiModel(QObject *parent) : QObject{parent} {
    pimpl_ = std::make_unique<GuiModelImpl>(*this);
}

GuiModel::~GuiModel() = default;

GuiModel::Colors::Colors() : BGFG_{}, BGFGintense_{}, regular_{}, intense_{} {}

GuiModel::Terminals::Terminals() {
    // Extract supported terminal emulators from terminalToEnum_
    for (const auto &[key, val] : terminalToEnum_)
        supported_.push_back(key);
}

const GuiModel::Colors &GuiModel::getColors() const {
    return pimpl_->getColors();
}

void GuiModel::onImageDropped(const QString &path) {
    pimpl_->setImagePath(path);

    emit doProcessColors(pimpl_->getImagePath());
    emit hideImageDropWidget();
}

void GuiModel::setImgColors(const std::vector<color> &colors) {
    pimpl_->setImgColors(colors);
}

void GuiModel::setBGFGColors(const std::vector<color> &bgfg) {
    pimpl_->setBGFGColors(bgfg);
}

const GuiModel::Terminals &GuiModel::getTerminals() const {
    return pimpl_->getTerminals();
}

void GuiModel::insertTerminal(const std::string &term) {
    pimpl_->insertTerminal(term);
}
