#ifndef TERMINALSCONTROLLER_H
#define TERMINALSCONTROLLER_H

#include "gui/model/terminalsmodel/TerminalsModel.h"
#include "gui/model/colorsmodel/ColorsModel.h"
#include <QObject>

namespace TermColor {

class TerminalsController : public QObject {
    Q_OBJECT
public:
    TerminalsController(TerminalsModel *t, const ColorsModel &c, QObject *parent = nullptr);
    ~TerminalsController() = default;

    void doTerminals();

signals:
    void noTerminalsFound();

private:
    TerminalsModel *terminalsModel_;
    const ColorsModel &colorsModel_;

private slots:
    void onProcessSave(std::string_view saveOption);
    void onSaveToJson(std::string_view saveLocation);
};

}

#endif // TERMINALSCONTROLLER_H
