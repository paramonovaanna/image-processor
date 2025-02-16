#pragma once

#include <vector>
#include <string_view>

using FilterParams = std::vector<std::string_view>;

struct FilterArgs {
    std::string_view filter_name;
    FilterParams params;
};