#ifndef BGFGCHOOSER_H
#define BGFGCHOOSER_H

#include <QWidget>

class QRadioButton;
class QButtonGroup;

namespace TermColor {

class BGFGChooser : public QWidget {
    Q_OBJECT
public:
    explicit BGFGChooser(QWidget *parent = nullptr);

signals:
    void radioBtnClicked(int id);

private:
    void doConnections();

    QButtonGroup *btnGroup_;
    QRadioButton *darkBtn_;
    QRadioButton *lightBtn_;
};

}

#endif // BGFGCHOOSER_H
