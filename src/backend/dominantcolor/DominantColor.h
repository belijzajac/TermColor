#ifndef DOMINANTCOLOR_H
#define DOMINANTCOLOR_H

#include <memory>
#include <vector>
#include <string_view>

namespace TermColor::Utils {

struct color {
    size_t r, g, b;
    std::string getCommaSeparatedStr() const;
    std::string getHexStr() const;

    // some operators
    friend bool operator==(const color &lhs, const color &rhs);
    friend bool operator!=(const color &lhs, const color &rhs);
};

} // TermColor::Utils

namespace TermColor {

using color_t = Utils::color;

class DominantColor {
    class DominantColorImpl;

public:
    DominantColor();
    ~DominantColor();

    // This function attempts to read to img;
    void readImage(std::string_view name);

    // This function obtains dominant colors using kmeans algorithm
    void performKMeans();

    // Gets dominant colors from an image
    std::vector<color_t> getColors() const;

    // Returns predefined colors for background and foreground
    std::vector<color_t> getBGFGColors(bool isDark = true) const;

    // Returns an intense colors copy of given colors
    std::vector<color_t> intenseColors(const std::vector<color_t> &colors) const;

private:
    std::unique_ptr<DominantColorImpl> pimpl_;
};

}

#endif // DOMINANTCOLOR_H
