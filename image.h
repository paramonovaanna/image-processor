#pragma once

#include <vector>
#include <cstddef>

struct Pixel {
    double blue_ = 0;
    double red_ = 0;
    double green_ = 0;

    Pixel operator*(const double value) const;

    Pixel& operator+=(const Pixel& add_pixel);
};

class Image {
public:
    using PixelMatrix = std::vector<std::vector<Pixel>>;

    Image() = default;

    explicit Image(PixelMatrix array);

    size_t GetWidth() const;

    size_t GetHeight() const;

    void Resize(size_t new_width, size_t new_height);

    Pixel& GetPixel(size_t x, size_t y);

    const Pixel& GetPixel(size_t x, size_t y) const;

private:
    PixelMatrix pixel_matrix_;
};