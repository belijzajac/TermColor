#ifndef DOMINANTCOLOR_H
#define DOMINANTCOLOR_H

#include <string>
#include <memory>
#include <vector>

struct color {
    int r, g, b;
};

class DominantColor {
    class DominantColorImpl;

public:
    DominantColor();
    ~DominantColor();

    // This function attempts to read to img;
    void readImage(const std::string &name);

    // This function obtains dominant colors using kmeans algorithm
    void performKMeans();

    // Gets dominant colors from an image
    const std::vector<color> getColors() const;

    // Returns predefined colors for background and foreground
    const std::vector<color> getBGFGColors(bool isDark = true) const;

    // Returns an intense colors copy of given colors
    const std::vector<color> intenseColors(const std::vector<color> &colors) const;

private:
    std::unique_ptr<DominantColorImpl> pimpl_;
};

#endif // DOMINANTCOLOR_H
