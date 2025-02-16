#include "image.h"

Pixel Pixel::operator*(const double value) const {
    Pixel result;
    result.blue_ = value * blue_;
    result.green_ = value * green_;
    result.red_ = value * red_;
    return result;
}

Pixel& Pixel::operator+=(const Pixel& add_pixel) {
    blue_ += add_pixel.blue_;
    green_ += add_pixel.green_;
    red_ += add_pixel.red_;
    return *this;
}

Image::Image(PixelMatrix array) {
    pixel_matrix_ = array;
}

size_t Image::GetHeight() const {
    return pixel_matrix_.size();
}

size_t Image::GetWidth() const {
    if (GetHeight() == 0) {
        return 0;
    }
    return pixel_matrix_[0].size();
}

void Image::Resize(size_t new_width, size_t new_height) {
    pixel_matrix_.resize(new_height);
    for (size_t i = 0; i < GetHeight(); ++i) {
        pixel_matrix_[i].resize(new_width);
    }
}

Pixel& Image::GetPixel(size_t x, size_t y) {
    return pixel_matrix_[x][y];
}

const Pixel& Image::GetPixel(size_t x, size_t y) const {
    return pixel_matrix_[x][y];
}