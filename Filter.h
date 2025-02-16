#pragma once

#include "image.h"
#include <cstddef>

using FilterMatrix = std::vector<std::vector<double>>;

class FilterMatrixApplication {
public:
    FilterMatrixApplication(double corner, double edge, double center);

    Pixel GetNewPixel(const Image& org_image, size_t x, size_t y) const;

    size_t FixCoord(int coord, size_t border) const;

    Image ApplyFilterMatrix(const Image& image) const;

private:
    FilterMatrix matrix_;

    FilterMatrix GetFilterMatrix(double edge, double corner, double center);
};

class Filter {
public:
    virtual Image ApplyTo(const Image&) const = 0;
    virtual ~Filter() = default;
};

class Crop : public Filter {
public:
    Crop(size_t width, size_t height);

    Image ApplyTo(const Image& image) const override;

private:
    size_t width_ = 0;
    size_t height_ = 0;
};

class GrayScale : public Filter {
public:
    const double RED_CONST = 0.299;
    const double BLUE_CONST = 0.114;
    const double GREEN_CONST = 0.587;

    double GetNewColor(Pixel& pixel) const;

    Image ApplyTo(const Image& image) const override;
};

class Negative : public Filter {
public:
    Image ApplyTo(const Image& image) const override;
};

class Sharpening : public Filter {
public:
    Image ApplyTo(const Image& image) const override;

private:
    const double sharp_const_corners_ = 0;
    const double sharp_const_edges_ = -1;
    const double sharp_const_middle_ = 5;
};

class EdgeDetection : public GrayScale {
public:
    explicit EdgeDetection(double threshold);

    Image ApplyTo(const Image& image) const override;

    double GetBrightness(double color) const;

private:
    const double ed_const_corners_ = 0;
    const double ed_const_edges_ = -1;
    const double ed_const_middle_ = 4;

    double threshold_ = 0;
};
