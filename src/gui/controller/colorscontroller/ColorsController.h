#ifndef COLORSCONTROLLER_H
#define COLORSCONTROLLER_H

#include "gui/model/colorsmodel/ColorsModel.h"
#include <QObject>

namespace TermColor {

class ColorsController : public QObject {
    Q_OBJECT
public:
    ColorsController(ColorsModel *c, QObject *parent = nullptr);
    ~ColorsController() = default;

    void doImageColors(std::string_view imgPath);

private:
    ColorsModel *colorsModel_;
    std::unique_ptr<DominantColor> domColor;

private slots:
    void onRadioBtnClicked(int id);
};

}

#endif // COLORSCONTROLLER_H
