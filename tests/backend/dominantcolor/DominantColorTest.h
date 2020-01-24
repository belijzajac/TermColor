#ifndef DOMINANTCOLORTEST_H
#define DOMINANTCOLORTEST_H

#include <QObject>
#include <memory>

#include "DominantColor.h"

class DominantColorTest : public QObject {
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testDominantColors();
    void testBGFGColors();
    void testIntenseColors();

private:
    void createSampleImg();

private:
    std::unique_ptr<TermColor::DominantColor> dominantColor_;
};

#endif
