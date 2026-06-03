#pragma once

#include <QString>
#include <concepts>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace wr {

// ------------------------------ TYPES -----------------------------------

template<typename T>
concept StringType = std::same_as<T, const char *> || std::same_as<std::decay_t<T>, std::string>
                     || std::same_as<std::decay_t<T>, QString>;

//! \enum Supported encodings for dict files.
enum class DictFileEncoding : char {
    not_set = 0,
    utf8 = 1,
    utf16 = 2,
    latin1 = 3,
};

// ------------------------------ GLOBALS ---------------------------------

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

class unknown_encoding : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

// ------------------------------ FUNCTIONS -------------------------------

template<typename T>
inline std::string _to_string(T s)
{
    return std::string(s);
}

template<>
inline std::string _to_string(QString s)
{
    return s.toStdString();
}

//! \brief Encoding as string to encoding enum.
template<StringType ST>
inline DictFileEncoding encoding_from_str(ST s)
{
    if (s == "utf-8")
        return DictFileEncoding::utf8;
    if (s == "utf-16")
        return DictFileEncoding::utf16;
    if (s == "latin-1")
        return DictFileEncoding::latin1;

    throw wr::unknown_encoding(std::string("Unknown encoding '") + _to_string(s) + "'");
}

} // namespace wr
