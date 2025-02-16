#include "Filter.h"
#include <algorithm>

#include "image.h"

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

FilterMatrixApplication::FilterMatrixApplication(double corner, double edge, double center) {
    matrix_ = GetFilterMatrix(edge, corner, center);
};

Pixel FilterMatrixApplication::GetNewPixel(const Image& org_image, size_t row, size_t col) const {
    Pixel new_pixel;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            size_t temp_row = FixCoord(static_cast<int>(row) + i, org_image.GetHeight());
            size_t temp_col = FixCoord(static_cast<int>(col) + j, org_image.GetWidth());
            new_pixel += org_image.GetPixel(temp_row, temp_col) * matrix_[i + 1][j + 1];
        }
    }
    new_pixel.blue_ = std::clamp(new_pixel.blue_, 0.0, 1.0);
    new_pixel.green_ = std::clamp(new_pixel.green_, 0.0, 1.0);
    new_pixel.red_ = std::clamp(new_pixel.red_, 0.0, 1.0);
    return new_pixel;
}

size_t FilterMatrixApplication::FixCoord(int coord, size_t border) const {
    if (coord < 0) {
        return 0;
    }
    if (coord >= border) {
        return border - 1;
    }
    return coord;
}

Image FilterMatrixApplication::ApplyFilterMatrix(const Image& image) const {
    Image new_image = image;
    for (size_t x = 0; x < image.GetHeight(); ++x) {
        for (size_t y = 0; y < image.GetWidth(); ++y) {
            Pixel& pixel = new_image.GetPixel(x, y);
            pixel = GetNewPixel(image, x, y);
        }
    }
    return new_image;
}

FilterMatrix FilterMatrixApplication::GetFilterMatrix(double edge, double corner, double center) {
    FilterMatrix result({{corner, edge, corner}, {edge, center, edge}, {corner, edge, corner}});
    return result;
}

Image Crop::ApplyTo(const Image& image) const {
    Image cropped_image = image;
    cropped_image.Resize(width_, height_);
    return cropped_image;
}

double GrayScale::GetNewColor(Pixel& pixel) const {
    return RED_CONST * pixel.red_ + BLUE_CONST * pixel.blue_ + GREEN_CONST * pixel.green_;
}

Image GrayScale::ApplyTo(const Image& image) const {
    Image gs_image = image;
    for (size_t x = 0; x < gs_image.GetHeight(); ++x) {
        for (size_t y = 0; y < gs_image.GetWidth(); ++y) {
            Pixel& pixel = gs_image.GetPixel(x, y);
            double new_color = GetNewColor(pixel);
            pixel.blue_ = new_color;
            pixel.green_ = new_color;
            pixel.red_ = new_color;
        }
    }
    return gs_image;
}

Image Negative::ApplyTo(const Image& image) const {
    Image neg_image = image;
    for (size_t x = 0; x < neg_image.GetHeight(); ++x) {
        for (size_t y = 0; y < neg_image.GetWidth(); ++y) {
            Pixel& pixel = neg_image.GetPixel(x, y);
            pixel.blue_ = 1 - pixel.blue_;
            pixel.green_ = 1 - pixel.green_;
            pixel.red_ = 1 - pixel.red_;
        }
    }
    return neg_image;
}

Image Sharpening::ApplyTo(const Image& image) const {
    FilterMatrixApplication applier(sharp_const_corners_, sharp_const_edges_, sharp_const_middle_);
    Image sharp_image = applier.ApplyFilterMatrix(image);
    return sharp_image;
}

EdgeDetection::EdgeDetection(double threshold) : threshold_(threshold){};

double EdgeDetection::GetBrightness(double color) const {
    if (color > threshold_) {
        return 1.0;
    }
    return 0.0;
}

Image EdgeDetection::ApplyTo(const Image& image) const {
    GrayScale filter;
    Image gs_image = filter.ApplyTo(image);

    FilterMatrixApplication applier(ed_const_corners_, ed_const_edges_, ed_const_middle_);
    Image ed_image = applier.ApplyFilterMatrix(gs_image);

    for (size_t x = 0; x < image.GetHeight(); ++x) {
        for (size_t y = 0; y < image.GetWidth(); ++y) {
            Pixel& pixel = ed_image.GetPixel(x, y);
            pixel.blue_ = GetBrightness(pixel.blue_);
            pixel.green_ = GetBrightness(pixel.green_);
            pixel.red_ = GetBrightness(pixel.red_);
        }
    }

    return ed_image;
};
