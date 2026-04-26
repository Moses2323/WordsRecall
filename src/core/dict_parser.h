#pragma once

#include <list>
#include <stdexcept>
#include <string>

struct DictWord
{
    std::string word;
    std::string meaning;
};

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
