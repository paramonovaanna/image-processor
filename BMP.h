#pragma once

#include "image.h"

#include <filesystem>
#include <fstream>

const int MAX_COLOR = 255;

struct BmpHeader {
public:
    static const int BMPOFFSETDEFAULT = 54;

    BmpHeader() = default;

    BmpHeader(std::ifstream& file, std::filesystem::path file_name);

    char id_field_[2];
    uint32_t size_ = 0;
    uint16_t app_specific1_ = 0;
    uint16_t app_specific2_ = 0;
    uint32_t offset_ = BMPOFFSETDEFAULT;

    void Load(std::ifstream& file);

    void Check(std::filesystem::path file_name) const;

    void Write(std::ofstream& file) const;
};

struct DibHeader {

    DibHeader() = default;

    DibHeader(std::ifstream& file, std::filesystem::path file_name);

    static const uint32_t DIBHEADERSIZEDEFAULT = 40;
    static const uint32_t COLORPANELSDEFAULT = 1;
    static const uint16_t BITSPERPIXELDEFAULT = 24;
    static const uint32_t BIRGBDEFAULT = 0;
    static const int BMPOFFSETDEFAULT = 54;
    static const uint32_t COLORSDEFAULT = 0;
    static const uint32_t IMPORTANTCOLORSDEFAULT = 0;

    uint32_t dib_size_ = DIBHEADERSIZEDEFAULT;
    int32_t width_ = 0;
    int32_t height_ = 0;
    uint16_t color_panels_ = COLORPANELSDEFAULT;
    uint16_t bits_per_pixel_ = BITSPERPIXELDEFAULT;
    uint32_t bi_rgb_ = BIRGBDEFAULT;
    uint32_t data_size_ = 0;
    int32_t resolution_horizontal_ = 0;
    int32_t resolution_vertical_ = 0;
    uint32_t colors_ = COLORSDEFAULT;
    uint32_t important_colors_ = IMPORTANTCOLORSDEFAULT;

    void Load(std::ifstream& file);

    void Check(std::filesystem::path file_name) const;

    void Write(std::ofstream& file) const;
};

class Bmp : public Image {
public:
    explicit Bmp(std::filesystem::path file_name);

    explicit Bmp(Image image);

    void CheckInputFileExists(std::filesystem::path file_name);

    void Save(std::filesystem::path file_name);

    void WritePixelMatrix(std::ofstream& file);

    Image GetImage();

private:
    BmpHeader bmp_header_;
    DibHeader dib_header_;

    void ReadPixelMatrix(std::ifstream& file);
};