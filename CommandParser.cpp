#include "CommandParser.h"

#include <stdexcept>

CommandParser::CommandParser(int argc, char** argv) {
    CheckArgc(argc);
    filters_data_ = ParseArgs(argc, argv);
}

void CommandParser::CheckArgc(int argc) {
    if (argc == 2) {
        throw std::invalid_argument("Photo editor takes at least 2 arguments: input_filename and output_filename");
    }
}

CommandArgs CommandParser::ParseArgs(int argc, char** argv) const {
    CommandArgs result;
    result.input_filename = argv[1];
    result.output_filename = argv[2];
    result.filters = ParseFilters(argc, argv);
    return result;
}

std::vector<FilterArgs> CommandParser::ParseFilters(int argc, char** args) const {
    std::vector<FilterArgs> result;
    for (size_t i = 3; i < argc; ++i) {
        if (args[i][0] == '-') {
            FilterArgs filter_desc;
            filter_desc.filter_name = std::string_view(args[i] + 1);
            result.push_back(filter_desc);
        } else {
            if (!result.empty()) {
                result.back().params.push_back(args[i]);
            } else {
                throw std::invalid_argument("Could not correctly interprete the filters input");
            }
        }
    }
    return result;
}

CommandArgs& CommandParser::GetFiltersData() {
    return filters_data_;
}