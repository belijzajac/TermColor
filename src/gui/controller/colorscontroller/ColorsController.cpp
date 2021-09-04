#include "ColorsController.h"

#include "backend/exception/Exception.h"
#include <memory>

namespace TermColor {

ColorsController::ColorsController(ColorsModel *c, QObject *parent)
    : colorsModel_{c}
    , domColor(std::make_unique<DominantColor>())
{
    // ...
}

void ColorsController::doImageColors(std::string_view imgPath) {
    using namespace TermColor::Utils;

    try {
        // Passes an image to opencv's cv::imread function
        // and later performs the k-means' algorithm
        domColor->readImage(imgPath);
        domColor->performKMeans();

        // Obtains extracted colors
        auto dominantColors = domColor->getColors();
        const auto &intenseColors = domColor->intenseColors(dominantColors);

        // Obtain predefined BG/FG colors
        auto bgfg = domColor->getBGFGColors();
        const auto &bgfgIntense = domColor->intenseColors(bgfg);

        // Copy colors to one continuous vector<color> DS
        dominantColors.insert(dominantColors.end(), intenseColors.begin(), intenseColors.end());
        bgfg.insert(bgfg.end(), bgfgIntense.begin(), bgfgIntense.end());

        // Populates model with new data (colors, in this case)
        colorsModel_->setImgColors(dominantColors);
        colorsModel_->setBGFGColors(bgfg);

    } catch (TermColorException &e) {
        throw;
    }
}

void ColorsController::onRadioBtnClicked(int id) {
    // Dark BG/FG (id = 0) or Light BG/FG (id = 1)
    // getBGFGColors(true) ==> obtain dark BG/FG, otherwise (if false) - light BG/FG
    auto bgfg = domColor->getBGFGColors(id == 0);
    const auto &bgfgIntense = domColor->intenseColors(bgfg);

    // Copy colors to one continuous vector<color> DS
    bgfg.insert(bgfg.end(), bgfgIntense.begin(), bgfgIntense.end());

    // Populates model with new data (colors, in this case)
    colorsModel_->setBGFGColors(bgfg);
}

}
