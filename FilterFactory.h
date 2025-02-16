#pragma once

#include "FilterArgs.h"
#include "Filter.h"

#include <memory>

std::vector<std::unique_ptr<Filter>> CreateFilters(std::vector<FilterArgs>& filters_data);

struct FilterFactory {
    virtual std::unique_ptr<Filter> Create(const FilterParams& params) const = 0;
    virtual std::string GetHelpMessage() const = 0;
    virtual ~FilterFactory() = default;
};

struct CropFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParams& params) const override;
    std::string GetHelpMessage() const override;
};

struct GsFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParams& params) const override;
    std::string GetHelpMessage() const override;
};

struct NegFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParams& params) const override;
    std::string GetHelpMessage() const override;
};

struct SharpFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParams& params) const override;
    std::string GetHelpMessage() const override;
};

struct EDFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParams& params) const override;
    std::string GetHelpMessage() const override;
};
