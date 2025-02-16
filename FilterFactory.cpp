#include "FilterFactory.h"
#include "Filter.h"

#include <memory>
#include <map>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <iostream>

std::unique_ptr<Filter> CropFactory::Create(const FilterParams& params) const {
    if (params.size() != 2) {
        throw std::invalid_argument("Crop filter takes 2 parameters");
    }
    size_t width = std::stoi(static_cast<std::string>(params.at(0)));
    size_t height = std::stoi(static_cast<std::string>(params.at(1)));
    return std::make_unique<Crop>(width, height);
}

std::string CropFactory::GetHelpMessage() const {
    std::string message =
        "Crop filter cuts the given image. The filter takes 2 parameters: width and height of the resulting image. "
        "Both "
        "must be unsigned integers. Command: -crop width height";
    return message;
}

std::unique_ptr<Filter> GsFactory::Create(const FilterParams& params) const {
    if (!params.empty()) {
        throw std::invalid_argument("GrayScale filter doesn't take any arguments");
    }
    return std::make_unique<GrayScale>();
}

std::string GsFactory::GetHelpMessage() const {
    std::string message =
        "GrayScale filter transforms the image into gray shades. The filter doesn't take any arguments. Command: -gs";
    return message;
}

std::unique_ptr<Filter> NegFactory::Create(const FilterParams& params) const {
    if (!params.empty()) {
        throw std::invalid_argument("Negative filter doesn't take any arguments");
    }
    return std::make_unique<Negative>();
}

std::string NegFactory::GetHelpMessage() const {
    std::string message =
        "Negative filter inverts the image colors. The filter doesn't take any arguments. Command: -neg";
    return message;
}

std::unique_ptr<Filter> SharpFactory::Create(const FilterParams& params) const {
    if (!params.empty()) {
        throw std::invalid_argument("Sharpening filter doesn't take any arguments");
    }
    return std::make_unique<Sharpening>();
}

std::string SharpFactory::GetHelpMessage() const {
    std::string message =
        "Sharpening filter enhances the edges of object. The filter doesn't take any arguments. Command: -sharp";
    return message;
}

std::unique_ptr<Filter> EDFactory::Create(const FilterParams& params) const {
    if (params.size() != 1) {
        throw std::invalid_argument("Edge Detection filter takes 1 parameter");
    }
    double threshold = std::stod(static_cast<std::string>(params.at(0)));
    if (threshold < 0.0 || threshold > 1.0) {
        throw std::invalid_argument("Threshold must be a value from [0.0, 1.0]");
    }
    return std::make_unique<EdgeDetection>(threshold);
}

std::string EDFactory::GetHelpMessage() const {
    std::string message =
        "Edge Detection filter identifies edges, where the image brightness changed sharply. The filter takes 1 "
        "parameter, an unsigned fractional value called threshold from [0.0, 1.0]. Command: -edge threshold";
    return message;
}

std::vector<std::unique_ptr<Filter>> CreateFilters(std::vector<FilterArgs>& filters_data) {
    std::map<std::string_view, std::unique_ptr<FilterFactory>> available_filters_map;
    available_filters_map.emplace(std::string_view("crop"), std::make_unique<CropFactory>());
    available_filters_map.emplace(std::string_view("gs"), std::make_unique<GsFactory>());
    available_filters_map.emplace(std::string_view("neg"), std::make_unique<NegFactory>());
    available_filters_map.emplace(std::string_view("sharp"), std::make_unique<SharpFactory>());
    available_filters_map.emplace(std::string_view("edge"), std::make_unique<EDFactory>());

    std::vector<std::unique_ptr<Filter>> result;

    if (filters_data.empty()) {
        std::cout << "Photo editor with the following implemented filters:" << std::endl;
        for (auto& [filter_name, filter_ptr] : available_filters_map) {
            std::cout << filter_ptr->GetHelpMessage() << std::endl;
        }
        std::cout << "To start using the photo editor, enter the path of the original image and the path where the "
                     "result will be written to"
                  << std::endl;
        std::cout << "End of the help message" << std::endl;
    }

    for (FilterArgs& filter_data : filters_data) {
        if (!available_filters_map.contains(filter_data.filter_name)) {
            throw std::invalid_argument(
                "The given filter is not implemented. Available filters are Crop, GrayScale, Negative, Sharpening, "
                "Edge Detection");
        }
        result.push_back(available_filters_map.at(filter_data.filter_name)->Create(filter_data.params));
    }
    return result;
}