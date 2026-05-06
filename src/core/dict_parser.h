#pragma once

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

//! \todo Delete if unnecessary
struct WDictWord
{
    std::wstring word;
    std::wstring meaning;

    WDictWord() = default;
    WDictWord(const std::wstring &word, const std::wstring &meaning);
};
std::wostream &operator<<(std::wostream &s, const WDictWord &wdw);

struct DictWord
{
    std::string word;
    std::string meaning;

    WDictWord to_wdict() const;

    DictWord() = default;
    DictWord(const std::string &word, const std::string &meaning);
};
std::ostream &operator<<(std::ostream &s, const DictWord &dw);

/*! Parse simple dictionary-like file.
 *
 * The dictionary file should have a structure:
 * '''
 * word1 @ meaning1 @
 * word2 @ meaning2 @
 * '''
 * You can have as many spaces and new line characters in-between.
 *
 * \param filename Simple dict filename.
 * \return List of parsed dictionary pairs word-meaning.
 */
std::list<DictWord> parse_dict_file(const std::string &filename);

std::list<DictWord> parse_dict_file_content(const std::string &file_content);

class dict_file_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};
