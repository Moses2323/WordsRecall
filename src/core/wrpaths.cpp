#include "wrpaths.h"

#include "wrdefined.h"
#include <string>

namespace fs = std::filesystem;

namespace wr {

std::filesystem::path get_default_dictionaries_fld()
{
    fs::path p(wr::gDICT_DEFAULT_FLD);
    if (!fs::exists(p)) {
        std::stringstream ess;
        ess << "Directory '" << fs::absolute(p) << "' does not exist";
        throw wr::file_not_found(std::string(ess.str()));
    }
    if (!fs::is_directory(p)) {
        std::stringstream ess;
        ess << "File '" << fs::absolute(p) << "' is not a directory";
        throw wr::file_not_found(std::string(ess.str()));
    }
    return p;
}

//! \todo need to be tested properly.
std::string path_to_string(const std::filesystem::path &p)
{
    std::string s;
    {
        auto utf8_str = p.u8string();
        s = std::move(std::string(reinterpret_cast<const char *>(utf8_str.c_str()),
                                  utf8_str.size() * sizeof(utf8_str[0])

                                      ));
    }

    return s;
}

} // namespace wr