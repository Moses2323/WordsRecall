#pragma once

#include <src/core/wrdefined.h>
#include <src/core/wrgamedata.h>
#include <src/core/wrsettings.h>
#include <vector>

//! \struct Word with multiple meaning. Not supported by the application => as a warning.
struct WRMultiMeaning
{
    QString word;
    std::vector<QString> meanings;
};

//! \struct Dict merged result.
struct WRMergedResult
{
    //! \brief Merged dicts.
    std::vector<WRGDictWord> merged;
    /*! \brief If multiple meaning for the same word is found across dicts - it goes here.
     *  \details A word with multiple meanings is not supported.
     */
    std::vector<WRMultiMeaning> unexpected_multimeaning;
};

namespace wr {

WRMergedResult merge_dicts(const std::vector<WRDictToggleSetting> &toggles,
                           wr::DictFileEncoding encoding);

} // namespace wr
