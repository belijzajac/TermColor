#ifndef DOMINANTCOLOR_H
#define DOMINANTCOLOR_H

#include <opencv2/core/mat.hpp>
#include <string>

class DominantColor {
private:
    cv::Mat img;

    struct color {
        int r, g, b;
    };

    std::vector<color> colors;

public:
    DominantColor() = default;
    ~DominantColor() = default;

    // This function attempts to read to img;
    void readImage(const std::string &name);

    // This function returns the number of unique colors (clusters)
    // that appear in the image
    int getNumOfClusters();

    // This function obtains dominant colors using kmeans algorithm
    void performKMeans();

    const std::vector<color> getColors() const { return colors; }
};

#endif // DOMINANTCOLOR_H
