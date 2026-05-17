#pragma once

#include <filesystem>
#include <sstream>

namespace wr {

const char *gDICT_DEFAULT_FLD = "./dictionaries";

class file_not_found : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

} // namespace wr
