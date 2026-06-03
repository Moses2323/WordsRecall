#pragma once

#include <filesystem>
#include <string>

namespace wr {

/*! Default dictionaries folder.
 *
 * Will be used for the application by default.

 * \return Dictionaries folder as filesystem object.
 */
std::filesystem::path get_default_dictionaries_fld();

std::string path_to_string(const std::filesystem::path &p);

} // namespace wr