#include "dict_parser.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::list<DictWord> parse_dict_file(const std::string &filename)
{
    fs::path path(filename);
    // \todo Move to 1 level higher
    if (fs::is_directory(path)) {
        throw dict_file_error("Input file is a directory");
    }

    std::fstream fin(path);
    if (!fin) {
        throw dict_file_error("Cannot open file");
    }

    size_t file_size = fs::file_size(path);
    std::string content(file_size, '\0');
    fin.read(content.data(), file_size);

    return parse_dict_file_content(content);
}

std::list<DictWord> parse_dict_file_content(const std::string &file_content)
{
    std::cout << "file content:" << std::endl;
    std::cout << file_content << "END" << std::endl;
    throw std::exception("not implemented");
}