#include "DominantColorTest.h"

#include <QtTest/QtTest>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <filesystem>

namespace fs = std::filesystem;

void DominantColorTest::init() {
    dominantColor_ = std::make_unique<TermColor::DominantColor>();

    // create a sample image for testing
    createSampleImg();

    dominantColor_->readImage("image.jpg");
    dominantColor_->performKMeans();
}

void DominantColorTest::cleanup() {
    fs::path imgPath = fs::current_path();
    imgPath += "/image.jpg";
    fs::remove(imgPath);
}

void DominantColorTest::testDominantColors() {
    const auto domColors = dominantColor_->getColors();
    QVERIFY(!domColors.empty());

    std::vector<TermColor::Utils::color> expectedColors = {
        {0, 0, 253}, {1, 126, 1}, {253, 0, 0}, {0, 254, 254},
        {126, 0, 0}, {0, 0, 125}, {0, 254, 1}, {0, 127, 126}
    };

    QCOMPARE(domColors, expectedColors);
}

void DominantColorTest::testBGFGColors() {
    const auto bgfgColors = dominantColor_->getBGFGColors();
    QVERIFY(!bgfgColors.empty());

    std::vector<TermColor::Utils::color> expectedColors = {
        {40, 38, 45},
        {130, 159, 159}
    };

    QCOMPARE(bgfgColors, expectedColors);
}

void DominantColorTest::testIntenseColors() {
    const auto domColors = dominantColor_->getColors();
    QVERIFY(!domColors.empty());

    const auto intenseColors = dominantColor_->intenseColors(domColors);
    QVERIFY(!intenseColors.empty());

    std::vector<TermColor::Utils::color> expectedColors = {
        {102, 254, 102}, {177, 102, 102}, {102, 102, 253}, {102, 178, 177},
        {102, 254, 254}, {253, 102, 102}, {102, 102, 177}, {102, 177, 102}
    };

    QCOMPARE(intenseColors, expectedColors);
}

void DominantColorTest::createSampleImg() {
    cv::Mat image = cv::Mat::zeros(500, 1000, CV_8UC3);

    for (int col = 0; col < 2; ++col) {
        cv::rectangle(image, cv::Point(0,   250 * col), cv::Point(250,  250 * (col+1)), cv::Scalar(0,           0,           255/(col+1)), -1);
        cv::rectangle(image, cv::Point(250, 250 * col), cv::Point(500,  250 * (col+1)), cv::Scalar(0,           255/(col+1), 0),           -1);
        cv::rectangle(image, cv::Point(500, 250 * col), cv::Point(750,  250 * (col+1)), cv::Scalar(255/(col+1), 0,           0),           -1);
        cv::rectangle(image, cv::Point(750, 250 * col), cv::Point(1000, 250 * (col+1)), cv::Scalar(255/(col+1), 255/(col+1), 0),           -1);
    }

    cv::imwrite("image.jpg", image);
}

QTEST_MAIN(DominantColorTest)
