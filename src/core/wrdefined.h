#pragma once

#include <stdexcept>

namespace wr {

//! \brief Default folder where the application will try to find the dict files.
constexpr const char *gDICT_DEFAULT_FLD = "./dictionaries";

// ------------------------------ EXCEPTIONS ------------------------------

class file_not_found : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class dict_file_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class game_logic_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

} // namespace wr
