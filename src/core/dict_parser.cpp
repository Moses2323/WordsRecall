#include "dict_parser.h"
#include <cctype>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <src/core/wrdefined.h>
#include <src/core/wrpaths.h>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

namespace {

//! \brief Separator symbol for the dict file.
constexpr char SEPARATOR = '@';

// Dict words indices from the file content string
struct _DictWordAsIndices
{
    // beginning of the word index
    size_t b_word = 0;
    // end of the word index (included)
    size_t e_word = 0;
    // beginning of the meaning index
    size_t b_meaning = 0;
    // end of the meaning index (included)
    size_t e_meaning = 0;

    _DictWordAsIndices() = default;
};
std::ostream &operator<<(std::ostream &s, const _DictWordAsIndices &dwi)
{
    s << "_DictWordAsIndices(b_word=" << dwi.b_word << ", e_word=" << dwi.e_word
      << ", b_meaning=" << dwi.b_meaning << ", e_meaning=" << dwi.e_meaning << ")";
    return s;
}

// 'bla-bla @ meaning @' is one dict word
size_t _count_num_dict_words(const std::string &file_content)
{
    size_t num_separators = std::count(file_content.cbegin(), file_content.cend(), SEPARATOR);
    if (num_separators % 2 != 0) {
        std::stringstream ess;
        ess << "Number of separators should be even, but now it is odd: " << num_separators;
        throw wr::dict_file_error(ess.str());
    }
    size_t num_words = num_separators / 2;
    return num_words;
}

bool _is_wspace_char(int ch)
{
    if ((ch < 0) || (ch > 255))
        return false;
    return ::isspace(ch);
}

void _strip_dict_word(const std::string &file_content, _DictWordAsIndices &dwi)
{
    // shrink beginning of the word
    for (size_t i = dwi.b_word; i < dwi.e_word; ++i) {
        if (!_is_wspace_char(file_content[i])) {
            dwi.b_word = i;
            break;
        }
    }

    // shrink end of the word
    for (size_t i = dwi.e_word; i >= dwi.b_word; --i) {
        if (!_is_wspace_char(file_content[i])) {
            dwi.e_word = i;
            break;
        }
    }

    // shrink beginning of the meaning
    for (size_t i = dwi.b_meaning; i < dwi.e_meaning; ++i) {
        if (!_is_wspace_char(file_content[i])) {
            dwi.b_meaning = i;
            break;
        }
    }

    // shrink end of the meaning
    for (size_t i = dwi.e_meaning; i >= dwi.b_meaning; --i) {
        if (!_is_wspace_char(file_content[i])) {
            dwi.e_meaning = i;
            break;
        }
    }

    if ((dwi.b_word > dwi.e_word) || (dwi.b_meaning > dwi.e_meaning)) {
        std::stringstream ess;
        ess << "The word or it's meaning seems empty:" << std::endl;
        ess << "    " << dwi << std::endl;
        ess << "    '" << file_content.substr(dwi.b_word, dwi.e_meaning) << "'" << std::endl;
        throw wr::dict_file_error(ess.str());
    }
}

void _check_output(const std::vector<DictWord> &dict)
{
    for (const DictWord &dw : dict) {
        if (dw.word.empty() || dw.meaning.empty()) {
            std::stringstream ess;
            ess << "Something went wrong during the dictionary parsing. The word " << dw
                << " has empty strings";
            throw wr::dict_file_error(ess.str());
        }
    }
}

} // namespace

// -----------------------------------------------------------------------------------------------

DictWord::DictWord(const std::string &word, const std::string &meaning)
    : word(word)
    , meaning(meaning)
{}

std::ostream &operator<<(std::ostream &s, const DictWord &dw)
{
    s << "DictWord(word='" << dw.word << "', meaning='" << dw.meaning << "')";
    return s;
}

namespace wr {

std::vector<DictWord> parse_dict_file(const std::string &filename)
{
    return parse_dict_file(fs::path(filename));
}

std::vector<DictWord> parse_dict_file(const std::filesystem::path &path)
{
    if (fs::is_directory(path)) {
        std::stringstream ess;
        ess << "Input file '" << wr::path_to_string(path) << "' is a directory";
        throw dict_file_error(ess.str());
    }

    std::fstream fin(path);
    if (!fin) {
        std::stringstream ess;
        ess << "Cannot open file '" << fs::absolute(path)
            << "'. Check the access right or the path.";
        throw dict_file_error(ess.str());
    }

    // read the whole file in one go
    size_t file_size = fs::file_size(path);
    std::string content(file_size, '\0');
    fin.read(content.data(), file_size);

    // parse the content
    return parse_dict_file_content(content);
}

std::vector<DictWord> parse_dict_file_content(const std::string &dict_string)
{
    size_t num_dict_words = _count_num_dict_words(dict_string);

    // rough split based on separators only
    std::vector<_DictWordAsIndices> dict_indices(num_dict_words);
    if (dict_string[0] == SEPARATOR) {
        throw dict_file_error("Dict file should not start with '@' symbol");
    }
    size_t current_word_idx = 0;
    bool current_is_word = true; // word / meaning
    size_t b_word = 0;
    for (size_t i = 0; i < dict_string.size(); ++i) {
        if (dict_string[i] == SEPARATOR) {
            if (current_is_word) {
                dict_indices[current_word_idx].b_word = b_word;
                dict_indices[current_word_idx].e_word = i - 1;
                dict_indices[current_word_idx].b_meaning = i + 1;
            } else {
                dict_indices[current_word_idx].e_meaning = i - 1;
                current_word_idx += 1;
                b_word = i + 1;
            }
            current_is_word = !current_is_word;
        }
    }

    if (current_word_idx != dict_indices.size()) {
        throw dict_file_error("something went wrong during parsing: number of words is not "
                              "matching `number of '@' symbols / 2`");
    }

    // strip the strings
    for (_DictWordAsIndices &e : dict_indices)
        _strip_dict_word(dict_string, e);

    // split into word-meaning pairs (make new strings)
    std::vector<DictWord> dict_words;
    dict_words.reserve(dict_indices.size());
    for (const _DictWordAsIndices &dwi : dict_indices) {
        size_t nchar_word = dwi.e_word - dwi.b_word + 1;
        size_t nchar_meaning = dwi.e_meaning - dwi.b_meaning + 1;
        dict_words.push_back(DictWord(dict_string.substr(dwi.b_word, nchar_word),
                                      dict_string.substr(dwi.b_meaning, nchar_meaning)));
    }

    _check_output(dict_words);
    return dict_words;
}

} // namespace wr
