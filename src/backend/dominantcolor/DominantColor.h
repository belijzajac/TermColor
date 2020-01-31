#ifndef DOMINANTCOLOR_H
#define DOMINANTCOLOR_H

#include <memory>
#include <vector>

namespace TermColor::Utils {

struct color {
    int r, g, b;

    const std::string getCommaSeparatedStr() const;
    const std::string getHexStr() const;

    // some operators
    friend bool operator==(const color &lhs, const color &rhs);
    friend bool operator!=(const color &lhs, const color &rhs);
};

} // TermColor::Utils

namespace TermColor {

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
    const std::vector<TermColor::Utils::color> getColors() const;

    // Returns predefined colors for background and foreground
    const std::vector<TermColor::Utils::color> getBGFGColors(bool isDark = true) const;

    // Returns an intense colors copy of given colors
    const std::vector<TermColor::Utils::color> intenseColors(const std::vector<TermColor::Utils::color> &colors) const;

private:
    std::unique_ptr<DominantColorImpl> pimpl_;
};

}

#endif // DOMINANTCOLOR_H
