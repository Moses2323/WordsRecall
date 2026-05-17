#pragma once

#include <filesystem>

/*! Default dictionaries folder.
 *
 * Will be used for the application by default.

 * \return Dictionaries folder as filesystem object.
 */
std::filesystem::path get_default_dictionaries_fld();
