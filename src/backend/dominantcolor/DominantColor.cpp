#include "DominantColor.h"
#include "backend/exception/Exception.h"

#include <opencv2/opencv.hpp>
#include <algorithm>

namespace TermColor {

using namespace TermColor::Utils;

class DominantColor::DominantColorImpl {
public:
    DominantColorImpl() = default;

    void readImage(const std::string &name);
    void performKMeans();
    const std::vector<color> getColors() const;
    const std::vector<color> getBGFGColors(bool) const;
    const std::vector<color> intenseColors(const std::vector<color> &colors) const;

private:
    // Does K-Mean's algorithm
    void doKMeans(const cv::Mat &img, cv::Mat &labels, cv::Mat &centers, int clusterCount, int attempts);

    // This function returns the number of unique colors (clusters)
    // that appear in the image
    int getNumOfClusters() const;

    // Basically creates a copy of rearranged image in such way
    // that {r,g,b} is continuously filled down the first column,
    // instead of going further in a row
    const cv::Mat mapAndRearrange() const;

    // Extracts colors from centers
    const std::vector<color> extractColors(const cv::Mat &centers) const;

private:
    cv::Mat img_;
    std::vector<color> colors_;

    // BG and FG colors
    std::vector<color> darkBGFG {{40, 38, 45}};
    std::vector<color> lightBGFG {{255, 255, 255}};
};

void DominantColor::DominantColorImpl::readImage(const std::string &name) {
    img_ = cv::imread(name, 1);

    if (img_.empty())
        throw Exception{"Empty image (or not an image)"};
}

void DominantColor::DominantColorImpl::doKMeans(const cv::Mat &img, cv::Mat &labels, cv::Mat &centers,
                                                int clusterCount = 1, int attempts = 1) {
    cv::kmeans(
            img,
            clusterCount,
            labels,
            cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.01),
            attempts,
            cv::KMEANS_PP_CENTERS,
            centers
    );
}

void DominantColor::DominantColorImpl::performKMeans() {
    // Map and rearrange image
    cv::Mat rearrangedImg = mapAndRearrange();

    // Data we'll pass to kmeans function
    int clusterCount = std::min(8, getNumOfClusters());
    cv::Mat labels, centers;

    // An image doesn't have sufficient amount of colors (we need 8 for both regular and intense colors)
    if (clusterCount != 8)
        throw Exception{"Image doesn't have enough colors"};

    // Apply K-Mean's algorithm for regular colors
    doKMeans(rearrangedImg, labels, centers, clusterCount, 5);

    // Extract colors from centers
    colors_ = extractColors(centers);

    // Apply K-Mean's algorithm for foreground color
    cv::Mat centers_2;
    doKMeans(rearrangedImg, labels, centers_2);

    // Make the color brighter
    auto fgColor = intenseColors(extractColors(centers_2));

    // Append new color
    darkBGFG.push_back(fgColor.at(0));
    lightBGFG.push_back(fgColor.at(0));
}

int DominantColor::DominantColorImpl::getNumOfClusters() const {
    // key = color, value = color's count
    std::map<int, int> clusters;

    // Go though all pixels
    for (size_t i = 0; i < img_.total(); i++) {
        // Access to matrix
        auto v = img_.at<cv::Vec3b>(i);

        // An integer representation of the pixel
        int c = (v[0] << 16) | (v[1] << 8) | (v[2]);

        // Count unique pixels
        if (clusters.find(c) != clusters.end())
            clusters[c]++;
        else
            clusters[c] = 0;
    }

    // We will throw away all clusters that have < 200 elements
    for (auto it = clusters.begin(); it != clusters.end();) {
        if (it->second < 200)
            it = clusters.erase(it);
        else
            ++it;
    }

    return static_cast<int>(clusters.size());
}

const cv::Mat DominantColor::DominantColorImpl::mapAndRearrange() const {
    // We do this because in order to perform kmeans algorithm,
    // img_.depth() must be of type CV_32F
    cv::Mat modifiedImg(img_.total(), 3, CV_32F);
    auto modifiedImgPtr = modifiedImg.ptr<float>(0);

    // Rearrange img_ from
    // (b00,g00,r00) (b01,g01,r01) (b02,g02,r02) ...
    // (b10,g10,r10) (b11,g11,r11) (b12,g12,r12) ...
    // to
    // (r00,g00,b00)
    // (r01,g01,b01)
    // ...
    for (int row = 0; row != img_.rows; ++row) {
        auto imgBeginPtr = img_.ptr<uchar>(row);
        auto imgEndPtr = imgBeginPtr + img_.cols * img_.channels();

        while (imgBeginPtr != imgEndPtr) {
            modifiedImgPtr[0] = imgBeginPtr[2];
            modifiedImgPtr[1] = imgBeginPtr[1];
            modifiedImgPtr[2] = imgBeginPtr[0];

            modifiedImgPtr += 3;
            imgBeginPtr += 3;
        }
    }

    return modifiedImg;
}

const std::vector<color> DominantColor::DominantColorImpl::extractColors(const cv::Mat &centers) const {
    std::vector<color> extracted;

    for (int row = 0; row != centers.rows; ++row) {
        int r = static_cast<int>(centers.at<float>(row, 0));
        int g = static_cast<int>(centers.at<float>(row, 1));
        int b = static_cast<int>(centers.at<float>(row, 2));

        extracted.push_back({r, g, b});
    }

    return extracted;
}

const std::vector<color> DominantColor::DominantColorImpl::getColors() const {
    return colors_;
}

// Linearly interpolate between the original color and the target color (often white)
// Formula: C = A + (B - A) * time,
// C - in-between color, A and B - two colors, 0 <= time < 16
const color brightenColor(const color &c, int time) {
    const auto whiteColor = color{255, 255, 255};

    return color{
        c.r + (time * (whiteColor.r - c.r)) / 15,
        c.g + (time * (whiteColor.g - c.g)) / 15,
        c.b + (time * (whiteColor.b - c.b)) / 15,
    };
}

const std::vector<color> DominantColor::DominantColorImpl::intenseColors(const std::vector<color> &colors) const {
    std::vector<color> intenseColors;

    std::for_each(colors.begin(), colors.end(), [&intenseColors](const color &color){
        intenseColors.push_back({brightenColor(color, 6)});
    });

    return intenseColors;
}

const std::vector<color> DominantColor::DominantColorImpl::getBGFGColors(bool isDark) const {
    return (isDark)? darkBGFG : lightBGFG;
}

/// DominantColor

DominantColor::DominantColor() : pimpl_(std::make_unique<DominantColorImpl>()) {}

DominantColor::~DominantColor() = default;

void DominantColor::readImage(const std::string &name) {
    pimpl_->readImage(name);
}

void DominantColor::performKMeans() {
    pimpl_->performKMeans();
}

const std::vector<color> DominantColor::getColors() const {
    return pimpl_->getColors();
}

const std::vector<color> DominantColor::intenseColors(const std::vector<color> &colors) const {
    return pimpl_->intenseColors(colors);
}

const std::vector<color> DominantColor::getBGFGColors(bool isDark) const {
    return pimpl_->getBGFGColors(isDark);
}

// color

const std::string color::getCommaSeparatedStr() const {
    return std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b);
}

const std::string color::getHexStr() const {
    std::stringstream ss;

    ss << std::hex << r << std::hex << g << std::hex << b;
    return std::string{"#" + ss.str()};
}

namespace Utils {

bool operator==(const color &lhs, const color &rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

bool operator!=(const color &lhs, const color &rhs) {
    return !(lhs == rhs);
}

} // TermColor::Utils

} // TermColor
