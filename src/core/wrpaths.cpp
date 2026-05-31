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

} // namespace wr