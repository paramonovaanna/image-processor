#include "BMP.h"
#include "little_endian.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>

template <typename T>
T Read(std::ifstream& file) {
    char temp[sizeof(T)];
    file.read(temp, sizeof(T));
    return ReadLittleEndian<T>(temp);
}

template <typename T>
void Write(std::ofstream& file, T value) {
    file.write(TransformToLittleEndian(value).data(), sizeof(T));
}

Bmp::Bmp(std::filesystem::path file_name) {
    CheckInputFileExists(file_name);
    std::ifstream file(file_name, std::ios_base::binary | std::ios_base::in);
    bmp_header_ = BmpHeader(file, file_name);
    dib_header_ = DibHeader(file, file_name);
    ReadPixelMatrix(file);
}

void Bmp::CheckInputFileExists(std::filesystem::path file_name) {
    if (!std::filesystem::exists(file_name)) {
        throw std::invalid_argument("Invalid input file_name: such image doesn't exists");
    }
}

Bmp::Bmp(Image image) : Image{std::move(image)} {
    dib_header_.width_ = static_cast<int32_t>(GetWidth());
    dib_header_.height_ = static_cast<int32_t>(GetHeight());
    dib_header_.data_size_ = (dib_header_.width_ * 3 + dib_header_.width_ % 4) * dib_header_.height_;
    bmp_header_.size_ = bmp_header_.BMPOFFSETDEFAULT + dib_header_.data_size_;
}

void Bmp::Save(std::filesystem::path file_name) {
    std::ofstream file(file_name, std::ios_base::binary | std::ios_base::out);
    bmp_header_.Write(file);
    dib_header_.Write(file);
    WritePixelMatrix(file);
}

void Bmp::WritePixelMatrix(std::ofstream& file) {
    for (size_t row = dib_header_.height_ - 1; ~row; --row) {
        for (size_t col = 0; col < dib_header_.width_; ++col) {
            const Pixel& pixel = GetPixel(row, col);
            Write(file, static_cast<uint8_t>(pixel.blue_ * MAX_COLOR));
            Write(file, static_cast<uint8_t>(pixel.green_ * MAX_COLOR));
            Write(file, static_cast<uint8_t>(pixel.red_ * MAX_COLOR));
        }
        for (size_t k = 0; k < dib_header_.width_ % 4; ++k) {
            Write(file, static_cast<uint8_t>(0));
        }
    }
}

void Bmp::ReadPixelMatrix(std::ifstream& file) {
    Resize(static_cast<size_t>(dib_header_.width_), static_cast<size_t>(dib_header_.height_));
    for (size_t row = dib_header_.height_ - 1; ~row; --row) {
        for (size_t col = 0; col < dib_header_.width_; ++col) {
            Pixel& pixel = GetPixel(row, col);
            pixel.blue_ = static_cast<double>(Read<uint8_t>(file)) / MAX_COLOR;
            pixel.green_ = static_cast<double>(Read<uint8_t>(file)) / MAX_COLOR;
            pixel.red_ = static_cast<double>(Read<uint8_t>(file)) / MAX_COLOR;
        }
        for (size_t k = 0; k < dib_header_.width_ % 4; ++k) {
            Read<uint8_t>(file);
        }
    }
}

Image Bmp::GetImage() {
    return *this;
}

BmpHeader::BmpHeader(std::ifstream& file, std::filesystem::path file_name) {
    Load(file);
    Check(file_name);
}

void BmpHeader::Load(std::ifstream& file) {
    file.read(id_field_, sizeof(id_field_));
    size_ = Read<decltype(size_)>(file);
    app_specific1_ = Read<decltype(app_specific1_)>(file);
    app_specific2_ = Read<decltype(app_specific2_)>(file);
    offset_ = Read<decltype(offset_)>(file);
}

void BmpHeader::Write(std::ofstream& file) const {
    ::Write(file, 'B');
    ::Write(file, 'M');
    ::Write(file, size_);
    ::Write(file, app_specific1_);
    ::Write(file, app_specific2_);
    ::Write(file, offset_);
}

void BmpHeader::Check(std::filesystem::path file_name) const {
    if (id_field_[0] != 'B' || id_field_[1] != 'M') {
        throw std::invalid_argument("The loaded BMP-format image is damaged (invalied ID field)");
    }
    if (size_ != file_size(file_name)) {
        throw std::invalid_argument("The loaded BMP-format image is damaged (invalied size of the file field)");
    }
    if (offset_ != BMPOFFSETDEFAULT) {
        throw std::invalid_argument("The loaded BMP-format image is damaged (invalied bitmap offset field)");
    }
}

DibHeader::DibHeader(std::ifstream& file, std::filesystem::path file_name) {
    Load(file);
    Check(file_name);
}

void DibHeader::Load(std::ifstream& file) {
    dib_size_ = Read<decltype(dib_size_)>(file);
    width_ = Read<decltype(width_)>(file);
    height_ = Read<decltype(height_)>(file);
    color_panels_ = Read<decltype(color_panels_)>(file);
    bits_per_pixel_ = Read<decltype(bits_per_pixel_)>(file);
    bi_rgb_ = Read<decltype(bi_rgb_)>(file);
    data_size_ = Read<decltype(data_size_)>(file);
    resolution_horizontal_ = Read<decltype(resolution_horizontal_)>(file);
    resolution_vertical_ = Read<decltype(resolution_vertical_)>(file);
    colors_ = Read<decltype(colors_)>(file);
    important_colors_ = Read<decltype(important_colors_)>(file);
}

void DibHeader::Write(std::ofstream& file) const {
    ::Write(file, dib_size_);
    ::Write(file, width_);
    ::Write(file, height_);
    ::Write(file, color_panels_);
    ::Write(file, bits_per_pixel_);
    ::Write(file, bi_rgb_);
    ::Write(file, data_size_);
    ::Write(file, resolution_horizontal_);
    ::Write(file, resolution_vertical_);
    ::Write(file, colors_);
    ::Write(file, important_colors_);
}

void DibHeader::Check(std::filesystem::path file_name) const {
    if (dib_size_ != DIBHEADERSIZEDEFAULT) {
        throw std::invalid_argument("The loaded BMP-format image is damaged (invalied DIB Header size field)");
    }
    if (color_panels_ != COLORPANELSDEFAULT) {
        throw std::invalid_argument("The loaded BMP-format image is damaged (invalid number of color panels field)");
    }
    if (bits_per_pixel_ != BITSPERPIXELDEFAULT) {
        throw std::invalid_argument("The loaded BMP-format image is damaged (invalid bits per pixel filed)");
    }
    if (bi_rgb_ != BIRGBDEFAULT) {
        throw std::invalid_argument("The loaded BMP-format image is damaged (invalid BI RGB field)");
    }
    if (data_size_ != file_size(file_name) - BMPOFFSETDEFAULT && data_size_ != 0) {
        throw std::invalid_argument(
            "The loaded BMP-format image is damaged (invalid size of the raw bitmap data field)");
    }
    if (colors_ != COLORSDEFAULT) {
        throw std::invalid_argument(
            "The loaded BMP-format image is damaged (invalid number of colors in the pallete field)");
    }
    if (important_colors_ != IMPORTANTCOLORSDEFAULT) {
        throw std::invalid_argument(
            "The loaded BMP-format image is damaged (invalid number of important colors field)");
    }
}