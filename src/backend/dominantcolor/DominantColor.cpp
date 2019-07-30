#include "DominantColor.h"
#include "backend/exception/Exception.h"

#include <opencv2/opencv.hpp>
#include <algorithm>

class DominantColor::DominantColorImpl {
public:
    DominantColorImpl() = default;

    void readImage(const std::string &name);
    void performKMeans();
    const std::vector<color> getColors() const;
    const std::vector<color> intenseColors() const;

private:
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
};

void DominantColor::DominantColorImpl::readImage(const std::string &name) {
    img_ = cv::imread(name, 1);

    if (img_.empty())
        throw Exception{"empty image"};
}

void DominantColor::DominantColorImpl::performKMeans() {
    // Map and rearrange image
    cv::Mat rearrangedImg = mapAndRearrange();

    // Apply kmeans algorithm
    int clusterCount = std::min(8, getNumOfClusters());
    cv::Mat labels, centers;
    int attempts = 5;

    cv::kmeans(
            rearrangedImg,
            clusterCount,
            labels,
            cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.01),
            attempts,
            cv::KMEANS_PP_CENTERS,
            centers
    );

    // Extract colors from centers
    colors_ = extractColors(centers);
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

// A helper function to clamp the color value to 255 if it goes over
int doBrightenColor(int color, double multiplier) {
    return std::min(255, static_cast<int>(color * multiplier));
}

// Processes each color to function doBrightenColor
const color brightenColor(const color &c, double multiplier) {
    return { doBrightenColor(c.r, multiplier),
             doBrightenColor(c.g, multiplier),
             doBrightenColor(c.b, multiplier)
    };
}

const std::vector<color> DominantColor::DominantColorImpl::intenseColors() const {
    auto oldColors = colors_;
    std::vector<color> intenseColors;

    std::for_each(oldColors.begin(), oldColors.end(), [&intenseColors](const color &color){
        intenseColors.push_back({brightenColor(color, 1.5)});
    });

    return intenseColors;
}

/// DominantColor

DominantColor::DominantColor() {
    pimpl_ = std::make_unique<DominantColorImpl>();
}

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

const std::vector<color> DominantColor::intenseColors() const {
    return pimpl_->intenseColors();
}
