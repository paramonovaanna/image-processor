#pragma once

#include "FilterArgs.h"
#include <string>

struct CommandArgs {
    std::string input_filename;
    std::string output_filename;
    std::vector<FilterArgs> filters;
};

class CommandParser {
public:
    CommandParser(int argc, char** argv);

    CommandArgs ParseArgs(int argc, char** argv) const;

    std::vector<FilterArgs> ParseFilters(int argc, char** args) const;

    CommandArgs& GetFiltersData();

private:
    CommandArgs filters_data_;

    void CheckArgc(int argc);
};