#include "CommandParser.h"
#include "BMP.h"
#include "FilterFactory.h"

int main(int argc, char** argv) {
    CommandParser parsed_command(argc, argv);

    CommandArgs& command_args = parsed_command.GetFiltersData();

    Bmp input_file(command_args.input_filename);

    std::vector<std::unique_ptr<Filter>> filters = CreateFilters(command_args.filters);

    Image image = input_file.GetImage();

    for (std::unique_ptr<Filter>& filter : filters) {
        image = filter->ApplyTo(image);
    }

    Bmp result(image);

    result.Save(command_args.output_filename);

    return 0;
}
