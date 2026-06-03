#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

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

namespace wr {

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
std::vector<DictWord> parse_dict_file(const std::string &filename);
std::vector<DictWord> parse_dict_file(const std::filesystem::path &path);

/*! Parse simple dictionary-like string.
 *
 * The dictionary string should have a structure:
 * '''
 * word1 @ meaning1 @
 * word2 @ meaning2 @
 * '''
 * You can have as many spaces and new line characters in-between.
 *
 * \param dict_string Dict string.
 * \return List of parsed dictionary pairs word-meaning.
 */
std::vector<DictWord> parse_dict_file_content(const std::string &dict_string);

} // namespace wr