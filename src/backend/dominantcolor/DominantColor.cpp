#include "DominantColor.h"
#include "backend/exception/Exception.h"

#include <opencv2/opencv.hpp>
#include <algorithm>

void DominantColor::readImage(const std::string &name) {
    this->img = cv::imread(name, 1);

    if (this->img.empty())
        throw Exception{"empty image"};
}

int DominantColor::getNumOfClusters() {
    // key = color, value = color's count
    std::map<int, int> clusters;

    // Go though all pixels
    for (size_t i = 0; i < this->img.total(); i++) {
        // Access to matrix
        auto v = this->img.at<cv::Vec3b>(i);

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

void DominantColor::performKMeans() {
    // Map the image to the samples
    // We do this because in order to perform kmeans algorithm,
    // this->img.depth() must be of type CV_32F
    cv::Mat samples(this->img.total(), 3, CV_32F);
    auto samplesPtr = samples.ptr<float>(0);

    // Rearrange this->img from
    // (b00,g00,r00) (b01,g01,r01) (b02,g02,r02) ...
    // (b10,g10,r10) (b11,g11,r11) (b12,g12,r12) ...
    // to
    // (r00,g00,b00)
    // (r01,g01,b01)
    // ...
    for (int row = 0; row != img.rows; ++row) {
        auto imgBeginPtr = img.ptr<uchar>(row);
        auto imgEndPtr = imgBeginPtr + this->img.cols * this->img.channels();

        while (imgBeginPtr != imgEndPtr) {
            samplesPtr[0] = imgBeginPtr[2];
            samplesPtr[1] = imgBeginPtr[1];
            samplesPtr[2] = imgBeginPtr[0];

            samplesPtr += 3;
            imgBeginPtr += 3;
        }
    }

    // Apply kmeans algorithm
    int clusterCount = std::min(8, getNumOfClusters());
    cv::Mat labels, centers;
    int attempts = 5;

    cv::kmeans(
            samples,
            clusterCount,
            labels,
            cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.01),
            attempts,
            cv::KMEANS_PP_CENTERS,
            centers
    );

    // Extract colors from centers
    for (int row = 0; row != centers.rows; ++row) {
        int r = static_cast<int>(centers.at<float>(row, 0));
        int g = static_cast<int>(centers.at<float>(row, 1));
        int b = static_cast<int>(centers.at<float>(row, 2));

        this->colors.push_back({r, g, b});
    }
}
